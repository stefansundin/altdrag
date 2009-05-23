@echo off

taskkill /IM AltDrag.exe

if not exist build (
	mkdir build
)

gcc -o build\unhook.exe unhook.c
"build\unhook.exe"

windres -o build\resources.o resources.rc
windres -o build\resources_hooks.o hooks.rc

if "%1" == "all" (
	gcc -o build\ini.exe ini.c -lshlwapi
	
	@echo.
	echo Building binaries
	if not exist "build\en-US\AltDrag" (
		mkdir "build\en-US\AltDrag"
	)
	gcc -o "build\en-US\AltDrag\AltDrag.exe" altdrag.c build\resources.o -mwindows -lshlwapi -lwininet
	if not exist "build\en-US\AltDrag\AltDrag.exe" (
		exit /b
	)
	strip "build\en-US\AltDrag\AltDrag.exe"
	gcc -c -o "build\hooks.o" hooks.c
	if not exist "build\hooks.o" (
		exit /b
	)
	gcc -shared -o "build\en-US\AltDrag\hooks.dll" "build\hooks.o" "build\resources_hooks.o" -lshlwapi
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
		copy "AltDrag.ini" "build\%%f\AltDrag"
		"build\ini.exe" "build\%%f\AltDrag\AltDrag.ini" AltDrag Language %%f
	)
	
	@echo.
	echo Building installer
	makensis /V2 installer.nsi
) else (
	gcc -o AltDrag.exe altdrag.c build\resources.o -mwindows -lshlwapi -lwininet -DDEBUG
	gcc -c -o "build\hooks.o" hooks.c -DDEBUG
	gcc -shared -o "hooks.dll" "build\hooks.o" "build\resources_hooks.o" -lshlwapi
	
	if "%1" == "run" (
		start AltDrag.exe
	)
	if "%1" == "hide" (
		start AltDrag.exe -hide
	)
)
