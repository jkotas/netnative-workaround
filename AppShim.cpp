#include "Objbase.h"

#ifdef _ARM64_
#define ENTRYPOINT InvokeMain
#else
#define ENTRYPOINT InvokeMain
#endif

extern "C" __declspec(dllimport) void ENTRYPOINT();

extern "C" void MainEntryPoint();

void MainEntryPoint()
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    ENTRYPOINT();
}