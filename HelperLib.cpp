#ifdef _ARM64_
#define ENTRYPOINT InvokeMain
#else
#define ENTRYPOINT InvokeMain
#endif

extern "C" __declspec(dllexport) void ENTRYPOINT() { }
