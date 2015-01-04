@echo off

:: Note 2014-08-06:
:: It looks like recent mingw-w64 require -lole32 to be used on AltDrag.exe and hooks_x64.dll, which is probably a bug.
:: TODO: Revisit this issue later.

:: For traditional MinGW, set prefix32 to empty string
:: For mingw-w32, set prefix32 to i686-w64-mingw32-
:: For mingw-w64, set prefix64 to x86_64-w64-mingw32-

set prefix32=i686-w64-mingw32-
set prefix64=x86_64-w64-mingw32-

set x64=0
if "%1" == "x64" set x64=1
if "%2" == "x64" set x64=1

taskkill /IM AltDrag.exe

if not exist build. mkdir build

:: %prefix32%gcc -o localization/export_l10n_ini.exe tools/export_l10n_ini.c -lshlwapi
:: start localization/export_l10n_ini.exe
:: exit /b

:: %prefix32%gcc -o build/unhook.exe tools/unhook.c
:: "build/unhook.exe"

%prefix32%windres include/altdrag.rc build/altdrag.o
%prefix32%windres include/hooks.rc build/hooks.o

if "%1" == "all" (
  echo.
  echo Building release build
  %prefix32%gcc -o build/AltDrag.exe altdrag.c build/altdrag.o -mwindows -lshlwapi -lwininet -lcomctl32 -O2 -s
  if not exist build/AltDrag.exe. exit /b
  %prefix32%gcc -o build/hooks.dll hooks.c build/hooks.o -mdll -lshlwapi -lcomctl32 -lpsapi -lole32 -O2 -s
  if not exist build/hooks.dll. exit /b

  if "%x64%" == "1" (
    %prefix64%windres include/hookwindows_x64.rc build/hookwindows_x64.o
    %prefix64%windres include/hooks.rc build/hooks_x64.o
    %prefix64%gcc -o build/HookWindows_x64.exe hookwindows_x64.c build/hookwindows_x64.o -mwindows -lshlwapi -O2 -s
    if not exist build/HookWindows_x64.exe. exit /b
    %prefix64%gcc -o build/hooks_x64.dll hooks.c build/hooks_x64.o -mdll -lshlwapi -lcomctl32 -lpsapi -O2 -s
    if not exist build/hooks_x64.dll. exit /b
  )

  echo Building installer
  makensis /V2 installer.nsi
) else if "%1" == "lang" (
  %prefix32%gcc -o localization/import_languages.exe tools/import_languages.c -lshlwapi
  start localization/import_languages.exe
) else (
  %prefix32%gcc -o AltDrag.exe altdrag.c build/altdrag.o -mwindows -lshlwapi -lwininet -lcomctl32 -lole32 -g -DDEBUG
  %prefix32%gcc -o hooks.dll hooks.c build/hooks.o -mdll -lshlwapi -lcomctl32 -lpsapi -lole32 -g -DDEBUG

  if "%x64%" == "0" (
    if exist hooks_x64.dll (
      echo Found hooks_x64.dll, building x64 even though you didn't ask for it...
      set x64=1
    )
  )
  if "%x64%" == "1" (
    rem %prefix64%gcc -o build/unhook_x64.exe tools/unhook.c
    rem "build/unhook_x64.exe"

    %prefix64%windres include/hookwindows_x64.rc build/hookwindows_x64.o
    %prefix64%windres include/hooks.rc build/hooks_x64.o
    %prefix64%gcc -o HookWindows_x64.exe hookwindows_x64.c build/hookwindows_x64.o -mwindows -lshlwapi -g -DDEBUG
    %prefix64%gcc -o hooks_x64.dll hooks.c build/hooks_x64.o -mdll -lshlwapi -lcomctl32 -lpsapi -lole32 -g -DDEBUG
  )

  if "%1" == "run" (
    start AltDrag.exe
  )
)
