#pragma once

#include <Windows.h>
#include <PCANBasic.h>

#define GCC_EXTERN
#define GCC_EXPORT

struct STCAN_MSG
{
    unsigned int id;
    bool isExtended;
    unsigned char dlc;
    bool isCanfd;
    bool isRtr;
    unsigned char cluster;
    unsigned char data[64];

    STCAN_MSG(unsigned int messageId = 0, bool extended = false,
        unsigned char length = 0, bool canFd = false);
};

typedef void (*PCAN_RUNTIME_CALLBACK)();
typedef void (*PCAN_RUNTIME_RX_CALLBACK)(STCAN_MSG message);

struct PCAN_RUNTIME_TIMER
{
    unsigned int periodMs;
    PCAN_RUNTIME_CALLBACK callback;
};

void SendMsg(const STCAN_MSG& message);
void Trace(const char* text);

bool PcanRuntimeInitialize(const char* applicationName,
    PCAN_RUNTIME_RX_CALLBACK receiveCallback,
    PCAN_RUNTIME_CALLBACK connectedCallback,
    const PCAN_RUNTIME_TIMER* timers, unsigned int timerCount);
void PcanRuntimeShutdown();
void PcanRuntimeCreateControls(HWND parent, int x, int y, int width);
bool PcanRuntimeHandleCommand(HWND parent, int controlId, int notificationCode);
void PcanRuntimeRefreshStatus(HWND parent);
