#include "pcan_runtime.h"

void OnDLL_Load();
void OnDLL_Unload();
void OnBus_Connect();
void OnTimer_DTC_DM1_1000();
void OnTimer_DTC_SERVICE_100();
void OnMsg_All(STCAN_MSG message);
extern HANDLE gDtcGuiThread;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    const PCAN_RUNTIME_TIMER timers[] =
    {
        { 1000, OnTimer_DTC_DM1_1000 },
        { 100, OnTimer_DTC_SERVICE_100 }
    };

    if (!PcanRuntimeInitialize("PCAN J1939 DTC Emulator", OnMsg_All,
        OnBus_Connect, timers, sizeof(timers) / sizeof(timers[0])))
        return 1;

    OnDLL_Load();
    if (gDtcGuiThread)
        WaitForSingleObject(gDtcGuiThread, INFINITE);
    PcanRuntimeShutdown();
    OnDLL_Unload();
    return 0;
}
