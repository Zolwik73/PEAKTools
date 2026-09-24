#include "pcan_runtime.h"

void OnDLL_Load();
void OnDLL_Unload();
void OnBus_Connect();
void OnTimer_FMS_EEC1_20();
void OnTimer_FMS_EEC2_50();
void OnTimer_FMS_100();
void OnTimer_FMS_1000();
void OnTimer_FMS_DI_10000();
void OnTimer_FMS_WAVE_100();
extern HANDLE gFmsGuiThread;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    const PCAN_RUNTIME_TIMER timers[] =
    {
        { 20, OnTimer_FMS_EEC1_20 },
        { 50, OnTimer_FMS_EEC2_50 },
        { 100, OnTimer_FMS_100 },
        { 1000, OnTimer_FMS_1000 },
        { 10000, OnTimer_FMS_DI_10000 },
        { 100, OnTimer_FMS_WAVE_100 }
    };

    if (!PcanRuntimeInitialize("PCAN J1939 Emulator", NULL,
        OnBus_Connect, timers, sizeof(timers) / sizeof(timers[0])))
        return 1;

    OnDLL_Load();
    if (gFmsGuiThread)
        WaitForSingleObject(gFmsGuiThread, INFINITE);
    PcanRuntimeShutdown();
    OnDLL_Unload();
    return 0;
}
