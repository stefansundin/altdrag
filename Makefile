#use make -i

debug: pre
	gcc -O2 -o HookWindows_x64.exe hookwindows_x64.c build/hookwindows_x64.o -mwindows -lshlwapi -DDEBUG
	gcc -O2 -o hooks_x64.dll hooks_x64.c build/hooks_x64.o -mdll -lshlwapi -DDEBUG

all: pre
	cp HookWindows_x64.exe build/en-US/AltDrag/HookWindows_x64.exe
	cp hooks_x64.dll build/en-US/AltDrag/hooks_x64.dll

pre:
	taskkill -IM HookWindows_x64.exe
	build/unhook.exe
	windres -o build/hookwindows_x64.o include/hookwindows_x64.rc
	windres -o build/hooks_x64.o include/hooks_x64.rc
