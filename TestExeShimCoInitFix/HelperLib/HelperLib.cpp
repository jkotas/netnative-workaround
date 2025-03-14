#ifdef _ARM64_
#define ENTRYPOINT InvokeMain
#else
#define ENTRYPOINT RHBinder__ShimExeMain
#endif

extern "C" __declspec(dllexport) void ENTRYPOINT() { }
