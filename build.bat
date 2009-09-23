@echo off

taskkill /IM AltDrag.exe

if not exist build (
	mkdir build
)

gcc -o build\unhook.exe include\unhook.c -march=pentium2
"build\unhook.exe"

windres -o build\altdrag.o include\altdrag.rc
windres -o build\hooks.o include\hooks.rc

if "%1" == "all" (
	gcc -o build\ini.exe include\ini.c -lshlwapi -march=pentium2
	
	@echo.
	echo Building binaries
	if not exist "build\en-US\AltDrag" (
		mkdir "build\en-US\AltDrag"
	)
	gcc -o "build\en-US\AltDrag\AltDrag.exe" altdrag.c build\altdrag.o -mwindows -lshlwapi -lwininet -march=pentium2 -O2
	if not exist "build\en-US\AltDrag\AltDrag.exe" (
		exit /b
	)
	strip "build\en-US\AltDrag\AltDrag.exe"
	gcc -o "build\en-US\AltDrag\hooks.dll" hooks.c build\hooks.o -mdll -lshlwapi -lcomctl32 -march=pentium2 -O2
	if not exist "build\en-US\AltDrag\hooks.dll" (
		exit /b
	)
	strip "build\en-US\AltDrag\hooks.dll"
	
	for /D %%f in (localization/*) do (
		if not exist "localization\%%f\outdated.txt" (
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
	)
	
	@echo.
	echo Building installer
	makensis /V2 installer.nsi
) else (
	gcc -o AltDrag.exe altdrag.c build\altdrag.o -mwindows -lshlwapi -lwininet -march=pentium2 -DDEBUG
	gcc -o hooks.dll hooks.c build\hooks.o -mdll -lshlwapi -lcomctl32 -march=pentium2 -DDEBUG
	
	if "%1" == "run" (
		start AltDrag.exe
	)
	if "%1" == "hide" (
		start AltDrag.exe -hide
	)
)
