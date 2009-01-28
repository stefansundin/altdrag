@echo off

taskkill /IM AltDrag.exe

if not exist build (
	mkdir build
)

gcc -o build/unhook.exe unhook.c
"build/unhook.exe"

windres -o build/resources.o resources.rc

if "%1" == "all" (
	@echo.
	echo Building binaries
	if not exist "build/en-US/AltDrag" (
		mkdir "build\en-US\AltDrag"
	)
	gcc -o "build/en-US/AltDrag/AltDrag.exe" altdrag.c build/resources.o -mwindows -lshlwapi -lwininet
	if exist "build/en-US/AltDrag/AltDrag.exe" (
		strip "build/en-US/AltDrag/AltDrag.exe"
	)
	gcc -c -o "build/hooks.o" hooks.c
	if exist "build/hooks.o" (
		gcc -shared -o "build/en-US/AltDrag/hooks.dll" "build/hooks.o" -lshlwapi
		strip "build/en-US/AltDrag/hooks.dll"
	)
	
	for /D %%f in (localization/*) do (
		@echo.
		echo Putting together %%f
		if not %%f == en-US (
			if not exist "build/%%f/AltDrag" (
				mkdir "build\%%f\AltDrag"
			)
			copy "build\en-US\AltDrag\AltDrag.exe" "build/%%f/AltDrag"
			copy "build\en-US\AltDrag\hooks.dll" "build/%%f/AltDrag"
		)
		copy "localization\%%f\info.txt" "build/%%f/AltDrag"
		copy "AltDrag.ini" "build/%%f/AltDrag"
	)
	
	@echo.
	echo Building installer
	makensis /V2 installer.nsi
) else (
	gcc -o AltDrag.exe altdrag.c build/resources.o -mwindows -lshlwapi -lwininet -DDEBUG
	gcc -c -o "build/hooks.o" hooks.c -DDEBUG
	gcc -shared -o "hooks.dll" "build/hooks.o" -lshlwapi
	
	if "%1" == "run" (
		start AltDrag.exe
	)
	if "%1" == "hide" (
		start AltDrag.exe -hide
	)
)
