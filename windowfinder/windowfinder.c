/*
	Copyright (C) 2011  Stefan Sundin (recover89@gmail.com)

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

#define UNICODE
#define _UNICODE
#define _WIN32_WINNT 0x0500
#define _WIN32_IE 0x0600

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <shlwapi.h>
#include <wchar.h>

//App
#define APP_NAME      L"WindowFinder"
#define APP_VERSION   "0.2"
#define APP_URL       L"http://code.google.com/p/altdrag/wiki/WindowFinder"
//#define DEBUG

//Messages
#define WM_TRAY                WM_USER+1
#define SWM_FIND               WM_APP+1
#define SWM_FINDDELAY          WM_APP+2
#define SWM_FINDALL            WM_APP+3
#define SWM_UPDATE             WM_APP+4
#define SWM_ABOUT              WM_APP+5
#define SWM_EXIT               WM_APP+6

//Balloon stuff missing in MinGW
#ifndef NIIF_USER
#define NIIF_USER 4
#define NIN_BALLOONSHOW        WM_USER+2
#define NIN_BALLOONHIDE        WM_USER+3
#define NIN_BALLOONTIMEOUT     WM_USER+4
#define NIN_BALLOONUSERCLICK   WM_USER+5
#endif

//Boring stuff
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hinst = NULL;
HWND g_hwnd = NULL;
UINT WM_TASKBARCREATED = 0;
wchar_t txt[2000];

//Cool stuff
HHOOK mousehook = NULL;
HWND *wnds = NULL;
int numwnds = 0;
int find = 0;

//Include stuff
#include "localization/strings.h"
#include "../include/error.c"
#include "include/tray.c"

//Entry point
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
	g_hinst = hInst;

	//Look for previous instance
	HWND previnst = FindWindow(APP_NAME, NULL);
	if (previnst != NULL) {
		SendMessage(previnst, WM_COMMAND, SWM_FIND, 0);
		return 0;
	}

	//Create window
	WNDCLASSEX wnd = {sizeof(WNDCLASSEX), 0, WindowProc, 0, 0, hInst, NULL, NULL, (HBRUSH)(COLOR_WINDOW+1), NULL, APP_NAME, NULL};
	wnd.hCursor = LoadImage(hInst, L"find", IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR);
	RegisterClassEx(&wnd);
	g_hwnd = CreateWindowEx(WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_LAYERED, wnd.lpszClassName, APP_NAME, WS_POPUP, 0, 0, 0, 0, NULL, NULL, hInst, NULL);
	SetLayeredWindowAttributes(g_hwnd, 0, 1, LWA_ALPHA); //Almost transparent

	//Tray icon
	InitTray();
	UpdateTray();

	//Hook mouse
	if ((GetAsyncKeyState(VK_SHIFT)&0x8000)) {
		HookMouse();
	}

	//Message loop
	MSG msg;
	while (GetMessage(&msg,NULL,0,0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndDetailsMsgProc(INT nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HCBT_ACTIVATE) {
		//Edit the caption of the buttons
		SetDlgItemText((HWND)wParam, IDYES, L"Copy");
		SetDlgItemText((HWND)wParam, IDNO,  L"OK");
	}
	return 0;
}

DWORD WINAPI FindWnd(LPVOID arg) {
	POINT pt = *(POINT*)arg;
	free(arg);

	HWND hwnd_component = WindowFromPoint(pt);
	if (hwnd_component == NULL) {
		#ifdef DEBUG
		Error(L"WindowFromPoint()", L"FindWnd()", GetLastError());
		#endif
	}
	HWND hwnd = GetAncestor(hwnd_component, GA_ROOT);

	//Get child window
	RECT wnd;
	if (GetWindowRect(hwnd,&wnd) == 0) {
		#ifdef DEBUG
		Error(L"GetWindowRect()", L"FindWnd()", GetLastError());
		#endif
	}
	POINT pt_child;
	pt_child.x = pt.x-wnd.left;
	pt_child.y = pt.y-wnd.top;
	HWND hwnd_child = ChildWindowFromPoint(hwnd, pt_child);
	if (hwnd_child == NULL) {
		#ifdef DEBUG
		Error(L"ChildWindowFromPoint()", L"FindWnd()", GetLastError());
		#endif
	}

	//Get title and class
	wchar_t title[256], title_child[256], title_component[256];
	wchar_t classname[256], classname_child[256], classname_component[256];
	GetWindowText(hwnd, title, sizeof(title)/sizeof(wchar_t));
	GetClassName(hwnd, classname, sizeof(classname)/sizeof(wchar_t));
	GetWindowText(hwnd_child, title_child, sizeof(title_child)/sizeof(wchar_t));
	GetClassName(hwnd_child, classname_child, sizeof(classname_child)/sizeof(wchar_t));
	GetWindowText(hwnd_component, title_component, sizeof(title_component)/sizeof(wchar_t));
	GetClassName(hwnd_component, classname_component, sizeof(classname_component)/sizeof(wchar_t));

	//Assemble message
	swprintf(txt, L"Window:\n title: %s\n class: %s", title, classname);
	if (hwnd_child != hwnd) {
		swprintf(txt, L"%s\n\nChild:\n title: %s\n class: %s", txt, title_child, classname_child);
	}
	if (hwnd_component != hwnd_child && hwnd_component != hwnd) {
		swprintf(txt, L"%s\n\nComponent:\n title: %s\n class: %s", txt, title_component, classname_component);
	}
	swprintf(txt, L"%s\n\nExample blacklist rules:\n%s|%s\n*|%s", txt, title, classname, classname);
	//Show message
	HHOOK hhk = SetWindowsHookEx(WH_CBT, &WndDetailsMsgProc, 0, GetCurrentThreadId());
	int response = MessageBox(NULL, txt, l10n->wnddetails, MB_ICONINFORMATION|MB_YESNO|MB_DEFBUTTON2|MB_SYSTEMMODAL);
	UnhookWindowsHookEx(hhk);
	if (response == IDYES) {
		//Copy message to clipboard
		OpenClipboard(NULL);
		EmptyClipboard();
		wchar_t *data = LocalAlloc(LMEM_FIXED, (wcslen(txt)+1)*sizeof(wchar_t));
		memcpy(data, txt, (wcslen(txt)+1)*sizeof(wchar_t));
		SetClipboardData(CF_UNICODETEXT, data);
		CloseClipboard();
	}
}

int wnds_alloc = 0;
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	//Make sure we have enough space allocated
	if (numwnds == wnds_alloc) {
		wnds_alloc += 100;
		wnds = realloc(wnds, wnds_alloc*sizeof(HWND));
	}
	//Store window if it's visible
	if (IsWindowVisible(hwnd)) {
		wnds[numwnds++] = hwnd;
	}
	return TRUE;
}

void FindAllWnds() {
	//Enumerate windows
	numwnds = 0;
	EnumWindows(EnumWindowsProc, 0);
	wchar_t title[256];
	wchar_t classname[256];
	int i;
	swprintf(txt, L"");
	for (i=0; i < numwnds; i++) {
		GetWindowText(wnds[i], title, sizeof(title)/sizeof(wchar_t));
		GetClassName(wnds[i], classname, sizeof(classname)/sizeof(wchar_t));
		swprintf(txt, L"%s%s|%s", txt, title, classname);
		//swprintf_s(txt, sizeof(txt)/sizeof(wchar_t), L"%s%s|%s", txt, title, classname);
		if (i+1 < numwnds) {
			wcscat(txt, L"\n");
		}
	}

	//Show message
	HHOOK hhk = SetWindowsHookEx(WH_CBT, &WndDetailsMsgProc, 0, GetCurrentThreadId());
	int response = MessageBox(NULL, txt, l10n->allwnds, MB_ICONINFORMATION|MB_YESNO|MB_DEFBUTTON2);
	UnhookWindowsHookEx(hhk);
	if (response == IDYES) {
		//Copy message to clipboard
		OpenClipboard(NULL);
		EmptyClipboard();
		wchar_t *data = LocalAlloc(LMEM_FIXED, (wcslen(txt)+1)*sizeof(wchar_t));
		memcpy(data, txt, (wcslen(txt)+1)*sizeof(wchar_t));
		SetClipboardData(CF_UNICODETEXT, data);
		CloseClipboard();
	}
}

//Hooks
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		if (wParam == WM_LBUTTONDOWN && find) {
			POINT pt = ((PMSLLHOOKSTRUCT)lParam)->pt;

			//Make sure cursorwnd isn't in the way
			ShowWindow(g_hwnd, SW_HIDE);

			//Print window info
			POINT *p_pt = malloc(sizeof(pt));
			*p_pt = pt;
			CreateThread(NULL, 0, FindWnd, p_pt, 0, NULL);

			//Unhook mouse
			UnhookMouse();

			//Prevent mousedown from propagating
			return 1;
		}
		else if (wParam == WM_RBUTTONDOWN) {
			//Disable mouse
			DisableMouse();
			//Prevent mousedown from propagating
			return 1;
		}
		else if (wParam == WM_RBUTTONUP) {
			//Unhook mouse
			UnhookMouse();
			//Prevent mouseup from propagating
			return 1;
		}
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int HookMouse() {
	if (mousehook) {
		//Mouse already hooked
		return 1;
	}

	//Set up the hook
	mousehook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, g_hinst, 0);
	if (mousehook == NULL) {
		Error(L"SetWindowsHookEx(WH_MOUSE_LL)", L"Check the "APP_NAME" website if there is an update, if the latest version doesn't fix this, please report it.", GetLastError());
		return 1;
	}

	//Show cursor
	int left = GetSystemMetrics(SM_XVIRTUALSCREEN);
	int top = GetSystemMetrics(SM_YVIRTUALSCREEN);
	int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	MoveWindow(g_hwnd, left, top, width, height, FALSE);
	ShowWindowAsync(g_hwnd, SW_SHOWNA);

	//Success
	find = 1;
	return 0;
}

DWORD WINAPI DelayedUnhookMouse() {
	//Sleep so mouse events have time to be canceled
	Sleep(100);

	//Unhook the mouse hook
	if (UnhookWindowsHookEx(mousehook) == 0) {
		Error(L"UnhookWindowsHookEx(mousehook)", L"UnhookMouse()", GetLastError());
		return 1;
	}

	//Success
	mousehook = NULL;
}

int UnhookMouse() {
	if (!mousehook) {
		//Mouse not hooked
		return 1;
	}

	//Disable
	DisableMouse();

	//Unhook
	CreateThread(NULL, 0, DelayedUnhookMouse, NULL, 0, NULL);

	//Success
	return 0;
}

int DisableMouse() {
	//Disable
	find = 0;

	//Hide cursor
	ShowWindow(g_hwnd, SW_HIDE);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_TRAY) {
		if (lParam == WM_LBUTTONDOWN) {
			HookMouse();
		}
		else if (lParam == WM_MBUTTONDOWN) {
			Sleep(3000);
			HookMouse();
		}
		else if (lParam == WM_RBUTTONDOWN) {
			ShowContextMenu(hwnd);
		}
	}
	else if (msg == WM_TASKBARCREATED) {
		tray_added = 0;
		UpdateTray();
	}
	else if (msg == WM_COMMAND) {
		int wmId = LOWORD(wParam), wmEvent = HIWORD(wParam);
		if (wmId == SWM_FIND) {
			HookMouse();
		}
		else if (wmId == SWM_FINDDELAY) {
			Sleep(3000);
			HookMouse();
		}
		else if (wmId == SWM_FINDALL) {
			FindAllWnds();
		}
		else if (wmId == SWM_ABOUT) {
			MessageBox(NULL, l10n->about, l10n->about_title, MB_ICONINFORMATION|MB_OK);
		}
		else if (wmId == SWM_EXIT) {
			DestroyWindow(hwnd);
		}
	}
	else if (msg == WM_DESTROY) {
		showerror = 0;
		UnhookMouse();
		RemoveTray();
		PostQuitMessage(0);
		return 0;
	}
	else if (msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN) {
		//Hide the window if clicked on, this might happen if it wasn't hidden by the hooks for some reason
		ShowWindow(hwnd, SW_HIDE);
		//Since we take away the cursor, make sure the mouse is unhooked
		UnhookMouse();
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
