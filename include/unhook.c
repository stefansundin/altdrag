/*
	Send a message to all windows, this forces them to unhook invalid hooks.
	Needed to unhook windows hooked to hooks.dll CallWndProc after unclean AltDrag.exe exit.
	If you still can't override hooks.dll, use Process Explorer to search for hooks.dll.
	This also applies to hooks_x64.dll.
	
	Copyright (C) 2010  Stefan Sundin (recover89@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	/*
	char title[100];
	char classname[100];
	GetWindowText(hwnd, title, 100);
	GetClassName(hwnd, classname, 100);
	printf("Sending WM_NULL to window: %s [%s]\n", title, classname);
	*/
	PostMessage(hwnd, WM_NULL, 0, 0);
	
	return TRUE;
}

int main() {
	EnumWindows(EnumWindowsProc, 0);
	
	return 0;
}
