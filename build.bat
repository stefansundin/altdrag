@echo off

:: For traditional MinGW, set prefix32 to empty string
:: For mingw-w32, set prefix32 to i686-w64-mingw32-
:: For mingw-w64, set prefix64 to x86_64-w64-mingw32-

set prefix32=i686-w64-mingw32-
set prefix64=x86_64-w64-mingw32-
set l10n=en-US es-ES gl-ES zh-CN

taskkill /IM AltDrag.exe

if not exist build. mkdir build

rem %prefix32%gcc -o build\unhook.exe include\unhook.c
rem "build\unhook.exe"

%prefix32%windres include\altdrag.rc build\altdrag.o
%prefix32%windres include\hooks.rc build\hooks.o

if "%1" == "all" (
	%prefix32%gcc -o build\ini.exe include\ini.c -lshlwapi
	
	@echo.
	echo Building release build
	%prefix32%gcc -o build\AltDrag.exe altdrag.c build\altdrag.o -mwindows -lshlwapi -lwininet -lcomctl32 -O2 -s
	if not exist build\AltDrag.exe. exit /b
	%prefix32%gcc -o build\hooks.dll hooks.c build\hooks.o -mdll -lshlwapi -lcomctl32 -lpsapi -O2 -s
	if not exist build\hooks.dll. exit /b
	
	if "%2" == "x64" (
		%prefix64%windres include\hookwindows_x64.rc build\hookwindows_x64.o
		%prefix64%windres include\hooks.rc build\hooks_x64.o
		%prefix64%gcc -o build\HookWindows_x64.exe hookwindows_x64.c build\hookwindows_x64.o -mwindows -lshlwapi -O2 -s
		if not exist build\HookWindows_x64.exe. exit /b
		%prefix64%gcc -o build\hooks_x64.dll hooks.c build\hooks_x64.o -mdll -lshlwapi -lcomctl32 -lpsapi -O2 -s
		if not exist build\hooks_x64.dll. exit /b
	)
	
	for %%f in (%l10n%) do (
		@echo.
		echo Putting together %%f
		if not exist "build\%%f\AltDrag". mkdir "build\%%f\AltDrag"
		copy build\AltDrag.exe "build\%%f\AltDrag"
		copy build\hooks.dll "build\%%f\AltDrag"
		copy AltDrag.ini "build\%%f\AltDrag"
		build\ini "build\%%f\AltDrag\AltDrag.ini" AltDrag Language %%f
		if "%2" == "x64" (
			copy build\HookWindows_x64.exe "build\%%f\AltDrag"
			copy build\hooks_x64.dll "build\%%f\AltDrag"
		)
	)
	
	@echo.
	echo Building installer
	makensis /V2 installer.nsi
) else (
	%prefix32%gcc -o AltDrag.exe altdrag.c build\altdrag.o -mwindows -lshlwapi -lwininet -lcomctl32 -g -DDEBUG
	%prefix32%gcc -o hooks.dll hooks.c build\hooks.o -mdll -lshlwapi -lcomctl32 -lpsapi -g -DDEBUG
	
	if "%1" == "x64" (
		rem %prefix64%gcc -o build\unhook_x64.exe include\unhook.c
		rem "build\unhook_x64.exe"
		
		%prefix64%windres include\hookwindows_x64.rc build\hookwindows_x64.o
		%prefix64%windres include\hooks.rc build\hooks_x64.o
		%prefix64%gcc -o HookWindows_x64.exe hookwindows_x64.c build\hookwindows_x64.o -mwindows -lshlwapi -g -DDEBUG
		%prefix64%gcc -o hooks_x64.dll hooks.c build\hooks_x64.o -mdll -lshlwapi -lcomctl32 -lpsapi -g -DDEBUG
	)
	
	if "%1" == "run" (
		start AltDrag.exe
	)
)
