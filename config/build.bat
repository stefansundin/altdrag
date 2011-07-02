@echo off

:: For traditional MinGW, set prefix32 to empty string
:: For mingw-w32, set prefix32 to i686-w64-mingw32-
:: For mingw-w64, set prefix64 to x86_64-w64-mingw32-

set prefix32=i686-w64-mingw32-
set prefix64=x86_64-w64-mingw32-

taskkill /IM Config.exe

%prefix32%windres -o window.o window.rc
%prefix32%gcc -o "..\Config.exe" config.c window.o -mwindows -lshlwapi -lcomctl32 -g -DDEBUG

if "%1" == "run" (
	start ..\Config.exe
)
