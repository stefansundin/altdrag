
taskkill /IM AltDrag.exe

gcc -c hooks.c
gcc -shared -o hooks.dll hooks.o

windres -o resources.o resources.rc
gcc -o AltDrag altdrag.c resources.o -mwindows

strip AltDrag.exe
strip hooks.dll

upx --best -qq AltDrag.exe
upx --best -qq hooks.dll
