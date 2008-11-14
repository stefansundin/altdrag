/*
	Send a message to all windows, this forces them to unhook invalid hooks.
	Needed to unhook windows hooked to hooks.dll CallWndProc after unclean AltDrag.exe exit.
	
	Copyright (C) 2008  Stefan Sundin (recover89@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

static HWND *wnds=NULL;
static int numwnds=0;
static int maxwnds=0;

static char txt[100];

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	if (numwnds == maxwnds) {
		if ((wnds=realloc(wnds,(maxwnds+100)*sizeof(HWND))) == NULL) {
			printf(txt,"realloc() failed in file %s, line %d.",__FILE__,__LINE__);
			return FALSE;
		}
		maxwnds+=100;
	}
	wnds[numwnds++]=hwnd;
	return TRUE;
}

int main() {
	EnumWindows(EnumWindowsProc,0);
	
	char title[100];
	char classname[100];
	int i;
	for (i=0; i < numwnds; i++) {
		SendMessage(wnds[i],WM_PAINT,0,0);
		GetWindowText(wnds[i],title,100);
		GetClassName(wnds[i],classname,100);
		//printf("Sending WM_PAINT to window #%03d: %s [%s]\n",i,title,classname);
	}
	
	return 0;
}
