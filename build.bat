@echo off

taskkill /IM AltDrag.exe

if not exist build (
	mkdir build
)

gcc -o build/unhook.exe unhook.c
"build/unhook.exe"

windres -o build/resources.o resources.rc

if "%1" == "all" (
	echo Building all
	
	for /D %%f in (localization/*) do (
		@echo.
		echo Building %%f
		if not exist "build/%%f/AltDrag" (
			mkdir "build\%%f\AltDrag"
		)
		copy "localization\%%f\info.txt" "build/%%f/AltDrag/info.txt"
		
		gcc -o "build/%%f/AltDrag/AltDrag.exe" altdrag.c build/resources.o -mwindows -lshlwapi -DL10N_FILE=\"localization/%%f/strings.h\"
		if exist "build/%%f/AltDrag/AltDrag.exe" (
			strip "build/%%f/AltDrag/AltDrag.exe"
			upx --best -qq "build/%%f/AltDrag/AltDrag.exe"
		)
						
		gcc -c -o "build/%%f/hooks.o" hooks.c
		if exist "build/%%f/AltDrag/hooks.o" (
			gcc -shared -o "build/%%f/AltDrag/hooks.dll" "build/%%f/hooks.o"
			strip "build/%%f/AltDrag/hooks.dll"
			rem upx --best -qq "build/%%f/AltDrag/hooks.dll"
		)
	)
) else (
	gcc -o AltDrag.exe altdrag.c build/resources.o -mwindows -lshlwapi
	gcc -c -o "build/hooks.o" hooks.c
	gcc -shared -o "hooks.dll" "build/hooks.o"
	
	if "%1" == "run" (
		start AltDrag.exe
	)
)
