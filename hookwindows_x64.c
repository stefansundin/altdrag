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
#define _WIN32_IE 0x0600

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <shlwapi.h>

//App
#define APP_NAME L"AltDrag"

//Boring stuff
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

//Cool stuff
HINSTANCE hinstDLL = NULL;
HHOOK keyhook = NULL;
HHOOK msghook = NULL;

//Error()
#include "include/error.h"

//Entry point
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
	//Warn user
	if (!strcmp(szCmdLine,"")) {
		MessageBox(NULL, L"HookWindows_x64.exe is launched internally by "APP_NAME" if you have enabled HookWindows. There is no need to launch it manually.\n\nIf you still want to do this, launch HookWindows_x64.exe with an argument (it can be anything) to bypass this dialog.\n\nKeep in mind that HookWindows_x64.exe will automatically exit if it can't find "APP_NAME" running.", L"HookWindows_x64.exe", MB_ICONINFORMATION|MB_OK);
		return 1;
	}
	
	//Look for previous instance and make sure AltDrag is running
	if (FindWindow(APP_NAME"-x64",NULL) != NULL
	 || FindWindow(APP_NAME,NULL) == NULL) {
		return 0;
	}
	
	//Create window class
	WNDCLASSEX wnd;
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.style = 0;
	wnd.lpfnWndProc = WindowProc;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = hInst;
	wnd.hIcon = NULL;
	wnd.hIconSm = NULL;
	wnd.hCursor = LoadImage(NULL, IDC_HAND, IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
	wnd.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wnd.lpszMenuName = NULL;
	wnd.lpszClassName = APP_NAME"-x64";
	
	//Register class
	RegisterClassEx(&wnd);
	
	//Create window
	HWND hwnd = CreateWindowEx(0, wnd.lpszClassName, APP_NAME"-x64", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);
	
	//Start a timer that checks if AltDrag is still running every 10 seconds
	SetTimer(hwnd, 0, 10000, NULL);
	
	//Hook system
	HookSystem();
	
	//Exit if msghook failed
	if (!msghook) {
		return 1;
	}
	
	//Message loop
	MSG msg;
	while (GetMessage(&msg,NULL,0,0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

int HookSystem() {
	if (keyhook && msghook) {
		//System already hooked
		return 1;
	}
	
	//Load library
	if (!hinstDLL) {
		wchar_t path[MAX_PATH];
		GetModuleFileName(NULL, path, sizeof(path)/sizeof(wchar_t));
		PathRemoveFileSpec(path);
		wcscat(path, L"\\hooks_x64.dll");
		hinstDLL = LoadLibrary(path);
		if (hinstDLL == NULL) {
			Error(L"LoadLibrary('hooks_x64.dll')", L"This probably means that the file hooks_x64.dll is missing.", GetLastError(), TEXT(__FILE__), __LINE__);
			return 1;
		}
	}
	
	HOOKPROC procaddr;
	if (!keyhook) {
		//Get address to keyboard hook (beware name mangling)
		procaddr = (HOOKPROC)GetProcAddress(hinstDLL, "LowLevelKeyboardProc");
		if (procaddr == NULL) {
			Error(L"GetProcAddress('LowLevelKeyboardProc')", L"This probably means that the file hooks_x64.dll is from an old version or corrupt.", GetLastError(), TEXT(__FILE__), __LINE__);
			return 1;
		}
		//Set up the keyboard hook
		keyhook = SetWindowsHookEx(WH_KEYBOARD_LL, procaddr, hinstDLL, 0);
		if (keyhook == NULL) {
			Error(L"SetWindowsHookEx(WH_KEYBOARD_LL)", L"Check the "APP_NAME" website if there is an update, if the latest version doesn't fix this, please report it.", GetLastError(), TEXT(__FILE__), __LINE__);
			return 1;
		}
	}
	
	//Get address to message hook (beware name mangling)
	if (!msghook) {
		procaddr = (HOOKPROC)GetProcAddress(hinstDLL, "CallWndProc");
		if (procaddr == NULL) {
			Error(L"GetProcAddress('CallWndProc')", L"This probably means that the file hooks_x64.dll is from an old version or corrupt.", GetLastError(), TEXT(__FILE__), __LINE__);
			return 1;
		}
		//Set up the message hook
		msghook = SetWindowsHookEx(WH_CALLWNDPROC, procaddr, hinstDLL, 0);
		if (msghook == NULL) {
			Error(L"SetWindowsHookEx(WH_CALLWNDPROC)", L"Check the "APP_NAME" website if there is an update, if the latest version doesn't fix this, please report it.",GetLastError(),TEXT(__FILE__),__LINE__);
			return 1;
		}
	}
	
	//Success
	return 0;
}

int UnhookSystem() {
	if (!keyhook && !msghook) {
		//System not hooked
		return 1;
	}
	
	//Remove keyboard hook
	if (UnhookWindowsHookEx(keyhook) == 0) {
		Error(L"UnhookWindowsHookEx(keyhook)", L"Check the "APP_NAME" website if there is an update, if the latest version doesn't fix this, please report it.", GetLastError(), TEXT(__FILE__), __LINE__);
		return 1;
	}
	keyhook = NULL;
	
	//Remove message hook
	if (UnhookWindowsHookEx(msghook) == 0) {
		Error(L"UnhookWindowsHookEx(msghook)", L"Check the "APP_NAME" website if there is an update, if the latest version doesn't fix this, please report it.",GetLastError(),TEXT(__FILE__),__LINE__);
		return 1;
	}
	msghook = NULL;
	
	//Clear sharedsettings_loaded flag in dll (sometimes it isn't cleared because msghook keeps it alive somehow)
	void (*ClearSettings)() = (void*)GetProcAddress(hinstDLL, "ClearSettings");
	ClearSettings();
	
	if (hinstDLL) {
		//Unload library
		if (FreeLibrary(hinstDLL) == 0) {
			Error(L"FreeLibrary()", L"Check the "APP_NAME" website if there is an update, if the latest version doesn't fix this, please report it.", GetLastError(), TEXT(__FILE__), __LINE__);
			return 1;
		}
		hinstDLL = NULL;
	}
	
	//Success
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_DESTROY) {
		showerror = 0;
		UnhookSystem();
		PostQuitMessage(0);
		return 0;
	}
	else if (msg == WM_TIMER) {
		//Exit if AltDrag is not running
		if (FindWindow(APP_NAME,NULL) == NULL) {
			SendMessage(hwnd, WM_CLOSE, 0, 0);
		}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
