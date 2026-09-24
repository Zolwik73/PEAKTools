#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "resource.h"
#include <commdlg.h>
#include <mmsystem.h>
#include <conio.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#include "PCANBasic.h"

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "comdlg32.lib")

#define STATUS_REFRESH_MS 200
#define SEND_GAP_US_DEFAULT 1000
#define TX_BATCH_MAX_DEFAULT 10
#define TX_BATCH_WINDOW_US_DEFAULT 1000
#define TX_BATCH_MAX_LIMIT 10
#define MAX_CHANNELS 2
#define MAX_LOG_LINE 512
#define RX_BATCH 100
#define SOCKETCAN_ERR_FLAG 0x20000000UL
#define CAN_MAX_DLEN 8
#define CANFD_MAX_DLEN 64
#define CAN_SFF_MASK 0x000007ffUL
#define CAN_EFF_MASK 0x1fffffffUL
#define CANFD_FLAG_BRS 0x01
#define CANFD_FLAG_ESI 0x02
#define CANFD_TX_FLAGS_MASK CANFD_FLAG_BRS

typedef enum {
  FRAME_CLASSIC = 0,
  FRAME_FD = 1
} FrameKind;

typedef struct {
  uint64_t timestamp_us;
  int64_t source_timestamp_us;
  int has_source_timestamp;
  int channel;
  FrameKind kind;
  unsigned int id;
  int extended;
  int remote;
  int error;
  int len;
  unsigned char flags;
  unsigned char data[CANFD_MAX_DLEN];
} LoggedFrame;

typedef struct {
  const char *command;
  const char *file_name;
  const char *input_file_name;
  unsigned int device_index;
  unsigned int nominal_baud;
  unsigned int data_baud;
  int seconds;
  int replay_timing;
  int loop_send;
  int channel_enabled[MAX_CHANNELS];
  int listen_only;
  int classic_can;
  int no_brs;
  int canfd_standard;
  int resistance_enable;
  int send_gap_us;
  int batch_send;
  int batch_max;
  int batch_window_us;
  int bridge;
  int debug;
  const char *gui_stop_event_name;
} ProgramOptions;

typedef struct {
  TPCANHandle channel[MAX_CHANNELS];
  int enabled[MAX_CHANNELS];
  int classic_mode;
} CanDevice;

static volatile LONG program_running = 1;
static LARGE_INTEGER perf_frequency;
static int timer_period_enabled = 0;
static int debug_enabled = 0;
static HANDLE external_stop_event = NULL;

static int keep_running(void)
{
  if (external_stop_event != NULL &&
      WaitForSingleObject(external_stop_event, 0) == WAIT_OBJECT_0)
    InterlockedExchange(&program_running, 0);
  return InterlockedCompareExchange(&program_running, 1, 1) != 0;
}

static void stop_program(void)
{
  InterlockedExchange(&program_running, 0);
}

static BOOL WINAPI console_handler(DWORD ctrl_type)
{
  if (ctrl_type == CTRL_C_EVENT || ctrl_type == CTRL_BREAK_EVENT ||
      ctrl_type == CTRL_CLOSE_EVENT) {
    stop_program();
    return TRUE;
  }

  return FALSE;
}

static void detach_private_console(void)
{
  DWORD console_processes[2];

  if (GetConsoleWindow() != NULL &&
      GetConsoleProcessList(console_processes, 2) == 1)
    FreeConsole();
}

static void enable_precise_timing(void)
{
  if (timeBeginPeriod(1) == TIMERR_NOERROR)
    timer_period_enabled = 1;
}

static void disable_precise_timing(void)
{
  if (timer_period_enabled) {
    timeEndPeriod(1);
    timer_period_enabled = 0;
  }
}

static uint64_t now_us(void)
{
  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);
  return (uint64_t)((counter.QuadPart * 1000000ULL) / perf_frequency.QuadPart);
}

static void clear_key_buffer(void)
{
  if (external_stop_event != NULL)
    return;
  while (_kbhit())
    (void)_getch();
}

static void poll_stop_key(void)
{
  if (external_stop_event != NULL) {
    (void)keep_running();
    return;
  }
  if (_kbhit()) {
    (void)_getch();
    stop_program();
  }
}

static void sleep_until_or_key(uint64_t deadline_us)
{
  while (keep_running()) {
    uint64_t current = now_us();
    uint64_t remaining_us;
    DWORD sleep_ms;

    poll_stop_key();
    if (!keep_running() || current >= deadline_us)
      break;

    remaining_us = deadline_us - current;
    if (remaining_us > 3000) {
      sleep_ms = (DWORD)((remaining_us - 1500) / 1000);
      if (sleep_ms > 20)
        sleep_ms = 20;
      Sleep(sleep_ms);
    } else if (remaining_us > 1000) {
      Sleep(0);
    } else {
      YieldProcessor();
    }
  }
}

static int parse_uint_arg(const char *text, unsigned int *value)
{
  char *end = NULL;
  unsigned long parsed = strtoul(text, &end, 10);

  if (end == text || *end != '\0' || parsed > UINT_MAX)
    return -1;

  *value = (unsigned int)parsed;
  return 0;
}

static int parse_int_arg(const char *text, int *value)
{
  char *end = NULL;
  long parsed = strtol(text, &end, 10);

  if (end == text || *end != '\0' || parsed < 0 || parsed > INT_MAX)
    return -1;

  *value = (int)parsed;
  return 0;
}

static int parse_gap_ms_arg(const char *text, int *value_us)
{
  char *end = NULL;
  double parsed = strtod(text, &end);

  if (end == text || *end != '\0' || parsed < 0.0)
    return -1;

  parsed = (parsed * 1000.0) + 0.5;
  if (parsed > INT_MAX)
    return -1;

  *value_us = (int)parsed;
  return 0;
}

static int has_extension(const char *path, const char *extension)
{
  const char *dot = strrchr(path, '.');

  return dot != NULL && _stricmp(dot, extension) == 0;
}

static int parse_signed_int64(const char *text, int64_t *value)
{
  char *end = NULL;
  __int64 parsed = _strtoi64(text, &end, 10);

  if (end == text || *end != '\0')
    return -1;

  *value = (int64_t)parsed;
  return 0;
}

static int parse_signed_decimal_seconds_us(const char *text, int64_t *timestamp_us)
{
  const char *cursor = text;
  const char *dot;
  char seconds_text[32];
  char micros_text[7] = "000000";
  size_t seconds_len;
  size_t micros_len;
  int negative = 0;
  char *end = NULL;
  unsigned long long seconds;
  unsigned long micros;
  unsigned long long total;

  if (*cursor == '-') {
    negative = 1;
    cursor++;
  } else if (*cursor == '+') {
    cursor++;
  }

  dot = strchr(cursor, '.');
  if (dot == NULL)
    return -1;

  seconds_len = (size_t)(dot - cursor);
  if (seconds_len == 0 || seconds_len >= sizeof(seconds_text))
    return -1;

  memcpy(seconds_text, cursor, seconds_len);
  seconds_text[seconds_len] = '\0';

  micros_len = strlen(dot + 1);
  if (micros_len == 0 || micros_len > 6)
    return -1;

  memcpy(micros_text, dot + 1, micros_len);

  seconds = _strtoui64(seconds_text, &end, 10);
  if (end == seconds_text || *end != '\0')
    return -1;

  micros = strtoul(micros_text, &end, 10);
  if (end == micros_text || *end != '\0' || micros >= 1000000UL)
    return -1;

  total = seconds * 1000000ULL + micros;
  if (total > 9223372036854775807ULL)
    return -1;

  *timestamp_us = negative ? -(int64_t)total : (int64_t)total;
  return 0;
}

static int parse_can_interface(const char *text, int *channel)
{
  if (_stricmp(text, "can0") == 0 || _stricmp(text, "vcan0") == 0) {
    *channel = 0;
    return 0;
  }
  if (_stricmp(text, "can1") == 0 || _stricmp(text, "vcan1") == 0) {
    *channel = 1;
    return 0;
  }
  return -1;
}

static char *trim_left(char *text);
static void trim_right(char *text);

static const char *channel_name(int channel)
{
  return channel == 0 ? "CAN1" : "CAN2";
}

static const char *kind_name(FrameKind kind)
{
  return kind == FRAME_FD ? "CANFD" : "CAN";
}

static int parse_channel_token(const char *text, int *channel)
{
  if (_stricmp(text, "CAN1") == 0 || strcmp(text, "1") == 0) {
    *channel = 0;
    return 0;
  }
  if (_stricmp(text, "CAN2") == 0 || strcmp(text, "2") == 0) {
    *channel = 1;
    return 0;
  }
  return -1;
}

static void set_default_options(ProgramOptions *options)
{
  SecureZeroMemory(options, sizeof(*options));
  options->device_index = 0;
  options->nominal_baud = 500000;
  options->data_baud = 2000000;
  options->seconds = 0;
  options->replay_timing = 1;
  options->loop_send = 0;
  options->channel_enabled[0] = 1;
  options->channel_enabled[1] = 0;
  options->listen_only = 0;
  options->classic_can = 0;
  options->no_brs = 0;
  options->canfd_standard = 0;
  options->resistance_enable = -1;
  options->send_gap_us = SEND_GAP_US_DEFAULT;
  options->batch_send = 0;
  options->batch_max = TX_BATCH_MAX_DEFAULT;
  options->batch_window_us = TX_BATCH_WINDOW_US_DEFAULT;
  options->bridge = 0;
  options->debug = 0;
}

static void debug_log_device_call(const char *name, int channel, UINT result)
{
  if (!debug_enabled)
    return;

  if (channel >= 0)
    fprintf(stderr, "DLL %s %s result=%u\n", name, channel_name(channel), result);
  else
    fprintf(stderr, "DLL %s result=%u\n", name, result);
}

static void debug_log_handle(const char *name, int channel, TPCANHandle handle)
{
  if (!debug_enabled)
    return;

  if (channel >= 0)
    fprintf(stderr, "DLL %s %s handle=0x%X\n", name, channel_name(channel), handle);
  else
    fprintf(stderr, "DLL %s handle=0x%X\n", name, handle);
}

static int pcan_dlc_to_len(BYTE dlc)
{
  static const int lengths[16] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64
  };

  return dlc < 16 ? lengths[dlc] : CANFD_MAX_DLEN;
}

static BYTE pcan_len_to_dlc(int len)
{
  if (len <= 8)
    return (BYTE)len;
  if (len <= 12)
    return 9;
  if (len <= 16)
    return 10;
  if (len <= 20)
    return 11;
  if (len <= 24)
    return 12;
  if (len <= 32)
    return 13;
  if (len <= 48)
    return 14;
  return 15;
}

static int make_pcan_baudrate(unsigned int bitrate, TPCANBaudrate *baudrate)
{
  switch (bitrate) {
  case 1000000:
    *baudrate = PCAN_BAUD_1M;
    return 0;
  case 800000:
    *baudrate = PCAN_BAUD_800K;
    return 0;
  case 500000:
    *baudrate = PCAN_BAUD_500K;
    return 0;
  case 250000:
    *baudrate = PCAN_BAUD_250K;
    return 0;
  case 125000:
    *baudrate = PCAN_BAUD_125K;
    return 0;
  case 100000:
    *baudrate = PCAN_BAUD_100K;
    return 0;
  case 95238:
    *baudrate = PCAN_BAUD_95K;
    return 0;
  case 83333:
    *baudrate = PCAN_BAUD_83K;
    return 0;
  case 50000:
    *baudrate = PCAN_BAUD_50K;
    return 0;
  case 47619:
    *baudrate = PCAN_BAUD_47K;
    return 0;
  case 33333:
    *baudrate = PCAN_BAUD_33K;
    return 0;
  case 20000:
    *baudrate = PCAN_BAUD_20K;
    return 0;
  case 10000:
    *baudrate = PCAN_BAUD_10K;
    return 0;
  case 5000:
    *baudrate = PCAN_BAUD_5K;
    return 0;
  default:
    fprintf(stderr,
            "Unsupported PEAK CAN 2.0 bitrate %u. Use one of: 1000000, 800000, 500000, 250000, 125000, 100000, 95238, 83333, 50000, 47619, 33333, 20000, 10000, 5000.\n",
            bitrate);
    return -1;
  }
}

static TPCANHandle pcan_channel_handle(unsigned int device_index, int channel)
{
  unsigned int bus = device_index * MAX_CHANNELS + (unsigned int)channel + 1;

  if (bus >= 1 && bus <= 8)
    return (TPCANHandle)(PCAN_USBBUS1 + (bus - 1));
  if (bus >= 9 && bus <= 16)
    return (TPCANHandle)(PCAN_USBBUS9 + (bus - 9));
  return PCAN_NONEBUS;
}

static int make_fd_timing(unsigned int bitrate, int data_phase, int *brp,
                          int *tseg1, int *tseg2, int *sjw)
{
  const int clock = 80000000;
  const int total_tq = data_phase ? 20 : 16;
  int denominator;

  if (bitrate == 0)
    return -1;

  denominator = (int)(bitrate * (unsigned int)total_tq);
  if (denominator <= 0 || (clock % denominator) != 0)
    return -1;

  *brp = clock / denominator;
  *tseg1 = data_phase ? 15 : 13;
  *tseg2 = data_phase ? 4 : 2;
  *sjw = data_phase ? 4 : 1;
  return 0;
}

static int make_pcan_fd_bitrate(const ProgramOptions *options, char *buffer,
                                size_t buffer_size)
{
  int nom_brp, nom_tseg1, nom_tseg2, nom_sjw;
  int data_brp, data_tseg1, data_tseg2, data_sjw;

  if (make_fd_timing(options->nominal_baud, 0, &nom_brp, &nom_tseg1,
                     &nom_tseg2, &nom_sjw) != 0 ||
      make_fd_timing(options->data_baud, 1, &data_brp, &data_tseg1,
                     &data_tseg2, &data_sjw) != 0) {
    fprintf(stderr,
            "Unsupported PEAK bitrate pair %u/%u. Use common rates that divide 80 MHz, e.g. 500000/2000000.\n",
            options->nominal_baud, options->data_baud);
    return -1;
  }

  _snprintf_s(buffer, buffer_size, _TRUNCATE,
              "f_clock=80000000,nom_brp=%d,nom_tseg1=%d,nom_tseg2=%d,nom_sjw=%d,"
              "data_brp=%d,data_tseg1=%d,data_tseg2=%d,data_sjw=%d",
              nom_brp, nom_tseg1, nom_tseg2, nom_sjw,
              data_brp, data_tseg1, data_tseg2, data_sjw);
  return 0;
}

static void logged_from_pcan(int channel, const TPCANMsgFD *msg,
                             TPCANTimestampFD timestamp, LoggedFrame *out)
{
  int i;

  SecureZeroMemory(out, sizeof(*out));
  out->timestamp_us = timestamp;
  out->channel = channel;
  out->kind = (msg->MSGTYPE & PCAN_MESSAGE_FD) != 0 ? FRAME_FD : FRAME_CLASSIC;
  out->id = msg->ID;
  out->extended = (msg->MSGTYPE & PCAN_MESSAGE_EXTENDED) != 0;
  out->remote = (msg->MSGTYPE & PCAN_MESSAGE_RTR) != 0;
  out->error = (msg->MSGTYPE & (PCAN_MESSAGE_ERRFRAME | PCAN_MESSAGE_STATUS)) != 0;
  out->flags = 0;
  if ((msg->MSGTYPE & PCAN_MESSAGE_BRS) != 0)
    out->flags |= CANFD_FLAG_BRS;
  if ((msg->MSGTYPE & PCAN_MESSAGE_ESI) != 0)
    out->flags |= CANFD_FLAG_ESI;
  out->len = out->kind == FRAME_FD ? pcan_dlc_to_len(msg->DLC) : msg->DLC;
  if (out->len > CANFD_MAX_DLEN)
    out->len = CANFD_MAX_DLEN;
  if (out->kind == FRAME_CLASSIC && out->len > CAN_MAX_DLEN)
    out->len = CAN_MAX_DLEN;
  for (i = 0; i < out->len; i++)
    out->data[i] = msg->DATA[i];
}

static uint64_t pcan_timestamp_to_us(const TPCANTimestamp *timestamp)
{
  return timestamp->micros +
    (1000ULL * (uint64_t)timestamp->millis) +
    (0x100000000ULL * 1000ULL * (uint64_t)timestamp->millis_overflow);
}

static void logged_from_pcan_classic(int channel, const TPCANMsg *msg,
                                     const TPCANTimestamp *timestamp,
                                     LoggedFrame *out)
{
  int i;

  SecureZeroMemory(out, sizeof(*out));
  out->timestamp_us = pcan_timestamp_to_us(timestamp);
  out->channel = channel;
  out->kind = FRAME_CLASSIC;
  out->id = msg->ID;
  out->extended = (msg->MSGTYPE & PCAN_MESSAGE_EXTENDED) != 0;
  out->remote = (msg->MSGTYPE & PCAN_MESSAGE_RTR) != 0;
  out->error = (msg->MSGTYPE & (PCAN_MESSAGE_ERRFRAME | PCAN_MESSAGE_STATUS)) != 0;
  out->len = msg->LEN;
  if (out->len > CAN_MAX_DLEN)
    out->len = CAN_MAX_DLEN;
  for (i = 0; i < out->len; i++)
    out->data[i] = msg->DATA[i];
}

static int pcan_is_driver_status(const TPCANMsgFD *msg)
{
  return (msg->MSGTYPE & (PCAN_MESSAGE_STATUS | PCAN_MESSAGE_ERROR_NOTIFICATION)) != 0;
}

static int pcan_classic_is_driver_status(const TPCANMsg *msg)
{
  return (msg->MSGTYPE & PCAN_MESSAGE_STATUS) != 0;
}

static int validate_frame(const LoggedFrame *frame)
{
  if (frame->channel < 0 || frame->channel >= MAX_CHANNELS)
    return -1;
  if (frame->id > (frame->extended ? CAN_EFF_MASK : CAN_SFF_MASK))
    return -1;
  if (frame->kind == FRAME_CLASSIC)
    return frame->len >= 0 && frame->len <= CAN_MAX_DLEN ? 0 : -1;
  return frame->len >= 0 && frame->len <= CANFD_MAX_DLEN ? 0 : -1;
}

static int open_device(const ProgramOptions *options, CanDevice *device)
{
  int i;
  char bitrate[256];
  TPCANBaudrate classic_bitrate = PCAN_BAUD_500K;

  SecureZeroMemory(device, sizeof(*device));
  for (i = 0; i < MAX_CHANNELS; i++)
    device->channel[i] = PCAN_NONEBUS;

  device->classic_mode = options->classic_can;
  if (options->classic_can) {
    if (make_pcan_baudrate(options->nominal_baud, &classic_bitrate) != 0)
      return -1;
  } else {
    if (make_pcan_fd_bitrate(options, bitrate, sizeof(bitrate)) != 0)
      return -1;
  }

  printf("Device: PEAK PCAN-Basic USB, base device index: %u, protocol: %s\n",
         options->device_index, options->classic_can ? "CAN 2.0" : "CAN-FD");
  if (!options->classic_can && options->canfd_standard != 0)
    fprintf(stderr, "Warning: --non-iso is not supported by PCAN-Basic in this program and was ignored\n");

  for (i = 0; i < MAX_CHANNELS; i++) {
    TPCANStatus rc;
    TPCANHandle handle;

    if (!options->channel_enabled[i])
      continue;

    handle = pcan_channel_handle(options->device_index, i);
    if (handle == PCAN_NONEBUS) {
      fprintf(stderr, "Unsupported PCAN USB channel number for device index %u, %s\n",
              options->device_index, channel_name(i));
      return -1;
    }

    if (options->resistance_enable >= 0) {
      fprintf(stderr, "Warning: --term/--no-term is not supported by PCAN-Basic and was ignored for %s\n",
              channel_name(i));
    }

    if (options->classic_can) {
      rc = CAN_Initialize(handle, classic_bitrate, 0, 0, 0);
      debug_log_device_call("CAN_Initialize", i, rc);
    } else {
      rc = CAN_InitializeFD(handle, bitrate);
      debug_log_device_call("CAN_InitializeFD", i, rc);
    }
    debug_log_handle("PCAN channel", i, handle);
    if (rc != PCAN_ERROR_OK) {
      fprintf(stderr, "%s failed for %s (handle 0x%X), status=0x%X\n",
              options->classic_can ? "CAN_Initialize" : "CAN_InitializeFD",
              channel_name(i), handle, rc);
      return -1;
    }

    if (options->listen_only) {
      DWORD value = PCAN_PARAMETER_ON;
      rc = CAN_SetValue(handle, PCAN_LISTEN_ONLY, &value, sizeof(value));
      debug_log_device_call("CAN_SetValue(PCAN_LISTEN_ONLY)", i, rc);
      if (rc != PCAN_ERROR_OK)
        fprintf(stderr, "Warning: CAN_SetValue listen-only failed for %s, status=0x%X\n",
                channel_name(i), rc);
    }

    device->channel[i] = handle;
    device->enabled[i] = 1;
  }

  return 0;
}

static void close_device(CanDevice *device)
{
  int i;

  for (i = 0; i < MAX_CHANNELS; i++) {
    if (device->channel[i] != PCAN_NONEBUS) {
      (void)CAN_Reset(device->channel[i]);
      (void)CAN_Uninitialize(device->channel[i]);
    }
  }
}

static void write_log_header(FILE *file, const ProgramOptions *options)
{
  fprintf(file, "# PCAN-Basic candump log\n");
  fprintf(file, "# Format: (seconds.microseconds) can0|can1 id#data or id##flagsdata\n");
  fprintf(file, "# Device index: %u, nominal baud: %u, data baud: %u, mode: %s, channels:%s%s\n",
          options->device_index,
          options->nominal_baud,
          options->data_baud,
          options->listen_only ? "listen-only" : "normal",
          options->channel_enabled[0] ? " CAN1" : "",
          options->channel_enabled[1] ? " CAN2" : "");
  fflush(file);
}

static void write_log_frame(FILE *file, const LoggedFrame *frame)
{
  int i;
  unsigned int id = frame->id;

  if (frame->error)
    id |= SOCKETCAN_ERR_FLAG;

  fprintf(file, "(%llu.%06llu) can%d ",
          (unsigned long long)(frame->timestamp_us / 1000000ULL),
          (unsigned long long)(frame->timestamp_us % 1000000ULL),
          frame->channel);

  if (frame->extended || frame->error)
    fprintf(file, "%08X", id);
  else
    fprintf(file, "%03X", id);

  if (frame->remote) {
    fprintf(file, "#R");
    if (frame->len > 0)
      fprintf(file, "%d", frame->len);
    fputc('\n', file);
    return;
  }

  if (frame->kind == FRAME_FD)
    fprintf(file, "##%X", frame->flags & 0x0f);
  else
    fputc('#', file);

  for (i = 0; i < frame->len; i++)
    fprintf(file, "%02X", frame->data[i]);

  fputc('\n', file);
}

static int read_log_frame(FILE *file, LoggedFrame *frame)
{
  char line[MAX_LOG_LINE];

  while (fgets(line, sizeof(line), file) != NULL) {
    char iface[16];
    char frame_text[160];
    char timestamp_text[64];
    char *hash;
    char *payload;
    char *end;
    int64_t signed_timestamp_us;
    unsigned long parsed_id;
    size_t payload_len;
    int consumed;
    int i;

    if (line[0] == '#' || line[0] == '\r' || line[0] == '\n')
      continue;

    SecureZeroMemory(frame, sizeof(*frame));

    if (sscanf_s(line, " ( %63[^)] ) %15s %159s %n",
                 timestamp_text, (unsigned int)sizeof(timestamp_text),
                 iface, (unsigned int)sizeof(iface),
                 frame_text, (unsigned int)sizeof(frame_text),
                 &consumed) < 3) {
      fprintf(stderr, "Invalid candump log line: %s", line);
      return -1;
    }

    trim_right(timestamp_text);
    if (parse_signed_decimal_seconds_us(trim_left(timestamp_text), &signed_timestamp_us) != 0) {
      fprintf(stderr, "Invalid timestamp in candump log line: %s", line);
      return -1;
    }
    frame->source_timestamp_us = signed_timestamp_us;
    frame->has_source_timestamp = 1;
    frame->timestamp_us = signed_timestamp_us >= 0 ? (uint64_t)signed_timestamp_us : 0;

    if (parse_can_interface(iface, &frame->channel) != 0) {
      fprintf(stderr, "Invalid CAN interface in candump log line: %s", line);
      return -1;
    }

    hash = strchr(frame_text, '#');
    if (hash == NULL) {
      fprintf(stderr, "Invalid candump frame in log line: %s", line);
      return -1;
    }

    *hash = '\0';
    parsed_id = strtoul(frame_text, &end, 16);
    if (end == frame_text || *end != '\0') {
      fprintf(stderr, "Invalid CAN id in candump log line: %s", line);
      return -1;
    }

    frame->error = (parsed_id & SOCKETCAN_ERR_FLAG) != 0;
    if (frame->error)
      parsed_id &= ~SOCKETCAN_ERR_FLAG;
    frame->extended = strlen(frame_text) > 3 || parsed_id > CAN_SFF_MASK;
    if (parsed_id > (frame->extended ? CAN_EFF_MASK : CAN_SFF_MASK)) {
      fprintf(stderr, "Invalid CAN id in candump log line: %s", line);
      return -1;
    }
    frame->id = (unsigned int)parsed_id;

    payload = hash + 1;
    frame->kind = FRAME_CLASSIC;
    frame->remote = 0;
    frame->flags = 0;
    if (payload[0] == '#') {
      frame->kind = FRAME_FD;
      payload++;
      if (*payload == '\0') {
        fprintf(stderr, "Missing CAN-FD flags in candump log line: %s", line);
        return -1;
      }
      if (payload[0] >= '0' && payload[0] <= '9')
        frame->flags = (unsigned char)(payload[0] - '0');
      else if (payload[0] >= 'A' && payload[0] <= 'F')
        frame->flags = (unsigned char)(payload[0] - 'A' + 10);
      else if (payload[0] >= 'a' && payload[0] <= 'f')
        frame->flags = (unsigned char)(payload[0] - 'a' + 10);
      else {
        fprintf(stderr, "Invalid CAN-FD flags in candump log line: %s", line);
        return -1;
      }
      if ((frame->flags & ~(CANFD_FLAG_BRS | CANFD_FLAG_ESI)) != 0) {
        fprintf(stderr, "Unsupported CAN-FD flags in candump log line: %s", line);
        return -1;
      }
      payload++;
    } else if (payload[0] == 'R' || payload[0] == 'r') {
      frame->remote = 1;
      payload++;
      if (*payload != '\0') {
        unsigned long dlc = strtoul(payload, &end, 10);
        if (end == payload || *end != '\0' || dlc > CAN_MAX_DLEN) {
          fprintf(stderr, "Invalid RTR DLC in candump log line: %s", line);
          return -1;
        }
        frame->len = (int)dlc;
      }
      return validate_frame(frame) == 0 ? 1 : -1;
    }

    payload_len = strlen(payload);
    if ((payload_len % 2) != 0 || payload_len / 2 > CANFD_MAX_DLEN) {
      fprintf(stderr, "Invalid CAN data in candump log line: %s", line);
      return -1;
    }

    frame->len = (int)(payload_len / 2);
    for (i = 0; i < frame->len; i++) {
      char byte_text[3];
      unsigned long value;

      byte_text[0] = payload[i * 2];
      byte_text[1] = payload[i * 2 + 1];
      byte_text[2] = '\0';
      value = strtoul(byte_text, &end, 16);
      if (end == byte_text || *end != '\0') {
        fprintf(stderr, "Invalid CAN data byte in candump log line: %s", line);
        return -1;
      }
      frame->data[i] = (unsigned char)value;
    }

    if (validate_frame(frame) != 0)
      return -1;

    return 1;
  }

  return 0;
}

static char *trim_left(char *text)
{
  while (*text == ' ' || *text == '\t' || *text == '\r' || *text == '\n')
    text++;

  return text;
}

static void trim_right(char *text)
{
  size_t len = strlen(text);

  while (len > 0 &&
         (text[len - 1] == ' ' || text[len - 1] == '\t' ||
          text[len - 1] == '\r' || text[len - 1] == '\n')) {
    text[len - 1] = '\0';
    len--;
  }
}

static int parse_decimal_seconds_us(const char *text, uint64_t *timestamp_us)
{
  const char *dot;
  char seconds_text[32];
  char micros_text[7] = "000000";
  size_t seconds_len;
  size_t micros_len;
  char *end = NULL;
  unsigned long long seconds;
  unsigned long micros;

  dot = strchr(text, '.');
  if (dot == NULL)
    return -1;

  seconds_len = (size_t)(dot - text);
  if (seconds_len == 0 || seconds_len >= sizeof(seconds_text))
    return -1;

  memcpy(seconds_text, text, seconds_len);
  seconds_text[seconds_len] = '\0';

  micros_len = strlen(dot + 1);
  if (micros_len == 0 || micros_len > 6)
    return -1;

  memcpy(micros_text, dot + 1, micros_len);

  seconds = _strtoui64(seconds_text, &end, 10);
  if (end == seconds_text || *end != '\0')
    return -1;

  micros = strtoul(micros_text, &end, 10);
  if (end == micros_text || *end != '\0' || micros >= 1000000UL)
    return -1;

  *timestamp_us = (uint64_t)(seconds * 1000000ULL + micros);
  return 0;
}

static int parse_column_sniffer_line(char *line, LoggedFrame *frame,
                                     uint64_t *absolute_time_us)
{
  char *token;
  char *next = NULL;
  char *end = NULL;
  unsigned long channel;
  unsigned long id;
  unsigned long len;
  int i;

  line = trim_left(line);
  trim_right(line);

  token = strtok_s(line, " \t", &next);
  if (token == NULL)
    return 0;
  channel = strtoul(token, &end, 10);
  if (end == token || *end != '\0')
    return 0;
  if (channel >= MAX_CHANNELS)
    return -1;

  token = strtok_s(NULL, " \t", &next);
  if (token == NULL)
    return -1;
  id = strtoul(token, &end, 16);
  if (end == token || *end != '\0' || id > CAN_EFF_MASK)
    return -1;

  token = strtok_s(NULL, " \t", &next);
  if (token == NULL)
    return -1;
  len = strtoul(token, &end, 10);
  if (end == token || *end != '\0' || len > CAN_MAX_DLEN)
    return -1;

  SecureZeroMemory(frame, sizeof(*frame));
  frame->channel = (int)channel;
  frame->kind = FRAME_CLASSIC;
  frame->id = (unsigned int)id;
  frame->extended = id > CAN_SFF_MASK;
  frame->remote = 0;
  frame->error = 0;
  frame->len = (int)len;
  frame->flags = 0;

  for (i = 0; i < frame->len; i++) {
    unsigned long value;

    token = strtok_s(NULL, " \t", &next);
    if (token == NULL)
      return -1;

    value = strtoul(token, &end, 16);
    if (end == token || *end != '\0' || value > 0xff)
      return -1;

    frame->data[i] = (unsigned char)value;
  }

  token = strtok_s(NULL, " \t", &next);
  if (token == NULL || parse_decimal_seconds_us(token, absolute_time_us) != 0)
    return -1;

  token = strtok_s(NULL, " \t", &next);
  if (token == NULL || (strcmp(token, "R") != 0 && strcmp(token, "T") != 0))
    return -1;

  token = strtok_s(NULL, " \t", &next);
  if (token != NULL)
    return -1;

  return validate_frame(frame) == 0 ? 1 : -1;
}

static int parse_pcan_trc_line(char *line, LoggedFrame *frame)
{
  char *token;
  char *next = NULL;
  char *end = NULL;
  double offset_ms;
  unsigned long id;
  unsigned long len;
  int is_fd;
  int i;

  line = trim_left(line);
  trim_right(line);

  token = strtok_s(line, " \t", &next);
  if (token == NULL)
    return 0;
  (void)strtoul(token, &end, 10);
  if (end == token || *end != '\0')
    return 0;

  token = strtok_s(NULL, " \t", &next);
  if (token == NULL)
    return -1;
  offset_ms = strtod(token, &end);
  if (end == token || *end != '\0' || offset_ms < 0.0)
    return -1;

  token = strtok_s(NULL, " \t", &next);
  if (token == NULL)
    return -1;
  if (_stricmp(token, "DT") == 0)
    is_fd = 0;
  else if (_stricmp(token, "FD") == 0)
    is_fd = 1;
  else
    return 0;

  token = strtok_s(NULL, " \t", &next);
  if (token == NULL)
    return -1;
  id = strtoul(token, &end, 16);
  if (end == token || *end != '\0' || id > CAN_EFF_MASK)
    return -1;

  token = strtok_s(NULL, " \t", &next);
  if (token == NULL)
    return -1;
  if (_stricmp(token, "Rx") != 0 && _stricmp(token, "Tx") != 0)
    return -1;

  token = strtok_s(NULL, " \t", &next);
  if (token == NULL)
    return -1;
  len = strtoul(token, &end, 10);
  if (end == token || *end != '\0' ||
      len > (unsigned long)(is_fd ? CANFD_MAX_DLEN : CAN_MAX_DLEN))
    return -1;

  SecureZeroMemory(frame, sizeof(*frame));
  frame->timestamp_us = (uint64_t)((offset_ms * 1000.0) + 0.5);
  frame->channel = 0;
  frame->kind = is_fd ? FRAME_FD : FRAME_CLASSIC;
  frame->id = (unsigned int)id;
  frame->extended = id > CAN_SFF_MASK;
  frame->remote = 0;
  frame->error = 0;
  frame->len = (int)len;
  frame->flags = 0;

  for (i = 0; i < frame->len; i++) {
    unsigned long value;

    token = strtok_s(NULL, " \t", &next);
    if (token == NULL)
      return -1;

    value = strtoul(token, &end, 16);
    if (end == token || *end != '\0' || value > 0xff)
      return -1;

    frame->data[i] = (unsigned char)value;
  }

  return validate_frame(frame) == 0 ? 1 : -1;
}

static int parse_bool_text(const char *text, int *value)
{
  if (_stricmp(text, "true") == 0 || strcmp(text, "1") == 0) {
    *value = 1;
    return 0;
  }
  if (_stricmp(text, "false") == 0 || strcmp(text, "0") == 0) {
    *value = 0;
    return 0;
  }
  return -1;
}

static int parse_gvret_csv_line(char *line, LoggedFrame *frame,
                                int64_t *source_time_us)
{
  char *token;
  char *next = NULL;
  char *end = NULL;
  int extended = 0;
  unsigned long id;
  unsigned long bus;
  unsigned long len;
  int i;

  token = strtok_s(line, ",", &next);
  if (token == NULL)
    return 0;
  trim_right(token);
  if (_stricmp(token, "Time Stamp") == 0)
    return 0;
  if (parse_signed_int64(token, source_time_us) != 0)
    return 0;

  token = strtok_s(NULL, ",", &next);
  if (token == NULL)
    return -1;
  id = strtoul(token, &end, 16);
  if (end == token || *end != '\0' || id > CAN_EFF_MASK)
    return -1;

  token = strtok_s(NULL, ",", &next);
  if (token == NULL || parse_bool_text(token, &extended) != 0)
    return -1;

  token = strtok_s(NULL, ",", &next);
  if (token == NULL)
    return -1;

  token = strtok_s(NULL, ",", &next);
  if (token == NULL)
    return -1;
  bus = strtoul(token, &end, 10);
  if (end == token || *end != '\0' || bus >= MAX_CHANNELS)
    return -1;

  token = strtok_s(NULL, ",", &next);
  if (token == NULL)
    return -1;
  len = strtoul(token, &end, 10);
  if (end == token || *end != '\0' || len > CAN_MAX_DLEN)
    return -1;

  SecureZeroMemory(frame, sizeof(*frame));
  frame->source_timestamp_us = *source_time_us;
  frame->has_source_timestamp = 1;
  frame->channel = (int)bus;
  frame->kind = FRAME_CLASSIC;
  frame->id = (unsigned int)id;
  frame->extended = extended || id > CAN_SFF_MASK;
  frame->len = (int)len;

  for (i = 0; i < frame->len; i++) {
    unsigned long value;

    token = strtok_s(NULL, ",", &next);
    if (token == NULL)
      return -1;
    trim_right(token);
    value = strtoul(token, &end, 16);
    if (end == token || *end != '\0' || value > 0xff)
      return -1;
    frame->data[i] = (unsigned char)value;
  }

  return validate_frame(frame) == 0 ? 1 : -1;
}

static int parse_savvycan_trace_time_us(const char *text, int64_t *source_time_us)
{
  const char *last = strrchr(text, ':');
  char *end = NULL;

  if (last != NULL) {
    int64_t value;
    if (parse_signed_int64(last + 1, &value) != 0)
      return -1;
    *source_time_us = value * 1000;
    return 0;
  }

  {
    double ms = strtod(text, &end);
    if (end == text || *end != '\0')
      return -1;
    *source_time_us = (int64_t)(ms * 1000.0);
    return 0;
  }
}

static int parse_savvycan_trace_line(char *line, LoggedFrame *frame,
                                     int64_t *source_time_us)
{
  char *token;
  char *next = NULL;
  char *end = NULL;
  unsigned long id;
  unsigned long len;
  int i;

  line = trim_left(line);
  trim_right(line);

  token = strtok_s(line, " \t", &next);
  if (token == NULL)
    return 0;
  (void)strtoul(token, &end, 10);
  if (end == token || *end != '\0')
    return 0;

  token = strtok_s(NULL, " \t", &next);
  if (token == NULL || parse_savvycan_trace_time_us(token, source_time_us) != 0)
    return -1;

  token = strtok_s(NULL, " \t", &next);
  if (token == NULL)
    return -1;
  id = strtoul(token, &end, 16);
  if (end == token || *end != '\0' || id > CAN_EFF_MASK)
    return -1;

  token = strtok_s(NULL, " \t", &next);
  if (token == NULL)
    return -1;
  len = strtoul(token, &end, 10);
  if (end == token || *end != '\0' || len > CAN_MAX_DLEN)
    return -1;

  SecureZeroMemory(frame, sizeof(*frame));
  frame->source_timestamp_us = *source_time_us;
  frame->has_source_timestamp = 1;
  frame->channel = 0;
  frame->kind = FRAME_CLASSIC;
  frame->id = (unsigned int)id;
  frame->extended = id > CAN_SFF_MASK;
  frame->len = (int)len;

  for (i = 0; i < frame->len; i++) {
    unsigned long value;

    token = strtok_s(NULL, " \t", &next);
    if (token == NULL)
      return -1;
    value = strtoul(token, &end, 16);
    if (end == token || *end != '\0' || value > 0xff)
      return -1;
    frame->data[i] = (unsigned char)value;
  }

  return validate_frame(frame) == 0 ? 1 : -1;
}

static void write_relative_converted_frame(FILE *output, LoggedFrame *frame,
                                           int64_t source_time_us,
                                           int64_t *base_time_us,
                                           int *have_base_time)
{
  if (!*have_base_time) {
    *base_time_us = source_time_us;
    *have_base_time = 1;
  }

  frame->timestamp_us = source_time_us >= *base_time_us ?
    (uint64_t)(source_time_us - *base_time_us) : 0;
  write_log_frame(output, frame);
}

static int convert_column_sniffer_file(const ProgramOptions *options)
{
  FILE *input;
  FILE *output;
  char line[MAX_LOG_LINE];
  uint64_t base_time_us = 0;
  unsigned long long converted = 0;
  unsigned long long ignored = 0;
  unsigned long long errors = 0;
  int have_base_time = 0;

  if (fopen_s(&input, options->input_file_name, "r") != 0) {
    fprintf(stderr, "Cannot open input file: %s\n", options->input_file_name);
    return -1;
  }

  if (fopen_s(&output, options->file_name, "w") != 0) {
    fprintf(stderr, "Cannot open output file: %s\n", options->file_name);
    fclose(input);
    return -1;
  }

  fprintf(output, "# PCAN-Basic candump log\n");
  fprintf(output, "# Converted from column sniffer file: %s\n", options->input_file_name);
  fprintf(output, "# Format: (seconds.microseconds) can0|can1 id#data or id##flagsdata\n");
  fprintf(output, "# Source format: channel id_hex dlc data_hex... seconds.microseconds R|T\n");

  while (fgets(line, sizeof(line), input) != NULL) {
    char line_copy[MAX_LOG_LINE];
    char *trimmed;
    LoggedFrame frame;
    uint64_t absolute_time_us;
    int parsed;

    strcpy_s(line_copy, sizeof(line_copy), line);
    trimmed = trim_left(line_copy);
    trim_right(trimmed);

    if (trimmed[0] == '\0' || trimmed[0] == '#' || trimmed[0] == ';' ||
        strncmp(trimmed, "//", 2) == 0) {
      ignored++;
      continue;
    }

    strcpy_s(line_copy, sizeof(line_copy), trimmed);
    parsed = parse_column_sniffer_line(line_copy, &frame, &absolute_time_us);
    if (parsed < 0) {
      fprintf(stderr, "Invalid column CAN line: %s\n", line);
      errors++;
      continue;
    }
    if (parsed == 0) {
      ignored++;
      continue;
    }

    if (!have_base_time) {
      base_time_us = absolute_time_us;
      have_base_time = 1;
    }

    frame.timestamp_us = absolute_time_us >= base_time_us ?
      absolute_time_us - base_time_us : 0;
    write_log_frame(output, &frame);
    converted++;
  }

  fclose(input);
  fclose(output);

  printf("Converted %llu frames to %s (%llu ignored, %llu errors)\n",
         converted, options->file_name, ignored, errors);
  return errors == 0 ? 0 : -1;
}

static int convert_gvret_csv_file(const ProgramOptions *options)
{
  FILE *input;
  FILE *output;
  char line[MAX_LOG_LINE];
  int64_t base_time_us = 0;
  unsigned long long converted = 0;
  unsigned long long ignored = 0;
  unsigned long long errors = 0;
  int have_base_time = 0;

  if (fopen_s(&input, options->input_file_name, "r") != 0) {
    fprintf(stderr, "Cannot open input file: %s\n", options->input_file_name);
    return -1;
  }

  if (fopen_s(&output, options->file_name, "w") != 0) {
    fprintf(stderr, "Cannot open output file: %s\n", options->file_name);
    fclose(input);
    return -1;
  }

  fprintf(output, "# PCAN-Basic candump log\n");
  fprintf(output, "# Converted from GVRET CSV log: %s\n", options->input_file_name);
  fprintf(output, "# Format: (seconds.microseconds) can0|can1 id#data or id##flagsdata\n");

  while (fgets(line, sizeof(line), input) != NULL) {
    char line_copy[MAX_LOG_LINE];
    char *trimmed;
    LoggedFrame frame;
    int64_t source_time_us;
    int parsed;

    strcpy_s(line_copy, sizeof(line_copy), line);
    trimmed = trim_left(line_copy);
    trim_right(trimmed);

    if (trimmed[0] == '\0' || trimmed[0] == '#' || trimmed[0] == ';') {
      ignored++;
      continue;
    }

    strcpy_s(line_copy, sizeof(line_copy), trimmed);
    parsed = parse_gvret_csv_line(line_copy, &frame, &source_time_us);
    if (parsed < 0) {
      fprintf(stderr, "Invalid GVRET CSV line: %s\n", line);
      errors++;
      continue;
    }
    if (parsed == 0) {
      ignored++;
      continue;
    }

    write_relative_converted_frame(output, &frame, source_time_us,
                                   &base_time_us, &have_base_time);
    converted++;
  }

  fclose(input);
  fclose(output);

  printf("Converted %llu frames to %s (%llu ignored, %llu errors)\n",
         converted, options->file_name, ignored, errors);
  return errors == 0 ? 0 : -1;
}

static int convert_savvycan_trace_file(const ProgramOptions *options)
{
  FILE *input;
  FILE *output;
  char line[MAX_LOG_LINE];
  int64_t base_time_us = 0;
  unsigned long long converted = 0;
  unsigned long long ignored = 0;
  unsigned long long errors = 0;
  int have_base_time = 0;

  if (fopen_s(&input, options->input_file_name, "r") != 0) {
    fprintf(stderr, "Cannot open input file: %s\n", options->input_file_name);
    return -1;
  }

  if (fopen_s(&output, options->file_name, "w") != 0) {
    fprintf(stderr, "Cannot open output file: %s\n", options->file_name);
    fclose(input);
    return -1;
  }

  fprintf(output, "# PCAN-Basic candump log\n");
  fprintf(output, "# Converted from SavvyCAN trace file: %s\n", options->input_file_name);
  fprintf(output, "# Format: (seconds.microseconds) can0|can1 id#data or id##flagsdata\n");
  fprintf(output, "# Note: colon-separated SavvyCAN trace timestamps are normalized from their last field\n");

  while (fgets(line, sizeof(line), input) != NULL) {
    char line_copy[MAX_LOG_LINE];
    char *trimmed;
    LoggedFrame frame;
    int64_t source_time_us;
    int parsed;

    strcpy_s(line_copy, sizeof(line_copy), line);
    trimmed = trim_left(line_copy);
    trim_right(trimmed);

    if (trimmed[0] == '\0' || trimmed[0] == ';' || trimmed[0] == '#') {
      ignored++;
      continue;
    }

    strcpy_s(line_copy, sizeof(line_copy), trimmed);
    parsed = parse_savvycan_trace_line(line_copy, &frame, &source_time_us);
    if (parsed < 0) {
      fprintf(stderr, "Invalid SavvyCAN trace line: %s\n", line);
      errors++;
      continue;
    }
    if (parsed == 0) {
      ignored++;
      continue;
    }

    write_relative_converted_frame(output, &frame, source_time_us,
                                   &base_time_us, &have_base_time);
    converted++;
  }

  fclose(input);
  fclose(output);

  printf("Converted %llu frames to %s (%llu ignored, %llu errors)\n",
         converted, options->file_name, ignored, errors);
  return errors == 0 ? 0 : -1;
}

static int convert_pcan_trc_file(const ProgramOptions *options)
{
  FILE *input;
  FILE *output;
  char line[MAX_LOG_LINE];
  unsigned long long converted = 0;
  unsigned long long ignored = 0;
  unsigned long long errors = 0;

  if (fopen_s(&input, options->input_file_name, "r") != 0) {
    fprintf(stderr, "Cannot open input file: %s\n", options->input_file_name);
    return -1;
  }

  if (fopen_s(&output, options->file_name, "w") != 0) {
    fprintf(stderr, "Cannot open output file: %s\n", options->file_name);
    fclose(input);
    return -1;
  }

  fprintf(output, "# PCAN-Basic candump log\n");
  fprintf(output, "# Converted from PCAN TRC file: %s\n", options->input_file_name);
  fprintf(output, "# Format: (seconds.microseconds) can0|can1 id#data or id##flagsdata\n");
  fprintf(output, "# Source format: PCAN-View TRC 2.0 columns N,O,T,I,d,l,D\n");

  while (fgets(line, sizeof(line), input) != NULL) {
    char line_copy[MAX_LOG_LINE];
    char *trimmed;
    LoggedFrame frame;
    int parsed;

    strcpy_s(line_copy, sizeof(line_copy), line);
    trimmed = trim_left(line_copy);
    trim_right(trimmed);

    if (trimmed[0] == '\0' || trimmed[0] == ';' || trimmed[0] == '#') {
      ignored++;
      continue;
    }

    strcpy_s(line_copy, sizeof(line_copy), trimmed);
    parsed = parse_pcan_trc_line(line_copy, &frame);
    if (parsed < 0) {
      fprintf(stderr, "Invalid PCAN TRC line: %s\n", line);
      errors++;
      continue;
    }
    if (parsed == 0) {
      ignored++;
      continue;
    }

    write_log_frame(output, &frame);
    converted++;
  }

  fclose(input);
  fclose(output);

  printf("Converted %llu frames to %s (%llu ignored, %llu errors)\n",
         converted, options->file_name, ignored, errors);
  return errors == 0 ? 0 : -1;
}

static int input_looks_like_savvycan_trace(const char *path)
{
  FILE *file;
  char line[MAX_LOG_LINE];
  int lines = 0;

  if (fopen_s(&file, path, "r") != 0)
    return 0;

  while (lines < 20 && fgets(line, sizeof(line), file) != NULL) {
    if (strstr(line, "SavvyCAN") != NULL) {
      fclose(file);
      return 1;
    }
    lines++;
  }

  fclose(file);
  return 0;
}

static int print_status_frame(const LoggedFrame *frame)
{
  int i;
  int written = 0;

  written += printf("%s ", channel_name(frame->channel));
  if (frame->kind == FRAME_FD) {
    written += printf("CANFD ");
    if ((frame->flags & CANFD_FLAG_BRS) != 0)
      written += printf("BRS ");
    if ((frame->flags & CANFD_FLAG_ESI) != 0)
      written += printf("ESI ");
  }

  written += printf("%s %s %X [%d]",
                    frame->extended ? "EXT" : "STD",
                    frame->error ? "ERR" : (frame->remote ? "RTR" : "DATA"),
                    frame->id,
                    frame->len);

  if (frame->len > 0)
    written += printf(" ");

  for (i = 0; i < frame->len; i++) {
    written += printf("%02X", frame->data[i]);
    if (i + 1 < frame->len)
      written += printf(" ");
  }

  return written;
}

static void clear_status_tail(int *previous_len, int current_len)
{
  int i;

  if (current_len < *previous_len) {
    for (i = current_len; i < *previous_len; i++)
      putchar(' ');
  }

  *previous_len = current_len;
}

static void debug_print_frame(const char *prefix, const LoggedFrame *frame)
{
  int i;

  if (!debug_enabled)
    return;

  fprintf(stderr, "%s %s %s %s %s %X [%d] flags=%02X",
          prefix,
          channel_name(frame->channel),
          kind_name(frame->kind),
          frame->extended ? "EXT" : "STD",
          frame->error ? "ERR" : (frame->remote ? "RTR" : "DATA"),
          frame->id,
          frame->len,
          frame->flags);

  for (i = 0; i < frame->len; i++)
    fprintf(stderr, " %02X", frame->data[i]);

  fprintf(stderr, "\n");
}

static void debug_log_api_result(const char *name, int channel, UINT requested, UINT result)
{
  if (debug_enabled)
    fprintf(stderr, "DLL %s %s requested=%u result=%u\n",
            name, channel_name(channel), requested, result);
}

static void debug_log_api_result_timed(const char *name, int channel, UINT requested,
                                       UINT result, uint64_t elapsed_us)
{
  if (debug_enabled)
    fprintf(stderr, "DLL %s %s requested=%u result=%u elapsed=%llu.%03llums\n",
            name, channel_name(channel), requested, result,
            (unsigned long long)(elapsed_us / 1000ULL),
            (unsigned long long)(elapsed_us % 1000ULL));
}

static void print_sniff_status(uint64_t start_us, unsigned long long frames,
                               unsigned long long errors, const LoggedFrame *last)
{
  static int previous_len = 0;
  uint64_t elapsed_us = now_us() - start_us;
  int written;

  written = printf("\rSniff: %llu frames, %llu errors, %llu.%03llus",
                   frames, errors,
                   (unsigned long long)(elapsed_us / 1000000ULL),
                   (unsigned long long)((elapsed_us / 1000ULL) % 1000ULL)) - 1;

  if (last != NULL) {
    written += printf(", last ");
    written += print_status_frame(last);
  } else {
    written += printf("   ");
  }

  clear_status_tail(&previous_len, written);
  fflush(stdout);
}

static int read_pcan_frame(CanDevice *device, int channel, LoggedFrame *frame)
{
  TPCANStatus status;

  if (device->classic_mode) {
    TPCANMsg msg;
    TPCANTimestamp timestamp;

    SecureZeroMemory(&msg, sizeof(msg));
    SecureZeroMemory(&timestamp, sizeof(timestamp));
    status = CAN_Read(device->channel[channel], &msg, &timestamp);
    if (status == PCAN_ERROR_QRCVEMPTY)
      return 0;
    debug_log_device_call("CAN_Read", channel, status);
    if (status != PCAN_ERROR_OK)
      return -1;
    if (pcan_classic_is_driver_status(&msg)) {
      if (debug_enabled)
        fprintf(stderr, "RX %s PCAN status notification ignored, msgtype=0x%02X id=%u\n",
                channel_name(channel), msg.MSGTYPE, msg.ID);
      return 2;
    }
    logged_from_pcan_classic(channel, &msg, &timestamp, frame);
    return 1;
  }

  {
    TPCANMsgFD msg;
    TPCANTimestampFD timestamp;

    SecureZeroMemory(&msg, sizeof(msg));
    timestamp = 0;
    status = CAN_ReadFD(device->channel[channel], &msg, &timestamp);
    if (status == PCAN_ERROR_QRCVEMPTY)
      return 0;
    debug_log_device_call("CAN_ReadFD", channel, status);
    if (status != PCAN_ERROR_OK)
      return -1;
    if (pcan_is_driver_status(&msg)) {
      if (debug_enabled)
        fprintf(stderr, "RX %s PCAN status notification ignored, msgtype=0x%02X id=%u\n",
                channel_name(channel), msg.MSGTYPE, msg.ID);
      return 2;
    }
    logged_from_pcan(channel, &msg, timestamp, frame);
    return 1;
  }
}

static int transmit_classic_frame(CanDevice *device, const LoggedFrame *frame);
static int transmit_fd_or_classic_frame(CanDevice *device, const ProgramOptions *options,
                                        const LoggedFrame *frame);

static int bridge_frame(CanDevice *device, const ProgramOptions *options,
                        const LoggedFrame *source)
{
  LoggedFrame bridged = *source;

  if (!options->bridge)
    return 1;
  if (source->error)
    return 1;

  bridged.channel = source->channel == 0 ? 1 : 0;
  if (!device->enabled[bridged.channel])
    return 0;

  debug_print_frame("BRIDGE", &bridged);
  if (device->classic_mode) {
    if (bridged.kind != FRAME_CLASSIC)
      return 0;
    return transmit_classic_frame(device, &bridged);
  }

  return transmit_fd_or_classic_frame(device, options, &bridged);
}

static int receive_channel_type(CanDevice *device, int channel, int receive_fd,
                                const ProgramOptions *options, uint64_t start_us,
                                FILE *file, LoggedFrame *last,
                                int *have_last, unsigned long long *frames,
                                unsigned long long *errors)
{
  if (!device->enabled[channel])
    return 0;

  while (keep_running()) {
    LoggedFrame frame;
    int read_result = read_pcan_frame(device, channel, &frame);

    if (read_result == 0)
      break;
    if (read_result < 0) {
      (*errors)++;
      break;
    }
    if (read_result == 2)
      continue;
    if (!receive_fd && frame.kind != FRAME_CLASSIC)
      continue;

    frame.timestamp_us = now_us() - start_us;
    debug_print_frame("RX", &frame);
    write_log_frame(file, &frame);
    if (!bridge_frame(device, options, &frame))
      (*errors)++;
    *last = frame;
    *have_last = 1;
    (*frames)++;
  }

  return 0;
}

static int receive_channel(CanDevice *device, int channel, int classic_can,
                           const ProgramOptions *options, uint64_t start_us,
                           FILE *file, LoggedFrame *last,
                           int *have_last, unsigned long long *frames,
                           unsigned long long *errors)
{
  receive_channel_type(device, channel, classic_can ? 0 : 1, options, start_us,
                       file, last, have_last, frames, errors);

  return 0;
}

static unsigned long long poll_send_rx_channel_type(CanDevice *device, int channel,
                                                    int receive_fd)
{
  unsigned long long received = 0;

  if (!device->enabled[channel])
    return 0;

  while (keep_running()) {
    LoggedFrame frame;
    int read_result = read_pcan_frame(device, channel, &frame);

    if (read_result == 0)
      break;
    if (read_result < 0)
      break;
    if (read_result == 2)
      continue;
    if (!receive_fd && frame.kind != FRAME_CLASSIC)
      continue;

    received++;
    debug_print_frame("RX", &frame);
  }

  return received;
}

static unsigned long long poll_send_rx_channel(CanDevice *device, int channel,
                                               int classic_can)
{
  return poll_send_rx_channel_type(device, channel, classic_can ? 0 : 1);
}

static unsigned long long poll_send_rx(CanDevice *device,
                                       const ProgramOptions *options)
{
  unsigned long long received = 0;

  received += poll_send_rx_channel(device, 0, options->classic_can);
  received += poll_send_rx_channel(device, 1, options->classic_can);

  return received;
}

static int sniff_to_file(CanDevice *device, const ProgramOptions *options)
{
  FILE *file;
  uint64_t start_us = now_us();
  uint64_t stop_us = options->seconds > 0 ? start_us + ((uint64_t)options->seconds * 1000000ULL) : 0;
  uint64_t next_status_us = 0;
  unsigned long long frames = 0;
  unsigned long long errors = 0;
  LoggedFrame last;
  int have_last = 0;

  if (fopen_s(&file, options->file_name, "w") != 0) {
    fprintf(stderr, "Cannot open output file: %s\n", options->file_name);
    return -1;
  }

  write_log_header(file, options);
  clear_key_buffer();
  printf("Press any key to stop sniffing.\n");

  while (keep_running()) {
    uint64_t current_us = now_us();

    if (stop_us != 0 && current_us >= stop_us)
      break;

    poll_stop_key();
    receive_channel(device, 0, options->classic_can, options, start_us, file, &last,
                    &have_last, &frames, &errors);
    receive_channel(device, 1, options->classic_can, options, start_us, file, &last,
                    &have_last, &frames, &errors);
    fflush(file);

    current_us = now_us();
    if (current_us >= next_status_us) {
      print_sniff_status(start_us, frames, errors, have_last ? &last : NULL);
      next_status_us = current_us + (STATUS_REFRESH_MS * 1000ULL);
    }

    Sleep(1);
  }

  fflush(file);
  fclose(file);
  print_sniff_status(start_us, frames, errors, have_last ? &last : NULL);
  printf("\nSaved %llu frames to %s\n", frames, options->file_name);
  return 0;
}

static int frames_can_batch(const LoggedFrame *first, const LoggedFrame *next,
                            int batch_window_us)
{
  if (first->channel != next->channel || first->kind != next->kind)
    return 0;
  if (next->timestamp_us < first->timestamp_us)
    return 0;
  if (next->timestamp_us - first->timestamp_us > (uint64_t)batch_window_us)
    return 0;
  return 1;
}

static int transmit_classic_frame(CanDevice *device, const LoggedFrame *frame)
{
  TPCANMsg msg;
  TPCANStatus status;
  uint64_t api_start_us;
  int j;

  SecureZeroMemory(&msg, sizeof(msg));
  msg.ID = frame->id;
  msg.MSGTYPE = frame->extended ? PCAN_MESSAGE_EXTENDED : PCAN_MESSAGE_STANDARD;
  if (frame->remote)
    msg.MSGTYPE |= PCAN_MESSAGE_RTR;
  msg.LEN = (BYTE)frame->len;
  for (j = 0; j < frame->len; j++)
    msg.DATA[j] = frame->data[j];

  api_start_us = now_us();
  status = CAN_Write(device->channel[frame->channel], &msg);
  debug_log_api_result_timed("CAN_Write", frame->channel, 1,
                             status, now_us() - api_start_us);
  if (status != PCAN_ERROR_OK)
    fprintf(stderr, "CAN_Write failed for %s, status=0x%X\n",
            channel_name(frame->channel), status);
  return status == PCAN_ERROR_OK ? 1 : 0;
}

static int transmit_fd_or_classic_frame(CanDevice *device, const ProgramOptions *options,
                                        const LoggedFrame *frame)
{
  TPCANMsgFD msg;
  TPCANStatus status;
  uint64_t api_start_us;
  unsigned char tx_flags_mask = options->no_brs ? 0 : CANFD_TX_FLAGS_MASK;
  int j;

  SecureZeroMemory(&msg, sizeof(msg));
  msg.ID = frame->id;
  msg.MSGTYPE = frame->extended ? PCAN_MESSAGE_EXTENDED : PCAN_MESSAGE_STANDARD;
  if (frame->remote)
    msg.MSGTYPE |= PCAN_MESSAGE_RTR;

  if (frame->kind == FRAME_FD) {
    unsigned char flags = frame->flags & tx_flags_mask;

    msg.MSGTYPE |= PCAN_MESSAGE_FD;
    if ((flags & CANFD_FLAG_BRS) != 0)
      msg.MSGTYPE |= PCAN_MESSAGE_BRS;
    if ((flags & CANFD_FLAG_ESI) != 0)
      msg.MSGTYPE |= PCAN_MESSAGE_ESI;
    msg.DLC = pcan_len_to_dlc(frame->len);
    if (debug_enabled && flags != frame->flags)
      fprintf(stderr, "TX %s CANFD flags masked %02X -> %02X\n",
              channel_name(frame->channel), frame->flags, flags);
  } else {
    msg.DLC = (BYTE)frame->len;
  }

  for (j = 0; j < frame->len; j++)
    msg.DATA[j] = frame->data[j];

  api_start_us = now_us();
  status = CAN_WriteFD(device->channel[frame->channel], &msg);
  debug_log_api_result_timed("CAN_WriteFD", frame->channel, 1,
                             status, now_us() - api_start_us);
  if (status != PCAN_ERROR_OK)
    fprintf(stderr, "CAN_WriteFD failed for %s, status=0x%X\n",
            channel_name(frame->channel), status);
  return status == PCAN_ERROR_OK ? 1 : 0;
}

static int transmit_frames(CanDevice *device, const ProgramOptions *options,
                           const LoggedFrame *frames, int frame_count)
{
  const LoggedFrame *first;
  int i;
  int sent = 0;

  if (frame_count <= 0)
    return 0;

  first = &frames[0];
  if (!device->enabled[first->channel]) {
    fprintf(stderr, "Log uses %s, but that channel is not enabled\n", channel_name(first->channel));
    return -1;
  }

  for (i = 0; i < frame_count; i++) {
    if (frames[i].channel != first->channel || frames[i].kind != first->kind) {
      fprintf(stderr, "Internal batch error: mixed channels or frame types\n");
      return -1;
    }
    debug_print_frame("TX", &frames[i]);
  }

  for (i = 0; i < frame_count; i++) {
    int ok;

    if (device->classic_mode) {
      if (frames[i].kind != FRAME_CLASSIC) {
        fprintf(stderr, "Cannot send CAN-FD frame while PCAN channel is initialized as CAN 2.0\n");
        break;
      }
      ok = transmit_classic_frame(device, &frames[i]);
    } else {
      ok = transmit_fd_or_classic_frame(device, options, &frames[i]);
    }

    if (!ok)
      break;
    sent++;
  }

  return sent;
}
static void print_send_status(uint64_t start_us, unsigned long long sent,
                              unsigned long long errors,
                              unsigned long long received,
                              int loop_send,
                              unsigned long long loops,
                              const LoggedFrame *last)
{
  static int previous_len = 0;
  uint64_t elapsed_us = now_us() - start_us;
  int written;

  written = printf("\rSend: %llu frames, %llu errors, rx %llu, %llu.%03llus",
                   sent, errors, received,
                   (unsigned long long)(elapsed_us / 1000000ULL),
                   (unsigned long long)((elapsed_us / 1000ULL) % 1000ULL)) - 1;

  if (last != NULL) {
    written += printf(", last ");
    written += print_status_frame(last);
  } else {
    written += printf("   ");
  }

  if (loop_send)
    written += printf(", loops=%llu", loops);

  clear_status_tail(&previous_len, written);
  fflush(stdout);
}

static int send_from_file(CanDevice *device, const ProgramOptions *options)
{
  FILE *file;
  uint64_t start_us = now_us();
  int64_t first_log_source_us = 0;
  uint64_t loop_start_us = start_us;
  uint64_t next_send_allowed_us = start_us;
  uint64_t next_status_us = 0;
  unsigned long long sent = 0;
  unsigned long long errors = 0;
  unsigned long long received = 0;
  unsigned long long loops = 0;
  unsigned long long skipped = 0;
  int have_first = 0;
  int have_pending = 0;
  LoggedFrame pending_frame;

  if (fopen_s(&file, options->file_name, "r") != 0) {
    fprintf(stderr, "Cannot open input file: %s\n", options->file_name);
    return -1;
  }

  clear_key_buffer();
  printf("Press any key to stop sending.\n");

  while (keep_running()) {
    LoggedFrame frame;
    LoggedFrame batch[TX_BATCH_MAX_LIMIT];
    int batch_count = 1;
    int sent_now;
    int read_result;
    uint64_t current_us;

    if (have_pending) {
      frame = pending_frame;
      have_pending = 0;
      read_result = 1;
    } else {
      read_result = read_log_frame(file, &frame);
    }

    if (read_result < 0) {
      fprintf(stderr, "\nInvalid log file format\n");
      fclose(file);
      return -1;
    }
    if (read_result == 0) {
      if (!options->loop_send)
        break;

      rewind(file);
      clearerr(file);
      have_first = 0;
      first_log_source_us = 0;
      loop_start_us = now_us();
      loops++;
      continue;
    }

    if (frame.error) {
      skipped++;
      if (debug_enabled)
        debug_print_frame("SKIP", &frame);
      continue;
    }

    batch[0] = frame;
    if (options->classic_can && frame.kind != FRAME_CLASSIC) {
      fprintf(stderr, "\nLog contains CAN-FD frame, but --classic was selected\n");
      fclose(file);
      return -1;
    }

    if (!have_first) {
      first_log_source_us = frame.has_source_timestamp ?
        frame.source_timestamp_us : (int64_t)frame.timestamp_us;
      frame.timestamp_us = 0;
      have_first = 1;
      if (sent > 0) {
        sleep_until_or_key(next_send_allowed_us);
        if (!keep_running())
          break;
      }
    } else {
      uint64_t target_us = next_send_allowed_us;
      int64_t frame_source_us = frame.has_source_timestamp ?
        frame.source_timestamp_us : (int64_t)frame.timestamp_us;
      uint64_t frame_relative_us = frame_source_us >= first_log_source_us ?
        (uint64_t)(frame_source_us - first_log_source_us) : 0;

      frame.timestamp_us = frame_relative_us;
      if (options->replay_timing) {
        uint64_t replay_target_us = loop_start_us + frame_relative_us;
        if (replay_target_us > target_us)
          target_us = replay_target_us;
      }
      sleep_until_or_key(target_us);
      if (!keep_running())
        break;
    }

    if (options->batch_send) {
      while (batch_count < options->batch_max) {
        LoggedFrame next_frame;
        int next_read_result = read_log_frame(file, &next_frame);

        if (next_read_result < 0) {
          fprintf(stderr, "\nInvalid log file format\n");
          fclose(file);
          return -1;
        }
        if (next_read_result == 0)
          break;
        if (next_frame.error) {
          skipped++;
          if (debug_enabled)
            debug_print_frame("SKIP", &next_frame);
          continue;
        }

        {
          int64_t next_source_us = next_frame.has_source_timestamp ?
            next_frame.source_timestamp_us : (int64_t)next_frame.timestamp_us;
          next_frame.timestamp_us = next_source_us >= first_log_source_us ?
            (uint64_t)(next_source_us - first_log_source_us) : 0;
        }

        if (!frames_can_batch(&batch[0], &next_frame, options->batch_window_us)) {
          pending_frame = next_frame;
          have_pending = 1;
          break;
        }
        if (options->classic_can && next_frame.kind != FRAME_CLASSIC) {
          fprintf(stderr, "\nLog contains CAN-FD frame, but --classic was selected\n");
          fclose(file);
          return -1;
        }

        batch[batch_count++] = next_frame;
      }
    }

    sent_now = 0;
    while (sent_now < batch_count) {
      int count = transmit_frames(device, options, batch + sent_now, batch_count - sent_now);
      if (count <= 0) {
        errors++;
        break;
      }
      sent_now += count;
      if (sent_now < batch_count)
        errors++;
    }
    sent += (unsigned long long)sent_now;
    next_send_allowed_us = now_us() +
      ((uint64_t)options->send_gap_us * (uint64_t)sent_now);
    received += poll_send_rx(device, options);

    current_us = now_us();
    if (current_us >= next_status_us) {
      print_send_status(start_us, sent, errors, received,
                        options->loop_send, loops, &batch[batch_count - 1]);
      next_status_us = current_us + (STATUS_REFRESH_MS * 1000ULL);
    }

    poll_stop_key();
  }

  fclose(file);
  print_send_status(start_us, sent, errors, received,
                    options->loop_send, loops, NULL);
  printf("\nSent %llu frames from %s\n", sent, options->file_name);
  if (skipped > 0)
    printf("Skipped %llu error frames\n", skipped);
  return errors == 0 ? 0 : -1;
}

static int parse_channels(const char *text, ProgramOptions *options)
{
  options->channel_enabled[0] = 0;
  options->channel_enabled[1] = 0;

  if (_stricmp(text, "both") == 0 || _stricmp(text, "all") == 0 ||
      strcmp(text, "1,2") == 0 || strcmp(text, "2,1") == 0) {
    options->channel_enabled[0] = 1;
    options->channel_enabled[1] = 1;
    return 0;
  }

  if (_stricmp(text, "CAN1") == 0 || strcmp(text, "1") == 0) {
    options->channel_enabled[0] = 1;
    return 0;
  }

  if (_stricmp(text, "CAN2") == 0 || strcmp(text, "2") == 0) {
    options->channel_enabled[1] = 1;
    return 0;
  }

  return -1;
}

static int parse_options(int argc, char **argv, ProgramOptions *options)
{
  int i;

  set_default_options(options);

  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "sniff") == 0 || strcmp(argv[i], "send") == 0 ||
        strcmp(argv[i], "convert") == 0) {
      options->command = argv[i];
    } else if ((strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) && i + 1 < argc) {
      options->file_name = argv[++i];
    } else if ((strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0) && i + 1 < argc) {
      options->input_file_name = argv[++i];
    } else if ((strcmp(argv[i], "--device") == 0 || strcmp(argv[i], "-d") == 0) && i + 1 < argc) {
      if (parse_uint_arg(argv[++i], &options->device_index) != 0)
        return -1;
    } else if ((strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--speed") == 0 ||
                strcmp(argv[i], "--nominal") == 0) && i + 1 < argc) {
      if (parse_uint_arg(argv[++i], &options->nominal_baud) != 0)
        return -1;
    } else if ((strcmp(argv[i], "--data") == 0 || strcmp(argv[i], "--dbit") == 0) && i + 1 < argc) {
      if (parse_uint_arg(argv[++i], &options->data_baud) != 0)
        return -1;
    } else if ((strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--time") == 0) && i + 1 < argc) {
      if (parse_int_arg(argv[++i], &options->seconds) != 0)
        return -1;
    } else if (strcmp(argv[i], "--send-gap") == 0 && i + 1 < argc) {
      if (parse_gap_ms_arg(argv[++i], &options->send_gap_us) != 0)
        return -1;
    } else if (strcmp(argv[i], "--send-gap-us") == 0 && i + 1 < argc) {
      if (parse_int_arg(argv[++i], &options->send_gap_us) != 0)
        return -1;
    } else if (strcmp(argv[i], "--batch-send") == 0) {
      options->batch_send = 1;
    } else if (strcmp(argv[i], "--batch-max") == 0 && i + 1 < argc) {
      if (parse_int_arg(argv[++i], &options->batch_max) != 0)
        return -1;
    } else if (strcmp(argv[i], "--batch-window-us") == 0 && i + 1 < argc) {
      if (parse_int_arg(argv[++i], &options->batch_window_us) != 0)
        return -1;
    } else if (strcmp(argv[i], "--bridge") == 0) {
      options->bridge = 1;
    } else if ((strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--can") == 0 ||
                strcmp(argv[i], "--channel") == 0) && i + 1 < argc) {
      if (parse_channels(argv[++i], options) != 0)
        return -1;
    } else if (strcmp(argv[i], "--fast") == 0) {
      options->replay_timing = 0;
    } else if (strcmp(argv[i], "--loop") == 0 || strcmp(argv[i], "-l") == 0) {
      options->loop_send = 1;
    } else if (strcmp(argv[i], "--listen") == 0 || strcmp(argv[i], "--silent") == 0) {
      options->listen_only = 1;
    } else if (strcmp(argv[i], "--classic") == 0 || strcmp(argv[i], "--can-classic") == 0) {
      options->classic_can = 1;
    } else if (strcmp(argv[i], "--no-brs") == 0) {
      options->no_brs = 1;
    } else if (strcmp(argv[i], "--iso") == 0) {
      options->canfd_standard = 0;
    } else if (strcmp(argv[i], "--non-iso") == 0) {
      options->canfd_standard = 1;
    } else if (strcmp(argv[i], "--term") == 0) {
      options->resistance_enable = 1;
    } else if (strcmp(argv[i], "--no-term") == 0) {
      options->resistance_enable = 0;
    } else if (strcmp(argv[i], "--debug") == 0) {
      options->debug = 1;
    } else if (strcmp(argv[i], "--gui-stop-event") == 0 && i + 1 < argc) {
      options->gui_stop_event_name = argv[++i];
    } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      return 1;
    } else {
      return -1;
    }
  }

  if (options->command == NULL || options->file_name == NULL)
    return -1;

  if (options->batch_max < 1 || options->batch_max > TX_BATCH_MAX_LIMIT ||
      options->batch_window_us < 0)
    return -1;

  if (options->bridge &&
      (strcmp(options->command, "sniff") != 0 ||
       !options->channel_enabled[0] || !options->channel_enabled[1])) {
    fprintf(stderr, "--bridge requires sniff --can both\n");
    return -1;
  }

  if (options->bridge && options->listen_only) {
    fprintf(stderr, "--bridge cannot be combined with --listen\n");
    return -1;
  }

  if (strcmp(options->command, "convert") == 0) {
    if (options->input_file_name == NULL)
      return -1;
  } else if (options->nominal_baud == 0 || options->data_baud == 0 ||
             (!options->channel_enabled[0] && !options->channel_enabled[1])) {
    return -1;
  }

  return 0;
}

static void display_help(const char *progname)
{
  fprintf(stderr,
    "Usage:\n"
    "  %s sniff -s 500000 --data 2000000 --can both -f capture.log [-t seconds]\n"
    "  %s send  -s 500000 --data 2000000 --can both -f capture.log [--fast] [--loop] [--batch-send]\n"
    "  %s convert -i column-sniffer.txt -f capture.log\n"
    "  %s convert -i PCAN_Std.trc|gvret.csv|savvycan.trace -f capture.log\n"
    "\n"
    "Options:\n"
    "  sniff|send|convert  sniff CAN/CAN-FD frames, replay a log, or convert logs\n"
    "  -f, --file          log file path\n"
    "  -i, --input         input file for convert mode\n"
    "  -s, --speed         nominal/arbitration bitrate, default 500000\n"
    "  --data, --dbit      CAN-FD data bitrate, default 2000000\n"
    "  -c, --can           1, 2, or both; default 1\n"
    "  -d, --device        device index, default 0\n"
    "  -t, --time          sniff duration in seconds; omit for unlimited sniffing\n"
    "  --classic           initialize PEAK as CAN 2.0 and use CAN_Read/CAN_Write\n"
    "  --no-brs            clear CAN-FD BRS flag while sending, including --bridge forwarding\n"
    "  --listen            listen-only/silent mode\n"
    "  --iso|--non-iso     accepted for compatibility; PEAK uses ISO CAN-FD here\n"
    "  --term|--no-term    accepted for compatibility; ignored by PCAN-Basic\n"
    "  --fast              replay without preserving sniffed timing\n"
    "  --send-gap ms       minimum delay between sent frames, default 1.000 ms\n"
    "  --send-gap-us us    same delay in microseconds\n"
    "  --batch-send        group short same-channel bursts for replay scheduling\n"
    "  --batch-max n       maximum frames per batch, 1..10, default 10\n"
    "  --batch-window-us us maximum time span inside a batch, default 1000 us\n"
    "  --bridge            with sniff --can both, forward CAN1 frames to CAN2 and CAN2 frames to CAN1\n"
    "  -l, --loop          keep replaying the input file until a key is pressed\n"
    "  --debug             print TX/RX frames and DLL call results to stderr\n"
    "  -h, --help          show this help\n"
    "\n"
    "Log format, compatible with can-utils candump -L:\n"
    "  (seconds.microseconds) can0|can1 id#data\n"
    "  (seconds.microseconds) can0|can1 id##flagsdata\n"
    "  Standard IDs use 3 hex digits, extended IDs use 8 hex digits.\n"
    "Column convert input:\n"
    "  This convert mode is for column logs.\n"
    "  channel id_hex dlc data_hex... seconds.microseconds R|T\n"
    "PCAN TRC convert input:\n"
    "  PCAN-View TRC 2.0 columns N,O,T,I,d,l,D; output uses can0\n"
    "GVRET/SavvyCAN convert input:\n"
    "  GVRET CSV and SavvyCAN trace exports are normalized to start at timestamp zero\n"
    "  Output is the candump-compatible log format shown above.\n"
    "\n"
    "Any key stops sniffing or sending. Ctrl+C also stops cleanly.\n",
    progname, progname, progname, progname);
}

#define WM_GUI_OUTPUT (WM_APP + 1)
#define WM_GUI_PROCESS_DONE (WM_APP + 2)

typedef struct {
  HWND hwnd;
  HANDLE pipe;
  HANDLE process;
} GuiReaderContext;

static HANDLE gui_process = NULL;
static HANDLE gui_reader_thread = NULL;
static HANDLE gui_output_pipe = NULL;
static HANDLE gui_stop_event = NULL;
static HFONT gui_console_font = NULL;

static HFONT gui_create_console_font(HWND hwnd)
{
  HDC dc = GetDC(hwnd);
  int height = -MulDiv(9, GetDeviceCaps(dc, LOGPIXELSY), 72);

  ReleaseDC(hwnd, dc);
  return CreateFontA(height, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                     DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                     CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, "Consolas");
}

static void gui_append_arg(char *command, size_t size, const char *arg)
{
  strncat_s(command, size, " \"", _TRUNCATE);
  strncat_s(command, size, arg, _TRUNCATE);
  strncat_s(command, size, "\"", _TRUNCATE);
}

static void gui_append_switch_value(char *command, size_t size,
                                    const char *name, const char *value)
{
  if (value[0] == '\0')
    return;
  strncat_s(command, size, " ", _TRUNCATE);
  strncat_s(command, size, name, _TRUNCATE);
  gui_append_arg(command, size, value);
}

static void gui_append_switch(char *command, size_t size, const char *name)
{
  strncat_s(command, size, " ", _TRUNCATE);
  strncat_s(command, size, name, _TRUNCATE);
}

static int gui_is_checked(HWND hwnd, int id)
{
  return SendDlgItemMessageA(hwnd, id, BM_GETCHECK, 0, 0) == BST_CHECKED;
}

static void gui_choose_file(HWND hwnd, int edit_id, int save_dialog)
{
  OPENFILENAMEA ofn;
  char path[MAX_PATH];

  SecureZeroMemory(&ofn, sizeof(ofn));
  SecureZeroMemory(path, sizeof(path));
  GetDlgItemTextA(hwnd, edit_id, path, sizeof(path));

  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = hwnd;
  ofn.lpstrFilter = "Log and trace files\0*.log;*.trc;*.csv;*.trace;*.txt\0All files\0*.*\0";
  ofn.lpstrFile = path;
  ofn.nMaxFile = sizeof(path);
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
  if (!save_dialog)
    ofn.Flags |= OFN_FILEMUSTEXIST;

  if ((save_dialog ? GetSaveFileNameA(&ofn) : GetOpenFileNameA(&ofn)) != 0)
    SetDlgItemTextA(hwnd, edit_id, path);
}

static void gui_set_mode_enabled(HWND hwnd)
{
  int convert = gui_is_checked(hwnd, IDC_GUI_MODE_CONVERT);
  int send = gui_is_checked(hwnd, IDC_GUI_MODE_SEND);
  int sniff = gui_is_checked(hwnd, IDC_GUI_MODE_SNIFF);
  int bridge = sniff && gui_is_checked(hwnd, IDC_GUI_BRIDGE);

  if (bridge) {
    SetDlgItemTextA(hwnd, IDC_GUI_CHANNEL, "both");
    CheckDlgButton(hwnd, IDC_GUI_LISTEN, BST_UNCHECKED);
  }

  EnableWindow(GetDlgItem(hwnd, IDC_GUI_INPUT_FILE), convert);
  EnableWindow(GetDlgItem(hwnd, IDC_GUI_BROWSE_INPUT), convert);
  EnableWindow(GetDlgItem(hwnd, IDC_GUI_SECONDS), sniff);
  EnableWindow(GetDlgItem(hwnd, IDC_GUI_LISTEN), sniff && !bridge);
  EnableWindow(GetDlgItem(hwnd, IDC_GUI_BRIDGE), sniff);
  EnableWindow(GetDlgItem(hwnd, IDC_GUI_LOOP), send);
  EnableWindow(GetDlgItem(hwnd, IDC_GUI_FAST), send);
  EnableWindow(GetDlgItem(hwnd, IDC_GUI_BATCH), send);
  EnableWindow(GetDlgItem(hwnd, IDC_GUI_SPEED), !convert);
  EnableWindow(GetDlgItem(hwnd, IDC_GUI_DATA_SPEED), !convert);
  EnableWindow(GetDlgItem(hwnd, IDC_GUI_CHANNEL), !convert && !bridge);
  EnableWindow(GetDlgItem(hwnd, IDC_GUI_CLASSIC), !convert);
}

static void gui_append_output(HWND hwnd, const char *text)
{
  HWND output = GetDlgItem(hwnd, IDC_GUI_OUTPUT);
  const char *cursor = text;

  while (*cursor != '\0') {
    const char *start;
    LONG end;

    if (*cursor == '\r' && cursor[1] == '\n') {
      end = GetWindowTextLengthA(output);
      SendMessageA(output, EM_SETSEL, end, end);
      SendMessageA(output, EM_REPLACESEL, FALSE, (LPARAM)"\r\n");
      cursor += 2;
      continue;
    }
    if (*cursor == '\r') {
      LRESULT line = SendMessageA(output, EM_LINEFROMCHAR, (WPARAM)-1, 0);
      LRESULT line_start = SendMessageA(output, EM_LINEINDEX, line, 0);
      end = GetWindowTextLengthA(output);
      SendMessageA(output, EM_SETSEL, line_start, end);
      SendMessageA(output, EM_REPLACESEL, FALSE, (LPARAM)"");
      cursor++;
      continue;
    }
    if (*cursor == '\n') {
      SendMessageA(output, EM_REPLACESEL, FALSE, (LPARAM)"\r\n");
      cursor++;
      continue;
    }

    start = cursor;
    while (*cursor != '\0' && *cursor != '\r' && *cursor != '\n')
      cursor++;
    end = GetWindowTextLengthA(output);
    SendMessageA(output, EM_SETSEL, end, end);
    {
      size_t length = (size_t)(cursor - start);
      char *part = (char *)HeapAlloc(GetProcessHeap(), 0, length + 1);
      if (part != NULL) {
        memcpy(part, start, length);
        part[length] = '\0';
        SendMessageA(output, EM_REPLACESEL, FALSE, (LPARAM)part);
        HeapFree(GetProcessHeap(), 0, part);
      }
    }
  }

  SendMessageA(output, EM_SCROLLCARET, 0, 0);
}

static DWORD WINAPI gui_reader_proc(LPVOID parameter)
{
  GuiReaderContext *context = (GuiReaderContext *)parameter;
  char buffer[2049];
  DWORD read_count;

  while (ReadFile(context->pipe, buffer, sizeof(buffer) - 1, &read_count, NULL) &&
         read_count > 0) {
    char *copy;

    buffer[read_count] = '\0';
    copy = _strdup(buffer);
    if (copy != NULL && !PostMessageA(context->hwnd, WM_GUI_OUTPUT, 0, (LPARAM)copy))
      free(copy);
  }

  WaitForSingleObject(context->process, INFINITE);
  PostMessageA(context->hwnd, WM_GUI_PROCESS_DONE, 0, 0);
  HeapFree(GetProcessHeap(), 0, context);
  return 0;
}

static void gui_set_running(HWND hwnd, int running)
{
  EnableWindow(GetDlgItem(hwnd, IDC_GUI_START), !running);
  EnableWindow(GetDlgItem(hwnd, IDC_GUI_HELP), !running);
  EnableWindow(GetDlgItem(hwnd, IDC_GUI_STOP), running);
}

static void gui_finish_process(HWND hwnd)
{
  DWORD exit_code = 0;
  char message[80];

  if (gui_process != NULL)
    GetExitCodeProcess(gui_process, &exit_code);

  _snprintf_s(message, sizeof(message), _TRUNCATE,
              "\r\nProcess finished (exit code %lu).\r\n", exit_code);
  gui_append_output(hwnd, message);

  if (gui_reader_thread != NULL)
    CloseHandle(gui_reader_thread);
  if (gui_output_pipe != NULL)
    CloseHandle(gui_output_pipe);
  if (gui_process != NULL)
    CloseHandle(gui_process);
  if (gui_stop_event != NULL)
    CloseHandle(gui_stop_event);

  gui_reader_thread = NULL;
  gui_output_pipe = NULL;
  gui_process = NULL;
  gui_stop_event = NULL;
  gui_set_running(hwnd, 0);
}

static void gui_build_command(HWND hwnd, char *command, size_t command_size)
{
  char exe[MAX_PATH];
  char log_file[MAX_PATH];
  char input_file[MAX_PATH];
  char speed[32];
  char data_speed[32];
  char channel[32];
  char seconds[32];
  char extra_args[1024];
  int convert;
  int send;

  GetModuleFileNameA(NULL, exe, sizeof(exe));
  GetDlgItemTextA(hwnd, IDC_GUI_LOG_FILE, log_file, sizeof(log_file));
  GetDlgItemTextA(hwnd, IDC_GUI_INPUT_FILE, input_file, sizeof(input_file));
  GetDlgItemTextA(hwnd, IDC_GUI_SPEED, speed, sizeof(speed));
  GetDlgItemTextA(hwnd, IDC_GUI_DATA_SPEED, data_speed, sizeof(data_speed));
  GetDlgItemTextA(hwnd, IDC_GUI_CHANNEL, channel, sizeof(channel));
  GetDlgItemTextA(hwnd, IDC_GUI_SECONDS, seconds, sizeof(seconds));
  GetDlgItemTextA(hwnd, IDC_GUI_EXTRA_ARGS, extra_args, sizeof(extra_args));

  convert = gui_is_checked(hwnd, IDC_GUI_MODE_CONVERT);
  send = gui_is_checked(hwnd, IDC_GUI_MODE_SEND);

  _snprintf_s(command, command_size, _TRUNCATE, "\"%s\"", exe);
  gui_append_switch(command, command_size, convert ? "convert" : (send ? "send" : "sniff"));
  if (convert)
    gui_append_switch_value(command, command_size, "-i", input_file);
  gui_append_switch_value(command, command_size, "-f", log_file);

  if (!convert) {
    gui_append_switch_value(command, command_size, "-s", speed);
    gui_append_switch_value(command, command_size, "--data", data_speed);
    gui_append_switch_value(command, command_size, "--can", channel);
    if (seconds[0] != '\0' && !send)
      gui_append_switch_value(command, command_size, "-t", seconds);
    if (gui_is_checked(hwnd, IDC_GUI_CLASSIC))
      gui_append_switch(command, command_size, "--classic");
    if (gui_is_checked(hwnd, IDC_GUI_LISTEN) && !send)
      gui_append_switch(command, command_size, "--listen");
    if (gui_is_checked(hwnd, IDC_GUI_BRIDGE) && !send)
      gui_append_switch(command, command_size, "--bridge");
    if (gui_is_checked(hwnd, IDC_GUI_LOOP) && send)
      gui_append_switch(command, command_size, "--loop");
    if (gui_is_checked(hwnd, IDC_GUI_FAST) && send)
      gui_append_switch(command, command_size, "--fast");
    if (gui_is_checked(hwnd, IDC_GUI_BATCH) && send)
      gui_append_switch(command, command_size, "--batch-send");
  }

  if (extra_args[0] != '\0') {
    strncat_s(command, command_size, " ", _TRUNCATE);
    strncat_s(command, command_size, extra_args, _TRUNCATE);
  }
}

static void gui_update_command_preview(HWND hwnd)
{
  char command[4096];
  char preview[4096];
  char *exe_end;

  gui_build_command(hwnd, command, sizeof(command));
  exe_end = strchr(command + 1, '"');
  if (command[0] == '"' && exe_end != NULL)
    _snprintf_s(preview, sizeof(preview), _TRUNCATE,
                "USB2PCAN.exe%s", exe_end + 1);
  else
    strcpy_s(preview, sizeof(preview), command);
  SetDlgItemTextA(hwnd, IDC_GUI_COMMAND_PREVIEW, preview);
}

static void gui_launch_command(HWND hwnd, char *command)
{
  STARTUPINFOA si;
  PROCESS_INFORMATION pi;
  SECURITY_ATTRIBUTES sa;
  GuiReaderContext *reader_context;
  HANDLE pipe_write = NULL;
  HANDLE null_input = INVALID_HANDLE_VALUE;
  char event_name[96];

  if (gui_process != NULL)
    return;

  _snprintf_s(event_name, sizeof(event_name), _TRUNCATE,
              "Local\\PCANSnifferGui_%lu_%lu", GetCurrentProcessId(), GetTickCount());
  gui_stop_event = CreateEventA(NULL, TRUE, FALSE, event_name);
  if (gui_stop_event == NULL) {
    MessageBoxA(hwnd, "Cannot create the stop event.", "PCAN Sniffer", MB_ICONERROR);
    return;
  }
  gui_append_switch_value(command, 4096, "--gui-stop-event", event_name);

  SecureZeroMemory(&sa, sizeof(sa));
  sa.nLength = sizeof(sa);
  sa.bInheritHandle = TRUE;
  if (!CreatePipe(&gui_output_pipe, &pipe_write, &sa, 0) ||
      !SetHandleInformation(gui_output_pipe, HANDLE_FLAG_INHERIT, 0)) {
    MessageBoxA(hwnd, "Cannot create the console output pipe.", "PCAN Sniffer", MB_ICONERROR);
    if (pipe_write != NULL)
      CloseHandle(pipe_write);
    if (gui_output_pipe != NULL)
      CloseHandle(gui_output_pipe);
    CloseHandle(gui_stop_event);
    gui_output_pipe = NULL;
    gui_stop_event = NULL;
    return;
  }

  null_input = CreateFileA("NUL", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                           &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (null_input == INVALID_HANDLE_VALUE) {
    MessageBoxA(hwnd, "Cannot open the null input device.", "PCAN Sniffer", MB_ICONERROR);
    CloseHandle(pipe_write);
    CloseHandle(gui_output_pipe);
    CloseHandle(gui_stop_event);
    gui_output_pipe = NULL;
    gui_stop_event = NULL;
    return;
  }

  SecureZeroMemory(&si, sizeof(si));
  SecureZeroMemory(&pi, sizeof(pi));
  si.cb = sizeof(si);
  si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
  si.wShowWindow = SW_HIDE;
  si.hStdInput = null_input;
  si.hStdOutput = pipe_write;
  si.hStdError = pipe_write;

  SetWindowTextA(GetDlgItem(hwnd, IDC_GUI_OUTPUT), "");
  gui_append_output(hwnd, command);
  gui_append_output(hwnd, "\r\n\r\n");

  if (!CreateProcessA(NULL, command, NULL, NULL, TRUE, CREATE_NO_WINDOW,
                      NULL, NULL, &si, &pi)) {
    MessageBoxA(hwnd, "Cannot start the sniffer process.", "PCAN Sniffer", MB_ICONERROR);
    CloseHandle(pipe_write);
    if (null_input != INVALID_HANDLE_VALUE)
      CloseHandle(null_input);
    CloseHandle(gui_output_pipe);
    CloseHandle(gui_stop_event);
    gui_output_pipe = NULL;
    gui_stop_event = NULL;
    return;
  }

  CloseHandle(pipe_write);
  CloseHandle(null_input);
  CloseHandle(pi.hThread);
  gui_process = pi.hProcess;

  reader_context = (GuiReaderContext *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                                                 sizeof(*reader_context));
  if (reader_context == NULL) {
    SetEvent(gui_stop_event);
    MessageBoxA(hwnd, "Cannot start the console reader.", "PCAN Sniffer", MB_ICONERROR);
    return;
  }
  reader_context->hwnd = hwnd;
  reader_context->pipe = gui_output_pipe;
  reader_context->process = gui_process;
  gui_reader_thread = CreateThread(NULL, 0, gui_reader_proc, reader_context, 0, NULL);
  if (gui_reader_thread == NULL) {
    HeapFree(GetProcessHeap(), 0, reader_context);
    SetEvent(gui_stop_event);
    MessageBoxA(hwnd, "Cannot start the console reader.", "PCAN Sniffer", MB_ICONERROR);
    return;
  }
  gui_set_running(hwnd, 1);
}

static void gui_start_command(HWND hwnd)
{
  char command[4096];
  char log_file[MAX_PATH];
  char input_file[MAX_PATH];

  GetDlgItemTextA(hwnd, IDC_GUI_LOG_FILE, log_file, sizeof(log_file));
  GetDlgItemTextA(hwnd, IDC_GUI_INPUT_FILE, input_file, sizeof(input_file));
  if (log_file[0] == '\0') {
    MessageBoxA(hwnd, "Choose the log output/input file first.", "PCAN Sniffer", MB_ICONWARNING);
    return;
  }
  if (gui_is_checked(hwnd, IDC_GUI_MODE_CONVERT) && input_file[0] == '\0') {
    MessageBoxA(hwnd, "Choose the source file to convert first.", "PCAN Sniffer", MB_ICONWARNING);
    return;
  }

  gui_build_command(hwnd, command, sizeof(command));
  gui_launch_command(hwnd, command);
}

static void gui_show_help(HWND hwnd)
{
  char exe[MAX_PATH];
  char command[4096];

  GetModuleFileNameA(NULL, exe, sizeof(exe));
  _snprintf_s(command, sizeof(command), _TRUNCATE, "\"%s\" --help", exe);
  gui_launch_command(hwnd, command);
}

static void gui_position_window_default(HWND hwnd)
{
  RECT rect;
  HWND probe;

  GetWindowRect(hwnd, &rect);
  probe = CreateWindowExA(0, "STATIC", "", WS_OVERLAPPED,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          rect.right - rect.left, rect.bottom - rect.top,
                          NULL, NULL, GetModuleHandleA(NULL), NULL);
  if (probe != NULL) {
    GetWindowRect(probe, &rect);
    DestroyWindow(probe);
    SetWindowPos(hwnd, NULL, rect.left, rect.top, 0, 0,
                 SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
  }
}

static INT_PTR CALLBACK gui_dialog_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
  switch (message) {
  case WM_INITDIALOG:
    gui_console_font = gui_create_console_font(hwnd);
    SendDlgItemMessageA(hwnd, IDC_GUI_OUTPUT, EM_SETLIMITTEXT, 1024 * 1024, 0);
    SendDlgItemMessageA(hwnd, IDC_GUI_OUTPUT, WM_SETFONT, (WPARAM)gui_console_font, TRUE);
    SendDlgItemMessageA(hwnd, IDC_GUI_COMMAND_PREVIEW, WM_SETFONT, (WPARAM)gui_console_font, TRUE);
    CheckDlgButton(hwnd, IDC_GUI_MODE_SNIFF, BST_CHECKED);
    SetDlgItemTextA(hwnd, IDC_GUI_LOG_FILE, "capture.log");
    SetDlgItemTextA(hwnd, IDC_GUI_SPEED, "500000");
    SetDlgItemTextA(hwnd, IDC_GUI_DATA_SPEED, "2000000");
    SetDlgItemTextA(hwnd, IDC_GUI_CHANNEL, "1");
    gui_set_running(hwnd, 0);
    gui_set_mode_enabled(hwnd);
    gui_update_command_preview(hwnd);
    return 0;

  case WM_COMMAND:
    switch (LOWORD(wparam)) {
    case IDC_GUI_MODE_SNIFF:
    case IDC_GUI_MODE_SEND:
    case IDC_GUI_MODE_CONVERT:
      gui_set_mode_enabled(hwnd);
      gui_update_command_preview(hwnd);
      return 0;
    case IDC_GUI_BRIDGE:
      gui_set_mode_enabled(hwnd);
      gui_update_command_preview(hwnd);
      return 0;
    case IDC_GUI_CLASSIC:
    case IDC_GUI_LISTEN:
    case IDC_GUI_LOOP:
    case IDC_GUI_FAST:
    case IDC_GUI_BATCH:
      gui_update_command_preview(hwnd);
      return 0;
    case IDC_GUI_BROWSE_LOG:
      gui_choose_file(hwnd, IDC_GUI_LOG_FILE, gui_is_checked(hwnd, IDC_GUI_MODE_SNIFF));
      return 0;
    case IDC_GUI_BROWSE_INPUT:
      gui_choose_file(hwnd, IDC_GUI_INPUT_FILE, 0);
      return 0;
    case IDC_GUI_START:
      gui_start_command(hwnd);
      return 0;
    case IDC_GUI_STOP:
      if (gui_stop_event != NULL) {
        SetEvent(gui_stop_event);
        EnableWindow(GetDlgItem(hwnd, IDC_GUI_STOP), FALSE);
        gui_append_output(hwnd, "\r\nStopping...\r\n");
      }
      return 0;
    case IDC_GUI_HELP:
      gui_show_help(hwnd);
      return 0;
    }
    if (HIWORD(wparam) == EN_CHANGE) {
      switch (LOWORD(wparam)) {
      case IDC_GUI_LOG_FILE:
      case IDC_GUI_INPUT_FILE:
      case IDC_GUI_SPEED:
      case IDC_GUI_DATA_SPEED:
      case IDC_GUI_CHANNEL:
      case IDC_GUI_SECONDS:
      case IDC_GUI_EXTRA_ARGS:
        gui_update_command_preview(hwnd);
        return 0;
      }
    }
    break;

  case WM_CLOSE:
    if (gui_stop_event != NULL)
      SetEvent(gui_stop_event);
    DestroyWindow(hwnd);
    return TRUE;

  case WM_GUI_OUTPUT:
    if (lparam != 0) {
      gui_append_output(hwnd, (const char *)lparam);
      free((void *)lparam);
    }
    return 0;

  case WM_GUI_PROCESS_DONE:
    gui_finish_process(hwnd);
    return 0;

  case WM_DESTROY:
    if (gui_console_font != NULL) {
      DeleteObject(gui_console_font);
      gui_console_font = NULL;
    }
    PostQuitMessage(0);
    return TRUE;
  }

  return FALSE;
}

static int run_gui(void)
{
  HWND hwnd;
  MSG message;
  HINSTANCE instance = GetModuleHandleA(NULL);

  hwnd = CreateDialogParamA(instance, MAKEINTRESOURCEA(IDD_PCAN_SNIFFER),
                            NULL, gui_dialog_proc, 0);
  if (hwnd == NULL)
    return EXIT_FAILURE;

  gui_position_window_default(hwnd);
  ShowWindow(hwnd, SW_SHOW);
  UpdateWindow(hwnd);

  while (GetMessageA(&message, NULL, 0, 0) > 0) {
    if (!IsDialogMessageA(hwnd, &message)) {
      TranslateMessage(&message);
      DispatchMessageA(&message);
    }
  }

  return (int)message.wParam;
}

int main(int argc, char **argv)
{
  ProgramOptions options;
  CanDevice device;
  int parse_result;
  int result;

  QueryPerformanceFrequency(&perf_frequency);
  SetConsoleCtrlHandler(console_handler, TRUE);

  if (argc == 1) {
    detach_private_console();
    return run_gui();
  }

  parse_result = parse_options(argc, argv, &options);
  if (parse_result != 0) {
    display_help(argv[0]);
    return parse_result > 0 ? EXIT_SUCCESS : EXIT_FAILURE;
  }

  if (options.gui_stop_event_name != NULL) {
    external_stop_event = OpenEventA(SYNCHRONIZE, FALSE,
                                     options.gui_stop_event_name);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
  }

  if (strcmp(options.command, "convert") == 0) {
    if (has_extension(options.input_file_name, ".csv"))
      return convert_gvret_csv_file(&options) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
    if (has_extension(options.input_file_name, ".trace"))
      return convert_savvycan_trace_file(&options) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
    if (has_extension(options.input_file_name, ".trc") &&
        input_looks_like_savvycan_trace(options.input_file_name))
      return convert_savvycan_trace_file(&options) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
    if (has_extension(options.input_file_name, ".trc"))
      return convert_pcan_trc_file(&options) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
    return convert_column_sniffer_file(&options) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
  }

  debug_enabled = options.debug;
  enable_precise_timing();

  if (open_device(&options, &device) != 0) {
    close_device(&device);
    disable_precise_timing();
    return EXIT_FAILURE;
  }

  if (strcmp(options.command, "sniff") == 0)
    result = sniff_to_file(&device, &options);
  else
    result = send_from_file(&device, &options);

  close_device(&device);
  disable_precise_timing();
  return result == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
