/*
	AltDrag - Drag windows with the mouse when pressing the alt key
	Copyright (C) 2009  Stefan Sundin (recover89@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/


#define UNICODE
#define _UNICODE
#define _WIN32_WINNT 0x0500

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <shlwapi.h>

//App
#define APP_NAME L"AltDrag"
#define STICKY_THRESHOLD 20

//Some variables must be shared so the CallWndProc hooks can access them
#define shareattr __attribute__((section ("shared"), shared))

//States
int alt = 0;
int shift shareattr = 0;
int move shareattr = 0;
int resize shareattr = 0;
HWND hwnd=NULL;
unsigned int clicktime = 0;
POINT offset;
POINT resize_offset;
enum {TOP, RIGHT, BOTTOM, LEFT, CENTER} resize_x, resize_y;

//Sticky
RECT *monitors = NULL;
int nummonitors = 0;
RECT *wnds = NULL;
int numwnds = 0;
HWND progman = NULL;

//Settings
struct {
	int Cursor;
	int AutoStick;
	int RMBMinimize;
} sharedsettings shareattr={0,0,0};
int sharedsettings_loaded shareattr=0;
wchar_t inipath[MAX_PATH] shareattr;
wchar_t txt[1000] shareattr;

//Blacklist
struct blacklistitem {
	wchar_t *title;
	wchar_t *classname;
};
struct blacklist {
	struct blacklistitem *items;
	int numitems;
};
struct {
	struct blacklist Blacklist;
	struct blacklist Blacklist_Sticky;
} settings = {{NULL,0},{NULL,0}};

//Roll-up data
#define NUMROLLUP 8
HWND rollup[NUMROLLUP];
int rolluppos = 0;

//Cursor data
HWND cursorwnd shareattr = NULL;

//Mousehook data
HINSTANCE hinstDLL = NULL;
HHOOK mousehook = NULL;

//Msghook data
WNDPROC oldwndproc = NULL;
enum {MOVE, RESIZE, NONE} msgaction = NONE;

//Error()
#ifdef DEBUG
#include "include/error.h"
#else
#define Error(a,b,c,d,e)
#endif

__cdecl __declspec(dllexport) LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
#include "hooks.h"

__cdecl __declspec(dllexport) LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		int vkey = ((PKBDLLHOOKSTRUCT)lParam)->vkCode;
		
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
			if (!alt && (vkey == VK_LMENU || vkey == VK_RMENU)) {
				alt = 1;
				clicktime = 0; //Reset double-click time
				//Don't hook the mouse if the foreground window is fullscreen
				HWND window = GetForegroundWindow();
				if (!(GetWindowLongPtr(window,GWL_STYLE)&WS_CAPTION)) {
					if (!IsWindow(progman)) {
						progman = FindWindow(L"Progman",L"Program Manager");
					}
					if (window != progman) {
						//Get window size
						RECT wnd;
						if (GetWindowRect(window,&wnd) == 0) {
							//Error(L"GetWindowRect(&wnd)", L"LowLevelKeyboardProc()", GetLastError(), TEXT(__FILE__), __LINE__);
							return CallNextHookEx(NULL, nCode, wParam, lParam);
						}
						/*
						if (nummonitors == 1) {
							//Return if the window is fullscreen
							if (wnd.left == monitors[0].left && wnd.top == monitors[0].top && wnd.right == monitors[0].right && wnd.bottom == monitors[0].bottom) {
								return CallNextHookEx(NULL, nCode, wParam, lParam);
							}
						}
						*/
						//Return if the window is fullscreen
						int i;
						for (i=0; i < nummonitors; i++) {
							if (wnd.left == monitors[i].left && wnd.top == monitors[i].top && wnd.right == monitors[i].right && wnd.bottom == monitors[i].bottom) {
								return CallNextHookEx(NULL, nCode, wParam, lParam);
							}
						}
					}
				}
				//Hook mouse
				HookMouse();
			}
			else if (vkey == VK_LSHIFT || vkey == VK_RSHIFT) {
				if (!shift) {
					shift = 1;
				}
			}
		}
		else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
			if ((vkey == VK_LMENU || vkey == VK_RMENU) && !(GetAsyncKeyState(VK_MENU)&0x8000)) {
				alt = 0;
				if (!move && !resize) {
					UnhookMouse();
				}
			}
			else if (vkey == VK_LSHIFT || vkey == VK_RSHIFT) {
				shift = 0;
			}
		}
	}
	
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

__cdecl __declspec(dllexport) LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		if (alt && (wParam == WM_LBUTTONDOWN || wParam == WM_MBUTTONDOWN || wParam == WM_RBUTTONDOWN)) {
			//Double check if any alt key is being pressed
			if (!(GetAsyncKeyState(VK_MENU)&0x8000)) {
				alt = 0;
				UnhookMouse();
				return CallNextHookEx(NULL, nCode, wParam, lParam);
			}
			
			//Alt key is still being pressed
			POINT pt = ((PMSLLHOOKSTRUCT)lParam)->pt;
			
			//Make sure cursorwnd isn't in the way
			if (sharedsettings.Cursor) {
				ShowWindow(cursorwnd, SW_HIDE);
			}
			
			//Get window
			if ((hwnd=WindowFromPoint(pt)) == NULL) {
				return CallNextHookEx(NULL, nCode, wParam, lParam);
			}
			hwnd = GetAncestor(hwnd,GA_ROOT);
			
			//Return if window is blacklisted
			if (blacklisted(hwnd,&settings.Blacklist)) {
				return CallNextHookEx(NULL, nCode, wParam, lParam);
			}
			
			//Get window size
			RECT wnd;
			if (GetWindowRect(hwnd,&wnd) == 0) {
				Error(L"GetWindowRect(&wnd)", L"LowLevelMouseProc()", GetLastError(), TEXT(__FILE__), __LINE__);
			}
			//Enumerate monitors
			nummonitors = 0;
			EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);
			//Return if the window is fullscreen
			int i;
			if (!(GetWindowLongPtr(hwnd,GWL_STYLE)&WS_CAPTION)) {
				for (i=0; i < nummonitors; i++) {
					if (wnd.left == monitors[i].left && wnd.top == monitors[i].top && wnd.right == monitors[i].right && wnd.bottom == monitors[i].bottom) {
						return CallNextHookEx(NULL, nCode, wParam, lParam);
					}
				}
			}
			
			//Do things depending on what button was pressed
			if (wParam == WM_LBUTTONDOWN) {
				//Maximize window if this is a double-click
				if (GetTickCount()-clicktime <= GetDoubleClickTime()) {
					//Alt+double-clicking a window maximizes it
					//Also roll-down the window if it's in the roll-up database
					for (i=0; i < NUMROLLUP; i++) {
						if (rollup[i] == hwnd) {
							//Remove window from database
							rollup[i] = NULL;
						}
					}
					//Stop move action
					move = 0;
					//Return
					return CallNextHookEx(NULL, nCode, wParam, lParam);
				}
				
				//Restore the window if it's maximized
				if (!IsZoomed(hwnd)) {
					//Remember time of this click so we can check for double-click
					clicktime = GetTickCount();
				}
				//Ready to move window
				move = 1;
			}
			else if (sharedsettings.RMBMinimize && wParam == WM_RBUTTONDOWN) { }
			else {
				//Roll-down the window if it's in the roll-up database
				for (i=0; i < NUMROLLUP; i++) {
					if (rollup[i] == hwnd) {
						//Remove window from database
						rollup[i] = NULL;
						//Return
						return CallNextHookEx(NULL, nCode, wParam, lParam);
					}
				}
				
				//Roll-up window if this is a double-click (or if both middle and right mouse button is pressed)
				if (GetTickCount()-clicktime <= GetDoubleClickTime() || resize) {
					//Alt+middle-double-clicking a window makes it roll-up
					//Store window size
					rollup[rolluppos] = hwnd;
					rolluppos = (rolluppos+1)%NUMROLLUP;
					//Stop resize action
					resize = 1;
					//Return
					return CallNextHookEx(NULL, nCode, wParam, lParam);
				}
				
				//Remember time of this click so we can check for double-click
				clicktime = GetTickCount();
				//Ready to resize window
				resize = 1;
			}
		}
		else if (wParam == WM_LBUTTONUP && move) {
			move = 0;
			if (!alt) {
				UnhookMouse();
			}
		}
		else if (sharedsettings.RMBMinimize && wParam == WM_RBUTTONUP && alt) { }
		else if ((wParam == WM_MBUTTONUP || wParam == WM_RBUTTONUP) && (alt || resize)) {
			resize = 0;
			if (!alt) {
				UnhookMouse();
			}
		}
		else if (wParam == WM_MOUSEMOVE) {
			//Reset double-click time
			clicktime = 0;
		}
	}
	
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		hinstDLL = hInstance;
		//Load settings
		//Settings shared with CallWndProc hook
		if (!sharedsettings_loaded) {
			sharedsettings_loaded = 1;
			//Have to store path to ini file at initial load so CallWndProc hooks can find it
			GetModuleFileName(NULL, inipath, sizeof(inipath)/sizeof(wchar_t));
			PathRemoveFileSpec(inipath);
			wcscat(inipath, L"\\AltDrag.ini");
			//Cursor
			GetPrivateProfileString(APP_NAME, L"Cursor", L"0", txt, sizeof(txt)/sizeof(wchar_t), inipath);
			swscanf(txt, L"%d", &sharedsettings.Cursor);
			if (sharedsettings.Cursor) {
				cursorwnd=FindWindow(APP_NAME,NULL);
			}
			//AutoStick
			GetPrivateProfileString(APP_NAME, L"AutoStick", L"0", txt, sizeof(txt)/sizeof(wchar_t), inipath);
			swscanf(txt, L"%d", &sharedsettings.AutoStick);
			//RMBMinimize
			GetPrivateProfileString(APP_NAME, L"RMBMinimize", L"0", txt, sizeof(txt)/sizeof(wchar_t), inipath);
			swscanf(txt, L"%d", &sharedsettings.RMBMinimize);
			//Zero-out roll-up hwnds
			int i;
			for (i=0; i < NUMROLLUP; i++) {
				rollup[i] = NULL;
			}
		}
		//Blacklist
		GetPrivateProfileString(APP_NAME, L"Blacklist", L"", txt, sizeof(txt)/sizeof(wchar_t), inipath);
		int blacklist_alloc = 0;
		wchar_t *pos = txt;
		struct blacklist *add_blacklist = &settings.Blacklist;
		while (pos != NULL) {
			wchar_t *title = pos;
			wchar_t *classname = wcsstr(pos,L"|");
			//Move pos to next item (if any)
			pos = wcsstr(pos,L",");
			if (pos != NULL) {
				*pos = '\0';
				pos++;
			}
			if (classname != NULL) {
				*classname = '\0';
				classname++;
			}
			//Allocate memory and copy over text
			wchar_t *item_title, *item_classname;
			if (!wcscmp(title,L"*")) {
				item_title = NULL;
			}
			else {
				item_title = malloc((wcslen(title)+1)*sizeof(wchar_t));
				wcscpy(item_title, title);
			}
			if (classname == NULL) {
				item_classname = NULL;
			}
			else {
				if (!wcscmp(classname,L"*")) {
					item_classname = NULL;
				}
				else {
					item_classname = malloc((wcslen(classname)+1)*sizeof(wchar_t));
					wcscpy(item_classname, classname);
				}
			}
			//Store item if it's not empty
			if (item_title != NULL || item_classname != NULL) {
				//Make sure we have enough space
				if (add_blacklist->numitems == blacklist_alloc) {
					blacklist_alloc += 10;
					add_blacklist->items = realloc(add_blacklist->items,blacklist_alloc*sizeof(struct blacklistitem));
				}
				//Store item
				add_blacklist->items[add_blacklist->numitems].title = item_title;
				add_blacklist->items[add_blacklist->numitems].classname = item_classname;
				add_blacklist->numitems++;
			}
			//Switch gears to Blacklist_Sticky?
			if (pos == NULL && add_blacklist == &settings.Blacklist) {
				add_blacklist = &settings.Blacklist_Sticky;
				blacklist_alloc = 0;
				GetPrivateProfileString(APP_NAME, L"Blacklist_Sticky", L"", txt, sizeof(txt)/sizeof(wchar_t), inipath);
				pos = txt;
			}
		}
		//Allocate space for wnds
		wnds_alloc += 5;
		wnds = realloc(wnds,wnds_alloc*sizeof(RECT));
		if (wnds == NULL) {
			Error(L"realloc(wnds)", L"Out of memory?", 0, TEXT(__FILE__), __LINE__);
			return FALSE;
		}
	}
	else if (reason == DLL_PROCESS_DETACH) {
		//Restore old WndProc
		if (oldwndproc != NULL && IsWindow(hwnd)) {
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)oldwndproc);
		}
		//Free memory
		int i;
		for (i=0; i < settings.Blacklist.numitems; i++) {
			free(settings.Blacklist.items[i].title);
			free(settings.Blacklist.items[i].classname);
		}
		settings.Blacklist.numitems = 0;
		free(settings.Blacklist.items);
		for (i=0; i < settings.Blacklist_Sticky.numitems; i++) {
			free(settings.Blacklist_Sticky.items[i].title);
			free(settings.Blacklist_Sticky.items[i].classname);
		}
		settings.Blacklist_Sticky.numitems = 0;
		free(settings.Blacklist_Sticky.items);
		free(wnds);
	}
	return TRUE;
}
