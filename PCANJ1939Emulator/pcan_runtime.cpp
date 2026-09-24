#include "pcan_runtime.h"

#include <stdio.h>
#include <string.h>

#define IDC_PCAN_CHANNEL  30001
#define IDC_PCAN_BITRATE  30002
#define IDC_PCAN_REFRESH  30003
#define IDC_PCAN_CONNECT  30004
#define IDC_PCAN_STATUS   30005

#define PCAN_MAX_CHANNELS 64
#define PCAN_MAX_TIMERS   12

struct RuntimeTimerState
{
    unsigned int periodMs;
    PCAN_RUNTIME_CALLBACK callback;
    ULONGLONG nextDue;
    volatile LONG running;
};

static CRITICAL_SECTION gRuntimeLock;
static bool gRuntimeInitialized = false;
static HANDLE gRuntimeStopEvent = NULL;
static HANDLE gRuntimeThread = NULL;
static TPCANHandle gRuntimeChannel = PCAN_NONEBUS;
static bool gRuntimeConnected = false;
static PCAN_RUNTIME_RX_CALLBACK gRuntimeReceiveCallback = NULL;
static PCAN_RUNTIME_CALLBACK gRuntimeConnectedCallback = NULL;
static RuntimeTimerState gRuntimeTimers[PCAN_MAX_TIMERS];
static unsigned int gRuntimeTimerCount = 0;
static volatile LONG gRuntimeTxCount = 0;
static volatile LONG gRuntimeRxCount = 0;
static volatile LONG gRuntimeErrorCount = 0;
static char gRuntimeApplicationName[96] = "PCAN Emulator";

struct BitrateChoice
{
    const char* label;
    TPCANBaudrate value;
};

static const BitrateChoice gBitrates[] =
{
    { "1 Mbit/s", PCAN_BAUD_1M },
    { "800 kbit/s", PCAN_BAUD_800K },
    { "500 kbit/s", PCAN_BAUD_500K },
    { "250 kbit/s", PCAN_BAUD_250K },
    { "125 kbit/s", PCAN_BAUD_125K },
    { "100 kbit/s", PCAN_BAUD_100K },
    { "50 kbit/s", PCAN_BAUD_50K },
    { "20 kbit/s", PCAN_BAUD_20K },
    { "10 kbit/s", PCAN_BAUD_10K }
};

STCAN_MSG::STCAN_MSG(unsigned int messageId, bool extended,
    unsigned char length, bool canFd)
    : id(messageId), isExtended(extended), dlc(length), isCanfd(canFd),
      isRtr(false), cluster(0)
{
    ZeroMemory(data, sizeof(data));
}

void Trace(const char* text)
{
    OutputDebugStringA(text);
    OutputDebugStringA("\r\n");
}

static void runtimeSetConnected(bool connected, TPCANHandle channel)
{
    EnterCriticalSection(&gRuntimeLock);
    gRuntimeConnected = connected;
    gRuntimeChannel = connected ? channel : PCAN_NONEBUS;
    LeaveCriticalSection(&gRuntimeLock);
}

void SendMsg(const STCAN_MSG& message)
{
    TPCANMsg tx;
    TPCANStatus status;

    if (message.isCanfd || message.dlc > 8)
    {
        InterlockedIncrement(&gRuntimeErrorCount);
        return;
    }

    ZeroMemory(&tx, sizeof(tx));
    tx.ID = message.id;
    tx.LEN = message.dlc;
    tx.MSGTYPE = message.isExtended ? PCAN_MESSAGE_EXTENDED : PCAN_MESSAGE_STANDARD;
    if (message.isRtr)
        tx.MSGTYPE = (TPCANMessageType)(tx.MSGTYPE | PCAN_MESSAGE_RTR);
    memcpy(tx.DATA, message.data, message.dlc);

    EnterCriticalSection(&gRuntimeLock);
    if (!gRuntimeConnected)
    {
        LeaveCriticalSection(&gRuntimeLock);
        return;
    }
    status = CAN_Write(gRuntimeChannel, &tx);
    LeaveCriticalSection(&gRuntimeLock);

    if (status == PCAN_ERROR_OK)
        InterlockedIncrement(&gRuntimeTxCount);
    else
        InterlockedIncrement(&gRuntimeErrorCount);
}

static DWORD WINAPI runtimeTimerWork(LPVOID parameter)
{
    RuntimeTimerState* timer = (RuntimeTimerState*)parameter;
    timer->callback();
    InterlockedExchange(&timer->running, 0);
    return 0;
}

static void runtimeReadMessages()
{
    for (;;)
    {
        TPCANMsg rx;
        TPCANTimestamp timestamp;
        TPCANStatus status;
        STCAN_MSG message;

        EnterCriticalSection(&gRuntimeLock);
        if (!gRuntimeConnected)
        {
            LeaveCriticalSection(&gRuntimeLock);
            return;
        }
        status = CAN_Read(gRuntimeChannel, &rx, &timestamp);
        LeaveCriticalSection(&gRuntimeLock);

        if (status == PCAN_ERROR_QRCVEMPTY)
            return;
        if (status != PCAN_ERROR_OK)
        {
            InterlockedIncrement(&gRuntimeErrorCount);
            return;
        }

        message.id = rx.ID;
        message.isExtended = (rx.MSGTYPE & PCAN_MESSAGE_EXTENDED) != 0;
        message.isRtr = (rx.MSGTYPE & PCAN_MESSAGE_RTR) != 0;
        message.dlc = rx.LEN;
        memcpy(message.data, rx.DATA, rx.LEN);
        InterlockedIncrement(&gRuntimeRxCount);
        if (gRuntimeReceiveCallback)
            gRuntimeReceiveCallback(message);
    }
}

static DWORD WINAPI runtimeThreadProc(LPVOID)
{
    while (WaitForSingleObject(gRuntimeStopEvent, 5) == WAIT_TIMEOUT)
    {
        bool connected;
        ULONGLONG now = GetTickCount64();

        EnterCriticalSection(&gRuntimeLock);
        connected = gRuntimeConnected;
        LeaveCriticalSection(&gRuntimeLock);

        if (!connected)
        {
            for (unsigned int i = 0; i < gRuntimeTimerCount; i++)
                gRuntimeTimers[i].nextDue = now + gRuntimeTimers[i].periodMs;
            continue;
        }

        runtimeReadMessages();
        for (unsigned int i = 0; i < gRuntimeTimerCount; i++)
        {
            RuntimeTimerState* timer = &gRuntimeTimers[i];
            if (now >= timer->nextDue)
            {
                do
                    timer->nextDue += timer->periodMs;
                while (timer->nextDue <= now);

                if (InterlockedCompareExchange(&timer->running, 1, 0) == 0)
                {
                    if (!QueueUserWorkItem(runtimeTimerWork, timer, WT_EXECUTEDEFAULT))
                    {
                        InterlockedExchange(&timer->running, 0);
                        InterlockedIncrement(&gRuntimeErrorCount);
                    }
                }
            }
        }
    }
    return 0;
}

static void runtimeDisconnect()
{
    TPCANHandle channel;
    EnterCriticalSection(&gRuntimeLock);
    channel = gRuntimeChannel;
    if (gRuntimeConnected)
        CAN_Uninitialize(channel);
    gRuntimeConnected = false;
    gRuntimeChannel = PCAN_NONEBUS;
    LeaveCriticalSection(&gRuntimeLock);
}

static void runtimePopulateBitrates(HWND parent)
{
    HWND combo = GetDlgItem(parent, IDC_PCAN_BITRATE);
    SendMessage(combo, CB_RESETCONTENT, 0, 0);
    for (unsigned int i = 0; i < sizeof(gBitrates) / sizeof(gBitrates[0]); i++)
    {
        LRESULT index = SendMessageA(combo, CB_ADDSTRING, 0, (LPARAM)gBitrates[i].label);
        SendMessage(combo, CB_SETITEMDATA, index, gBitrates[i].value);
        if (gBitrates[i].value == PCAN_BAUD_250K)
            SendMessage(combo, CB_SETCURSEL, index, 0);
    }
}

static void runtimePopulateChannels(HWND parent)
{
    HWND combo = GetDlgItem(parent, IDC_PCAN_CHANNEL);
    DWORD count = 0;
    TPCANChannelInformation channels[PCAN_MAX_CHANNELS];
    TPCANStatus status;
    int selected = -1;

    SendMessage(combo, CB_RESETCONTENT, 0, 0);
    status = CAN_GetValue(PCAN_NONEBUS, PCAN_ATTACHED_CHANNELS_COUNT,
        &count, sizeof(count));
    if (status != PCAN_ERROR_OK || count == 0)
    {
        SendMessageA(combo, CB_ADDSTRING, 0, (LPARAM)"No attached PCAN channel");
        SendMessage(combo, CB_SETCURSEL, 0, 0);
        return;
    }

    if (count > PCAN_MAX_CHANNELS)
        count = PCAN_MAX_CHANNELS;
    ZeroMemory(channels, sizeof(channels));
    status = CAN_GetValue(PCAN_NONEBUS, PCAN_ATTACHED_CHANNELS,
        channels, count * sizeof(channels[0]));
    if (status != PCAN_ERROR_OK)
        return;

    for (DWORD i = 0; i < count; i++)
    {
        char label[160];
        LRESULT index;
        sprintf_s(label, "%s  handle 0x%X  device %lu%s",
            channels[i].device_name, channels[i].channel_handle,
            channels[i].device_id,
            (channels[i].channel_condition & PCAN_CHANNEL_AVAILABLE) ? "" : "  (occupied)");
        index = SendMessageA(combo, CB_ADDSTRING, 0, (LPARAM)label);
        SendMessage(combo, CB_SETITEMDATA, index, channels[i].channel_handle);
        if (selected < 0 && (channels[i].channel_condition & PCAN_CHANNEL_AVAILABLE))
            selected = (int)index;
    }
    SendMessage(combo, CB_SETCURSEL, selected >= 0 ? selected : 0, 0);
}

static void runtimeUpdateControls(HWND parent)
{
    bool connected;
    EnterCriticalSection(&gRuntimeLock);
    connected = gRuntimeConnected;
    LeaveCriticalSection(&gRuntimeLock);
    SetWindowTextA(GetDlgItem(parent, IDC_PCAN_CONNECT), connected ? "Disconnect" : "Connect");
    EnableWindow(GetDlgItem(parent, IDC_PCAN_CHANNEL), !connected);
    EnableWindow(GetDlgItem(parent, IDC_PCAN_BITRATE), !connected);
    EnableWindow(GetDlgItem(parent, IDC_PCAN_REFRESH), !connected);
}

bool PcanRuntimeInitialize(const char* applicationName,
    PCAN_RUNTIME_RX_CALLBACK receiveCallback,
    PCAN_RUNTIME_CALLBACK connectedCallback,
    const PCAN_RUNTIME_TIMER* timers, unsigned int timerCount)
{
    if (gRuntimeInitialized)
        return true;
    InitializeCriticalSection(&gRuntimeLock);
    gRuntimeInitialized = true;
    strncpy_s(gRuntimeApplicationName, applicationName, _TRUNCATE);
    gRuntimeReceiveCallback = receiveCallback;
    gRuntimeConnectedCallback = connectedCallback;
    gRuntimeTimerCount = timerCount > PCAN_MAX_TIMERS ? PCAN_MAX_TIMERS : timerCount;
    for (unsigned int i = 0; i < gRuntimeTimerCount; i++)
    {
        gRuntimeTimers[i].periodMs = timers[i].periodMs;
        gRuntimeTimers[i].callback = timers[i].callback;
        gRuntimeTimers[i].nextDue = GetTickCount64() + timers[i].periodMs;
        gRuntimeTimers[i].running = 0;
    }
    gRuntimeStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    gRuntimeThread = CreateThread(NULL, 0, runtimeThreadProc, NULL, 0, NULL);
    return gRuntimeStopEvent != NULL && gRuntimeThread != NULL;
}

void PcanRuntimeShutdown()
{
    if (!gRuntimeInitialized)
        return;
    if (gRuntimeStopEvent)
        SetEvent(gRuntimeStopEvent);
    if (gRuntimeThread)
        WaitForSingleObject(gRuntimeThread, 3000);
    for (unsigned int attempt = 0; attempt < 500; attempt++)
    {
        bool busy = false;
        for (unsigned int i = 0; i < gRuntimeTimerCount; i++)
            busy = busy || gRuntimeTimers[i].running != 0;
        if (!busy)
            break;
        Sleep(10);
    }
    runtimeDisconnect();
    if (gRuntimeThread)
        CloseHandle(gRuntimeThread);
    if (gRuntimeStopEvent)
        CloseHandle(gRuntimeStopEvent);
    gRuntimeThread = NULL;
    gRuntimeStopEvent = NULL;
    gRuntimeInitialized = false;
    DeleteCriticalSection(&gRuntimeLock);
}

void PcanRuntimeCreateControls(HWND parent, int x, int y, int width)
{
    int comboWidth = width - 430;
    int bitrateX;
    if (width < 600)
    {
        CreateWindowExA(0, "BUTTON", "PEAK PCAN connection", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
            x, y, width, 144, parent, NULL, GetModuleHandle(NULL), NULL);
        CreateWindowExA(0, "STATIC", "Interface", WS_CHILD | WS_VISIBLE,
            x + 12, y + 25, 54, 20, parent, NULL, GetModuleHandle(NULL), NULL);
        CreateWindowExA(0, "COMBOBOX", "", WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST | WS_VSCROLL,
            x + 68, y + 21, width - 80, 200, parent, (HMENU)IDC_PCAN_CHANNEL, GetModuleHandle(NULL), NULL);
        CreateWindowExA(0, "STATIC", "Bitrate", WS_CHILD | WS_VISIBLE,
            x + 12, y + 59, 54, 20, parent, NULL, GetModuleHandle(NULL), NULL);
        CreateWindowExA(0, "COMBOBOX", "", WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST,
            x + 68, y + 55, 105, 180, parent, (HMENU)IDC_PCAN_BITRATE, GetModuleHandle(NULL), NULL);
        CreateWindowExA(0, "BUTTON", "Refresh", WS_CHILD | WS_VISIBLE,
            x + width - 102, y + 54, 90, 25, parent, (HMENU)IDC_PCAN_REFRESH, GetModuleHandle(NULL), NULL);
        CreateWindowExA(0, "BUTTON", "Connect", WS_CHILD | WS_VISIBLE,
            x + width - 102, y + 84, 90, 25, parent, (HMENU)IDC_PCAN_CONNECT, GetModuleHandle(NULL), NULL);
        CreateWindowExA(0, "STATIC", "Disconnected", WS_CHILD | WS_VISIBLE,
            x + 12, y + 92, width - 126, 38, parent, (HMENU)IDC_PCAN_STATUS, GetModuleHandle(NULL), NULL);
        runtimePopulateChannels(parent);
        runtimePopulateBitrates(parent);
        runtimeUpdateControls(parent);
        return;
    }
    if (comboWidth < 210)
        comboWidth = 210;
    bitrateX = x + 68 + comboWidth + 12;

    CreateWindowExA(0, "BUTTON", "PEAK PCAN connection", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        x, y, width, 84, parent, NULL, GetModuleHandle(NULL), NULL);
    CreateWindowExA(0, "STATIC", "Interface", WS_CHILD | WS_VISIBLE,
        x + 12, y + 25, 54, 20, parent, NULL, GetModuleHandle(NULL), NULL);
    CreateWindowExA(0, "COMBOBOX", "", WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST | WS_VSCROLL,
        x + 68, y + 21, comboWidth, 200, parent, (HMENU)IDC_PCAN_CHANNEL, GetModuleHandle(NULL), NULL);
    CreateWindowExA(0, "STATIC", "Bitrate", WS_CHILD | WS_VISIBLE,
        bitrateX, y + 25, 48, 20, parent, NULL, GetModuleHandle(NULL), NULL);
    CreateWindowExA(0, "COMBOBOX", "", WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST,
        bitrateX + 48, y + 21, 105, 180, parent, (HMENU)IDC_PCAN_BITRATE, GetModuleHandle(NULL), NULL);
    CreateWindowExA(0, "BUTTON", "Refresh", WS_CHILD | WS_VISIBLE,
        x + width - 188, y + 20, 78, 25, parent, (HMENU)IDC_PCAN_REFRESH, GetModuleHandle(NULL), NULL);
    CreateWindowExA(0, "BUTTON", "Connect", WS_CHILD | WS_VISIBLE,
        x + width - 102, y + 20, 90, 25, parent, (HMENU)IDC_PCAN_CONNECT, GetModuleHandle(NULL), NULL);
    CreateWindowExA(0, "STATIC", "Disconnected", WS_CHILD | WS_VISIBLE,
        x + 12, y + 55, width - 24, 20, parent, (HMENU)IDC_PCAN_STATUS, GetModuleHandle(NULL), NULL);
    runtimePopulateChannels(parent);
    runtimePopulateBitrates(parent);
    runtimeUpdateControls(parent);
}

bool PcanRuntimeHandleCommand(HWND parent, int controlId, int notificationCode)
{
    if (notificationCode != BN_CLICKED)
        return false;
    if (controlId == IDC_PCAN_REFRESH)
    {
        runtimePopulateChannels(parent);
        PcanRuntimeRefreshStatus(parent);
        return true;
    }
    if (controlId != IDC_PCAN_CONNECT)
        return false;

    EnterCriticalSection(&gRuntimeLock);
    bool connected = gRuntimeConnected;
    LeaveCriticalSection(&gRuntimeLock);
    if (connected)
    {
        runtimeDisconnect();
    }
    else
    {
        HWND channelCombo = GetDlgItem(parent, IDC_PCAN_CHANNEL);
        HWND bitrateCombo = GetDlgItem(parent, IDC_PCAN_BITRATE);
        LRESULT channelIndex = SendMessage(channelCombo, CB_GETCURSEL, 0, 0);
        LRESULT bitrateIndex = SendMessage(bitrateCombo, CB_GETCURSEL, 0, 0);
        if (channelIndex != CB_ERR && bitrateIndex != CB_ERR)
        {
            TPCANHandle channel = (TPCANHandle)SendMessage(channelCombo, CB_GETITEMDATA, channelIndex, 0);
            TPCANBaudrate bitrate = (TPCANBaudrate)SendMessage(bitrateCombo, CB_GETITEMDATA, bitrateIndex, 0);
            TPCANStatus status = CAN_Initialize(channel, bitrate, 0, 0, 0);
            if (status == PCAN_ERROR_OK)
            {
                runtimeSetConnected(true, channel);
                InterlockedExchange(&gRuntimeTxCount, 0);
                InterlockedExchange(&gRuntimeRxCount, 0);
                InterlockedExchange(&gRuntimeErrorCount, 0);
                if (gRuntimeConnectedCallback)
                    gRuntimeConnectedCallback();
            }
            else
            {
                InterlockedIncrement(&gRuntimeErrorCount);
                MessageBoxA(parent, "PCAN-Basic could not initialize the selected channel.",
                    gRuntimeApplicationName, MB_OK | MB_ICONERROR);
            }
        }
    }
    runtimeUpdateControls(parent);
    PcanRuntimeRefreshStatus(parent);
    return true;
}

void PcanRuntimeRefreshStatus(HWND parent)
{
    char status[256];
    bool connected;
    TPCANHandle channel;
    EnterCriticalSection(&gRuntimeLock);
    connected = gRuntimeConnected;
    channel = gRuntimeChannel;
    LeaveCriticalSection(&gRuntimeLock);
    if (connected)
        sprintf_s(status, "Connected to handle 0x%X\r\nRX %ld    TX %ld    errors %ld",
            channel, gRuntimeRxCount, gRuntimeTxCount, gRuntimeErrorCount);
    else
        sprintf_s(status, "Disconnected\r\nErrors %ld", gRuntimeErrorCount);
    SetWindowTextA(GetDlgItem(parent, IDC_PCAN_STATUS), status);
}
