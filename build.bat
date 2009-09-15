@echo off

taskkill /IM AltDrag.exe

if not exist build (
	mkdir build
)

gcc -march=pentium2 -mtune=pentium2 -o build\unhook.exe include\unhook.c
"build\unhook.exe"

windres -o build\resources.o include\resources.rc
windres -o build\resources_hooks.o include\hooks.rc

if "%1" == "all" (
	gcc -march=pentium2 -mtune=pentium2 -o build\ini.exe include\ini.c -lshlwapi
	
	@echo.
	echo Building binaries
	if not exist "build\en-US\AltDrag" (
		mkdir "build\en-US\AltDrag"
	)
	gcc -O2 -march=pentium2 -mtune=pentium2 -o "build\en-US\AltDrag\AltDrag.exe" altdrag.c build\resources.o -mwindows -lshlwapi -lwininet
	if not exist "build\en-US\AltDrag\AltDrag.exe" (
		exit /b
	)
	strip "build\en-US\AltDrag\AltDrag.exe"
	gcc -O2 -march=pentium2 -mtune=pentium2 -o "build\en-US\AltDrag\hooks.dll" hooks.c build\resources_hooks.o -mdll -lshlwapi
	if not exist "build\en-US\AltDrag\hooks.dll" (
		exit /b
	)
	strip "build\en-US\AltDrag\hooks.dll"
	
	for /D %%f in (localization/*) do (
		@echo.
		echo Putting together %%f
		if not %%f == en-US (
			if not exist "build\%%f\AltDrag" (
				mkdir "build\%%f\AltDrag"
			)
			copy "build\en-US\AltDrag\AltDrag.exe" "build\%%f\AltDrag"
			copy "build\en-US\AltDrag\hooks.dll" "build\%%f\AltDrag"
		)
		copy "localization\%%f\info.txt" "build\%%f\AltDrag"
		copy AltDrag.ini "build\%%f\AltDrag"
		"build\ini.exe" "build\%%f\AltDrag\AltDrag.ini" AltDrag Language %%f
	)
	
	@echo.
	echo Building installer
	makensis /V2 installer.nsi
) else (
	gcc -march=pentium2 -mtune=pentium2 -o AltDrag.exe altdrag.c build\resources.o -mwindows -lshlwapi -lwininet -DDEBUG
	gcc -march=pentium2 -mtune=pentium2 -o hooks.dll hooks.c build\resources_hooks.o -mdll -lshlwapi -DDEBUG
	
	if "%1" == "run" (
		start AltDrag.exe
	)
	if "%1" == "hide" (
		start AltDrag.exe -hide
	)
)
