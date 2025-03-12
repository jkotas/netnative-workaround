rem Usage: build.cmd <AppName>

cl /O2 /LD /Fe%1.dll HelperLib.cpp
cl /O2 AppShim.cpp /Fe%1.exe /link /SUBSYSTEM:WINDOWS /APPCONTAINER /ENTRY:MainEntryPoint %1.lib ole32.lib /stack:1048576

rem Overwrite %1.exe in the app with the %1.exe produced by this script