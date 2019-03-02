#!/usr/bin/env bash

# Note 2014-08-06:
# It looks like recent mingw-w64 require -lole32 to be used on AltDrag.exe and hooks_x64.dll, which is probably a bug.
# TODO: Revisit this issue later.

# For traditional MinGW, set prefix32 to empty string
# For mingw-w32, set prefix32 to i686-w64-mingw32-
# For mingw-w64, set prefix64 to x86_64-w64-mingw32-

prefix32="i686-w64-mingw32-"
prefix64="x86_64-w64-mingw32-"

if [[ $1 == "x64" ]] || [[ $2 == "x64" ]]; then
	x64=1
else
	x64=0
fi

taskkill.exe /IM AltDrag.exe

if [ ! -d "bin" ]; then
	mkdir bin
fi

# "$prefix32""gcc -o localization/export_l10n_ini.exe tools/export_l10n_ini.c -lshlwapi"
# ./localization/export_l10n_ini.exe
# exit 0

# "$prefix32""gcc -o bin/unhook.exe tools/unhook.c"
# ./bin/unhook.exe

eval "$prefix32""windres include/altdrag.rc bin/altdrag.o"
eval "$prefix32""windres include/hooks.rc bin/hooks.o"

if [[ $1 == "all" ]]; then
	echo
	echo "Building release build"
	eval "$prefix32""gcc -o bin/AltDrag.exe altdrag.c bin/altdrag.o -mwindows -lshlwapi -lwininet -lcomctl32 -O2 -s"
	if [[ ! -f "bin/AltDrag.exe" ]]; then
		exit 1
	fi
	eval "$prefix32""gcc -o bin/hooks.dll hooks.c bin/hooks.o -mdll -lshlwapi -lcomctl32 -lpsapi -lole32 -O2 -s"
	if [[ ! -f "bin/hooks.dll" ]]; then
		exit 1
	fi

	if [[ $x64 == "1" ]]; then
		eval "$prefix64""windres include/hookwindows_x64.rc bin/hookwindows_x64.o"
		eval "$prefix64""windres include/hooks.rc bin/hooks_x64.o"
		eval "$prefix64""gcc -o bin/HookWindows_x64.exe hookwindows_x64.c bin/hookwindows_x64.o -mwindows -lshlwapi -O2 -s"
		if [[ ! -f "bin/HookWindows_x64.exe" ]]; then
			exit 1
		fi
		eval "$prefix64""gcc -o bin/hooks_x64.dll hooks.c bin/hooks_x64.o -mdll -lshlwapi -lcomctl32 -lpsapi -O2 -s"
		if [[ ! -f "bin/hooks_x64.dll" ]]; then
			exit 1
		fi
	fi

	echo "Building installer"
	makensis -V2 installer.nsi # Requires nsis

	cd bin && sha256sum *.exe *.dll > sha256sum.txt
elif [[ $1 == "lang" ]]; then
	eval "$prefix32""gcc -o localization/import_languages.exe tools/import_languages.c -lshlwapi"
	./localization/import_languages.exe
else
	eval "$prefix32""gcc -o AltDrag.exe altdrag.c bin/altdrag.o -mwindows -lshlwapi -lwininet -lcomctl32 -lole32 -g -DDEBUG"
	eval "$prefix32""gcc -o hooks.dll hooks.c bin/hooks.o -mdll -lshlwapi -lcomctl32 -lpsapi -lole32 -g -DDEBUG"

	if [[ $x64 == 0 ]]; then
		if [[ -f "hooks_x64.dll" ]]; then
			echo "Found hooks_x64.dll, building x64 even though you didn't ask for it..."
			x64=1
		fi
	fi
	if [[ $x64 == 1 ]]; then
		# "$prefix64""gcc -o bin/unhook_x64.exe tools/unhook.c"
		# "bin/unhook_x64.exe"

		eval "$prefix64""windres include/hookwindows_x64.rc bin/hookwindows_x64.o"
		eval "$prefix64""windres include/hooks.rc bin/hooks_x64.o"
		eval "$prefix64""gcc -o HookWindows_x64.exe hookwindows_x64.c bin/hookwindows_x64.o -mwindows -lshlwapi -g -DDEBUG"
		eval "$prefix64""gcc -o hooks_x64.dll hooks.c bin/hooks_x64.o -mdll -lshlwapi -lcomctl32 -lpsapi -lole32 -g -DDEBUG"
	fi

	if [[ $1 == "run" ]]; then
		cmd.exe /C start AltDrag.exe
	fi
fi
