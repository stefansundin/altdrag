#use make -i

x64:
	taskkill -IM HookWindows_x64.exe
	build/unhook.exe
	windres -o build/hookwindows_x64.o include/hookwindows_x64.rc
	windres -o build/hooks_x64.o include/hooks_x64.rc
	gcc -o HookWindows_x64.exe hookwindows_x64.c build/hookwindows_x64.o -mwindows -lshlwapi -DDEBUG
	gcc -o hooks_x64.dll hooks_x64.c build/hooks_x64.o -mdll -lshlwapi -DDEBUG
