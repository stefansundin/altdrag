#!/bin/bash -ex
# sudo apt-get install gcc-mingw-w64 nsis

mkdir -p bin

i686-w64-mingw32-windres -o bin/altdrag.o include/altdrag.rc
i686-w64-mingw32-windres -o bin/hooks.o include/hooks.rc

x86_64-w64-mingw32-windres -o bin/hookwindows_x64.o include/hookwindows_x64.rc
x86_64-w64-mingw32-windres -o bin/hooks_x64.o include/hooks.rc

if [[ "$1" == "release" ]]; then
  i686-w64-mingw32-gcc -Wl,-dynamicbase,-nxcompat -Wp,-D_FORTIFY_SOURCE=2 -o bin/AltDrag.exe altdrag.c bin/altdrag.o -mwindows -lshlwapi -lwininet -lcomctl32 -O3 -s -fstack-protector-strong -fstack-clash-protection -mthreads
  i686-w64-mingw32-gcc -o bin/hooks.dll hooks.c bin/hooks.o -mdll -lshlwapi -lcomctl32 -lpsapi -lole32 -O2 -s

  x86_64-w64-mingw32-gcc -o bin/HookWindows_x64.exe hookwindows_x64.c bin/hookwindows_x64.o -mwindows -lshlwapi -O2 -s
  x86_64-w64-mingw32-gcc -o bin/hooks_x64.dll hooks.c bin/hooks_x64.o -mdll -lshlwapi -lcomctl32 -lpsapi -O2 -s

  makensis -V2 installer.nsi

  (cd bin && sha256sum *.exe *.dll > sha256sum.txt)
else
  i686-w64-mingw32-gcc -o AltDrag.exe altdrag.c bin/altdrag.o -mwindows -lshlwapi -lwininet -lcomctl32 -lole32 -g -DDEBUG
  i686-w64-mingw32-gcc -o hooks.dll hooks.c bin/hooks.o -mdll -lshlwapi -lcomctl32 -lpsapi -lole32 -g -DDEBUG

  x86_64-w64-mingw32-gcc -o HookWindows_x64.exe hookwindows_x64.c bin/hookwindows_x64.o -mwindows -lshlwapi -g -DDEBUG
  x86_64-w64-mingw32-gcc -o hooks_x64.dll hooks.c bin/hooks_x64.o -mdll -lshlwapi -lcomctl32 -lpsapi -lole32 -g -DDEBUG
fi
