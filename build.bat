@echo off

taskkill /IM WindowFinder.exe

if not exist build (
	mkdir build
)

windres -o build\resources.o include\resources.rc

gcc -o WindowFinder.exe windowfinder.c build\resources.o -mwindows -lshlwapi -lwininet

if "%1" == "run" (
	start WindowFinder.exe
)
