# Workaround for .NET Native Fiber Re-entrancy Issue

[Windows fibers](https://learn.microsoft.com/windows/win32/procthread/fibers) can be scheduled on a COM thread results in a .NET native crash due to a re-entrancy problem.  This repro provides a small workaround by changing the order in which thread shutdown callback occur, thereby working around the crash.

.NET native applications have a small "stub EXE" that calls the entrypoint for the application.  This EXE is simply a call instruction that invokes `RHBinder__ShimExeMain` on x86 and x64, and `InvokeMain` on arm64.  

The work around is to initialize COM before invoking the applicaiton entrypoint method.

The TestExeShimCoInitFix solution contains 3 projects:

1. **HelperLib** - this is a small library used to build a small dummy DLL used to compile the stub EXE.  This DLL is only used for compilation purposes and isn't needed in the app deployment scenario. It could be built from the command-line via `cl /O2 /LD /Fe<name_of_netnative_app>.dll HelperLib.cpp`

1. **netnative-workaround** - this is a small C++ project that builds a replacement stub EXE.  It could be built from the command-line via `cl /O2 AppShim.cpp /Fe<name_of_netnative_app>.exe /link /SUBSYSTEM:WINDOWS /APPCONTAINER /ENTRY:MainEntryPoint <name_of_netnative_app>.lib ole32.lib /stack:1048576`.  The workaround involves adding the following Target to the vcxproj, hooking the native build process to copy the stub exe to the UWP project:

    ```xml
      <Target Name="PatchExe" AfterTargets="Link" BeforeTargets="FinalizeBuildStatus">
        <Copy SourceFiles="$(OutDir)$(TargetName).exe" DestinationFolder="$(ProjectDir)..\$(TargetName)\PatchExe\$(Platform)\" />
      </Target>
    ```

    We also pass in the `/APPCONTAINER` switch into link.exe

    ```xml
    <AdditionalOptions>/APPCONTAINER</AdditionalOptions>
    ```

    And link against the HelperLib:
    ```...\$(Configuration)\TestExeShimCoInitFix.lib```

1. **TestExeShimCoInitFix** - small test UWP application demonstrating how to patch the stub EXE before the app launches with F5 and before app packaging.  The workaround involves adding the following Target to the .NET native csproj, hooking the .NET native build process to copy the stub exe to the proper locations before AppX registration or packaging occurs:

  ```xml
    <Target Name="PatchExe" Condition="'$(_ProjectNToolchainEnabled)' == 'true'" AfterTargets="BuildNativePackage" BeforeTargets="_GenerateAppxPackageRecipeFile">
      <Copy SourceFiles="$(ProjectDir)PatchExe\$(PlatformName)\$(TargetFileName)" DestinationFolder="$(ProjectDir)$(OutDir)" />
      <Copy SourceFiles="$(ProjectDir)PatchExe\$(PlatformName)\$(TargetFileName)" DestinationFolder="$(ProjectDir)$(OutDir)ilc\" />
      <Copy SourceFiles="$(ProjectDir)PatchExe\$(PlatformName)\$(TargetFileName)" DestinationFolder="$(ProjectDir)$(OutDir)AppX\" />
    </Target>
  ```
