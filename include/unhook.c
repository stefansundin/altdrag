/*
	Send a message to all windows, this forces them to unhook invalid hooks.
	Needed to unhook windows hooked to hooks.dll CallWndProc after unclean AltDrag.exe exit.
	If you still can't override hooks.dll, use Process Explorer to search for hooks.dll.
	
	Copyright (C) 2009  Stefan Sundin (recover89@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

HWND *wnds = NULL;
int numwnds = 0;
int maxwnds = 0;

int wnds_alloc = 0;
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	//Make sure we have enough space allocated
	if (numwnds == wnds_alloc) {
		wnds_alloc += 100;
		wnds = realloc(wnds,wnds_alloc*sizeof(HWND));
		if (wnds == NULL) {
			printf("realloc() failed in file %s, line %d.\n", __FILE__, __LINE__);
			return FALSE;
		}
	}
	//Add window to wnds
	wnds[numwnds++] = hwnd;
	return TRUE;
}

int main() {
	EnumWindows(EnumWindowsProc, 0);
	
	char title[100];
	char classname[100];
	int i;
	for (i=0; i < numwnds; i++) {
		/*
		GetWindowText(wnds[i], title, 100);
		GetClassName(wnds[i], classname, 100);
		printf("Sending WM_PAINT to window #%03d: %s [%s]\n", i, title, classname);
		*/
		PostMessage(wnds[i], WM_PAINT, 0, 0);
	}
	
	return 0;
}
