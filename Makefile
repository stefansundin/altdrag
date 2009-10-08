L10N = es-ES gl-ES ko-KR

debug: pre
	gcc -o HookWindows_x64.exe hookwindows_x64.c build/hookwindows_x64.o -mwindows -lshlwapi -DDEBUG
	gcc -o hooks_x64.dll hooks.c build/hooks_x64.o -mdll -lshlwapi -lcomctl32 -DDEBUG

all: pre
	@echo Building binaries
	gcc -o build/ini.exe include/ini.c -lshlwapi
	
	mkdir -p build/en-US/AltDrag
	gcc -o build/en-US/AltDrag/HookWindows_x64.exe hookwindows_x64.c build/hookwindows_x64.o -mwindows -lshlwapi -O2 -s
	gcc -o build/en-US/AltDrag/hooks_x64.dll hooks.c build/hooks_x64.o -mdll -lshlwapi -lcomctl32 -O2 -s
	@for lang in ${L10N} ;\
	do \
		echo Putting together $$lang ;\
		mkdir -p build/$$lang/AltDrag ;\
		cp build/en-US/AltDrag/HookWindows_x64.exe build/$$lang/AltDrag ;\
		cp build/en-US/AltDrag/hooks_x64.dll build/$$lang/AltDrag ;\
	done

pre:
	-taskkill -IM HookWindows_x64.exe
	-mkdir build
	gcc -o build/unhook_x64.exe include/unhook.c
	build/unhook_x64.exe
	windres -o build/hookwindows_x64.o include/hookwindows_x64.rc
	windres -o build/hooks_x64.o include/hooks_x64.rc
