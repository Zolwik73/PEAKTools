#include "pcan_runtime.h"

void OnDLL_Load();
void OnDLL_Unload();
void OnBus_Connect();
void OnTimer_CIA422_FAST_100();
void OnTimer_CIA422_HEARTBEAT_1000();
void OnMsg_All(STCAN_MSG message);
extern HANDLE gCiaGuiThread;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    const PCAN_RUNTIME_TIMER timers[] =
    {
        { 100, OnTimer_CIA422_FAST_100 },
        { 1000, OnTimer_CIA422_HEARTBEAT_1000 }
    };

    if (!PcanRuntimeInitialize("PCAN CANopen Emulator", OnMsg_All,
        OnBus_Connect, timers, sizeof(timers) / sizeof(timers[0])))
        return 1;

    OnDLL_Load();
    if (gCiaGuiThread)
        WaitForSingleObject(gCiaGuiThread, INFINITE);
    PcanRuntimeShutdown();
    OnDLL_Unload();
    return 0;
}
