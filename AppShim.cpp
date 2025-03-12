#include "Objbase.h"

#ifdef _ARM64_
#define ENTRYPOINT InvokeMain
#else
#define ENTRYPOINT RHBinder__ShimExeMain
#endif

extern "C" __declspec(dllimport) void ENTRYPOINT();

extern "C" void MainEntryPoint();

void MainEntryPoint()
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    ENTRYPOINT();
}