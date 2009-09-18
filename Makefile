#use make -i

debug: pre
	gcc -o HookWindows_x64.exe hookwindows_x64.c build/hookwindows_x64.o -mwindows -lshlwapi -DDEBUG
	gcc -o hooks_x64.dll hooks_x64.c build/hooks_x64.o -mdll -lshlwapi -DDEBUG

all: pre
	gcc -o build/en-US/AltDrag/HookWindows_x64.exe hookwindows_x64.c build/hookwindows_x64.o -mwindows -lshlwapi -O2
	gcc -o build/en-US/AltDrag/hooks_x64.dll hooks_x64.c build/hooks_x64.o -mdll -lshlwapi -O2

pre:
	taskkill -IM HookWindows_x64.exe
	build/unhook.exe
	windres -o build/hookwindows_x64.o include/hookwindows_x64.rc
	windres -o build/hooks_x64.o include/hooks_x64.rc
