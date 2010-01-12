@echo off

set prefix32=i686-w64-mingw32-
set prefix64=x86_64-w64-mingw32-

taskkill /IM AltDrag.exe

if not exist build. mkdir build

%prefix32%gcc -o build\unhook.exe include\unhook.c -march=pentium2
"build\unhook.exe"

%prefix32%windres -o build\altdrag.o include\altdrag.rc
%prefix32%windres -o build\hooks.o include\hooks.rc

if "%1" == "all" (
	%prefix32%gcc -o build\ini.exe include\ini.c -lshlwapi -march=pentium2
	
	@echo.
	echo Building binaries
	if not exist "build\en-US\AltDrag". mkdir "build\en-US\AltDrag"
	%prefix32%gcc -o "build\en-US\AltDrag\AltDrag.exe" altdrag.c build\altdrag.o -mwindows -lshlwapi -lwininet -march=pentium2 -O2 -s
	if not exist "build\en-US\AltDrag\AltDrag.exe". exit /b
	%prefix32%gcc -o "build\en-US\AltDrag\hooks.dll" hooks.c build\hooks.o include\libcomctl32.a -mdll -lshlwapi -lcomctl32 -march=pentium2 -O2 -s
	if not exist "build\en-US\AltDrag\hooks.dll". exit /b
	
	if "%2" == "x64" (
		%prefix64%windres -o build\hookwindows_x64.o include\hookwindows_x64.rc
		%prefix64%windres -o build\hooks_x64.o include\hooks_x64.rc
		%prefix64%gcc -o "build\en-US\AltDrag\HookWindows_x64.exe" hookwindows_x64.c build\hookwindows_x64.o -mwindows -lshlwapi -O2 -s
		if not exist "build\en-US\AltDrag\HookWindows_x64.exe". exit /b
		%prefix64%gcc -o "build\en-US\AltDrag\hooks_x64.dll" hooks.c build\hooks_x64.o -mdll -lshlwapi -lcomctl32 -O2 -s
		if not exist "build\en-US\AltDrag\hooks_x64.dll". exit /b
	)
	
	for /D %%f in (localization/*) do (
		if not exist "localization\%%f\outdated.txt" (
			@echo.
			echo Putting together %%f
			if not %%f == en-US (
				if not exist "build\%%f\AltDrag". mkdir "build\%%f\AltDrag"
				copy "build\en-US\AltDrag\AltDrag.exe" "build\%%f\AltDrag"
				copy "build\en-US\AltDrag\hooks.dll" "build\%%f\AltDrag"
				if "%2" == "x64" (
					copy "build\en-US\AltDrag\HookWindows_x64.exe" "build\%%f\AltDrag"
					copy "build\en-US\AltDrag\hooks_x64.dll" "build\%%f\AltDrag"
				)
			)
			copy "localization\%%f\info.txt" "build\%%f\AltDrag"
			copy AltDrag.ini "build\%%f\AltDrag"
			"build\ini.exe" "build\%%f\AltDrag\AltDrag.ini" AltDrag Language %%f
		)
	)
	
	@echo.
	echo Building installer
	makensis /V2 installer.nsi
) else (
	%prefix32%gcc -o AltDrag.exe altdrag.c build\altdrag.o -mwindows -lshlwapi -lwininet -march=pentium2 -DDEBUG
	%prefix32%gcc -o hooks.dll hooks.c build\hooks.o include\libcomctl32.a -mdll -lshlwapi -lcomctl32 -march=pentium2 -DDEBUG
	
	if "%1" == "x64" (
		%prefix64%gcc -o build\unhook_x64.exe include\unhook.c
		"build\unhook_x64.exe"
		
		%prefix64%windres -o build\hookwindows_x64.o include\hookwindows_x64.rc
		%prefix64%windres -o build\hooks_x64.o include\hooks_x64.rc
		%prefix64%gcc -o HookWindows_x64.exe hookwindows_x64.c build\hookwindows_x64.o -mwindows -lshlwapi -DDEBUG
		%prefix64%gcc -o hooks_x64.dll hooks.c build\hooks_x64.o -mdll -lshlwapi -lcomctl32 -DDEBUG
	)
	
	if "%1" == "run" (
		start AltDrag.exe
	)
)
