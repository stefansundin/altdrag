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
#define APP_NAME L"AltDrag-x64"

//Boring stuff
LRESULT CALLBACK WindowProc(HWND,UINT,WPARAM,LPARAM);
UINT WM_UPDATESETTINGS = 0;
wchar_t txt[1000];

//Cool stuff
HINSTANCE hinstDLL = NULL;
HHOOK keyhook = NULL;
HHOOK msghook = NULL;

//Error()
#include "include/error.h"

//Entry point
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
	//Look for previous instance
	WM_UPDATESETTINGS = RegisterWindowMessage(L"UpdateSettings");
	HWND previnst = FindWindow(APP_NAME,NULL);
	if (previnst != NULL) {
		PostMessage(previnst, WM_UPDATESETTINGS, 0, 0);
		return 0;
	}
	
	//Create window class
	WNDCLASSEX wnd;
	wnd.cbSize=sizeof(WNDCLASSEX);
	wnd.style=0;
	wnd.lpfnWndProc=WindowProc;
	wnd.cbClsExtra=0;
	wnd.cbWndExtra=0;
	wnd.hInstance=hInst;
	wnd.hIcon=NULL;
	wnd.hIconSm=NULL;
	wnd.hCursor=LoadImage(NULL, IDC_HAND, IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
	wnd.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	wnd.lpszMenuName=NULL;
	wnd.lpszClassName=APP_NAME;
	
	//Register class
	RegisterClassEx(&wnd);
	
	//Create window
	HWND hwnd=CreateWindowEx(0, wnd.lpszClassName, APP_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);
	
	//Hook system
	HookSystem();
	
	//Exit if msghook failed
	if (!msghook) {
		return 1;
	}
	
	//Message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

int HookSystem() {
	if (msghook) {
		//System already hooked
		return 1;
	}
	
	//Update settings
	//SendMessage(traydata.hWnd, WM_UPDATESETTINGS, 0, 0);
	
	//Load library
	if (!hinstDLL) {
		wchar_t path[MAX_PATH];
		GetModuleFileName(NULL, path, sizeof(path)/sizeof(wchar_t));
		PathRemoveFileSpec(path);
		wcscat(path, L"\\hooks_x64.dll");
		hinstDLL = LoadLibrary(path);
		if (hinstDLL == NULL) {
			Error(L"LoadLibrary()", L"This probably means that the file hooks_x64.dll is missing.\nYou can try to download "APP_NAME" again from the website.", GetLastError(), TEXT(__FILE__), __LINE__);
			return 1;
		}
	}
	
	HOOKPROC procaddr;
	if (!keyhook) {
		//Get address to keyboard hook (beware name mangling)
		procaddr = (HOOKPROC)GetProcAddress(hinstDLL,"LowLevelKeyboardProc");
		if (procaddr == NULL) {
			Error(L"GetProcAddress('LowLevelKeyboardProc')", L"This probably means that the file hooks_x64.dll is from an old version or corrupt.\nYou can try to download "APP_NAME" again from the website.", GetLastError(), TEXT(__FILE__), __LINE__);
			return 1;
		}
		//Set up the keyboard hook
		keyhook = SetWindowsHookEx(WH_KEYBOARD_LL,procaddr,hinstDLL,0);
		if (keyhook == NULL) {
			Error(L"SetWindowsHookEx(WH_KEYBOARD_LL)", L"Check the "APP_NAME" website if there is an update, if the latest version doesn't fix this, please report it.", GetLastError(), TEXT(__FILE__), __LINE__);
			return 1;
		}
	}
	
	//Get address to message hook (beware name mangling)
	procaddr = (HOOKPROC)GetProcAddress(hinstDLL,"CallWndProc");
	if (procaddr == NULL) {
		Error(L"GetProcAddress('CallWndProc')",L"This probably means that the file hooks_x64.dll is from an old version or corrupt.\nYou can try to download "APP_NAME" again from the website.", GetLastError(), TEXT(__FILE__), __LINE__);
		return 1;
	}
	
	//Set up the message hook
	msghook = SetWindowsHookEx(WH_CALLWNDPROC,procaddr,hinstDLL,0);
	if (msghook == NULL) {
		Error(L"SetWindowsHookEx(WH_CALLWNDPROC)",L"Check the "APP_NAME" website if there is an update, if the latest version doesn't fix this, please report it.",GetLastError(),TEXT(__FILE__),__LINE__);
		return 1;
	}
	
	//Success
	return 0;
}

int UnhookSystem() {
	if (!msghook) {
		//System not hooked
		return 1;
	}
	
	//Remove message hook
	if (UnhookWindowsHookEx(msghook) == 0) {
		Error(L"UnhookWindowsHookEx(msghook)",L"Check the "APP_NAME" website if there is an update, if the latest version doesn't fix this, please report it.",GetLastError(),TEXT(__FILE__),__LINE__);
		return 1;
	}
	msghook = NULL;
	
	//Clear sharedsettings_loaded flag in dll (sometimes it isn't cleared because msghook keeps it alive somehow)
	void (*ClearSettings)() = (void*)GetProcAddress(hinstDLL,"ClearSettings");
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

void ToggleState() {
	if (msghook) {
		UnhookSystem();
	}
	else {
		HookSystem();
	}
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_UPDATESETTINGS) {
		/*//Load settings
		wchar_t path[MAX_PATH];
		GetModuleFileName(NULL,path,sizeof(path)/sizeof(wchar_t));
		PathRenameExtension(path,L".ini");
		//HookApps
		GetPrivateProfileString(APP_NAME,L"HookApps",L"0",txt,sizeof(txt)/sizeof(wchar_t),path);
		swscanf(txt,L"%d",&settings.HookApps);
		//Language
		GetPrivateProfileString(APP_NAME,L"Language",L"en-US",txt,sizeof(txt)/sizeof(wchar_t),path);
		int i;
		for (i=0; i < num_languages; i++) {
			if (!wcscmp(txt,languages[i].code)) {
				l10n=languages[i].strings;
			}
		}
		//Reload hooks
		if (keyhook || msghook) {
			UnhookSystem();
			HookSystem();
		}*/
	}
	else if (msg == WM_DESTROY) {
		showerror = 0;
		UnhookSystem();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
