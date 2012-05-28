/*
	Copyright (C) 2012  Stefan Sundin (recover89@gmail.com)
	
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
HINSTANCE g_hinst = NULL;
HWND g_hwnd = NULL;

//Cool stuff
HINSTANCE hinstDLL = NULL;
HHOOK keyhook = NULL;
HHOOK msghook = NULL;

//Include stuff
#include "include/error.c"

//Entry point
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
	g_hinst = hInst;
	
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
	
	//Create window
	WNDCLASSEX wnd = {sizeof(WNDCLASSEX), 0, WindowProc, 0, 0, hInst, NULL, NULL, NULL, NULL, APP_NAME"-x64", NULL};
	RegisterClassEx(&wnd);
	g_hwnd = CreateWindowEx(0, wnd.lpszClassName, NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, hInst, NULL);
	
	//Start a timer that checks if AltDrag is still running every 10 seconds
	SetTimer(g_hwnd, 0, 10000, NULL);
	
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
			Error(L"LoadLibrary('hooks_x64.dll')", L"This probably means that the file hooks_x64.dll is missing. You can try reinstalling "APP_NAME".", GetLastError(), TEXT(__FILE__), __LINE__);
			return 1;
		}
	}
	
	HOOKPROC procaddr;
	if (!keyhook) {
		//Get address to keyboard hook (beware name mangling)
		procaddr = (HOOKPROC)GetProcAddress(hinstDLL, "LowLevelKeyboardProc");
		if (procaddr == NULL) {
			Error(L"GetProcAddress('LowLevelKeyboardProc')", L"This probably means that the file hooks_x64.dll is from an old version or corrupt. You can try reinstalling "APP_NAME".", GetLastError(), TEXT(__FILE__), __LINE__);
			return 1;
		}
		//Set up the keyboard hook
		keyhook = SetWindowsHookEx(WH_KEYBOARD_LL, procaddr, hinstDLL, 0);
		if (keyhook == NULL) {
			Error(L"SetWindowsHookEx(WH_KEYBOARD_LL)", L"Could not hook keyboard. Another program might be interfering.", GetLastError(), TEXT(__FILE__), __LINE__);
			return 1;
		}
	}
	
	//Get address to message hook (beware name mangling)
	if (!msghook) {
		procaddr = (HOOKPROC)GetProcAddress(hinstDLL, "CallWndProc");
		if (procaddr == NULL) {
			Error(L"GetProcAddress('CallWndProc')", L"This probably means that the file hooks_x64.dll is from an old version or corrupt. You can try reinstalling "APP_NAME".", GetLastError(), TEXT(__FILE__), __LINE__);
			return 1;
		}
		//Set up the message hook
		msghook = SetWindowsHookEx(WH_CALLWNDPROC, procaddr, hinstDLL, 0);
		if (msghook == NULL) {
			Error(L"SetWindowsHookEx(WH_CALLWNDPROC)", L"Could not hook message hook. Another program might be interfering.", GetLastError(), TEXT(__FILE__), __LINE__);
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
		Error(L"UnhookWindowsHookEx(keyhook)", L"Could not unhook keyboard. Try restarting "APP_NAME".", GetLastError(), TEXT(__FILE__), __LINE__);
		return 1;
	}
	keyhook = NULL;
	
	//Remove message hook
	if (UnhookWindowsHookEx(msghook) == 0) {
		Error(L"UnhookWindowsHookEx(msghook)", L"Could not unhook message hook. Try restarting "APP_NAME".", GetLastError(), TEXT(__FILE__), __LINE__);
		return 1;
	}
	msghook = NULL;
	
	//Tell dll file that we are unloading
	void (*Unload)() = (void*)GetProcAddress(hinstDLL, "Unload");
	if (Unload == NULL) {
		Error(L"GetProcAddress('Unload')", L"This probably means that the file hooks_x64.dll is from an old version or corrupt. You can try reinstalling "APP_NAME".", GetLastError(), TEXT(__FILE__), __LINE__);
	}
	else {
		Unload();
	}
	
	if (hinstDLL) {
		//Unload library
		if (FreeLibrary(hinstDLL) == 0) {
			Error(L"FreeLibrary()", L"Could not free hooks_x64.dll. Try restarting "APP_NAME".", GetLastError(), TEXT(__FILE__), __LINE__);
			return 1;
		}
		hinstDLL = NULL;
	}
	
	//Success
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_DESTROY || msg == WM_QUERYENDSESSION) {
		showerror = 0;
		UnhookSystem();
		PostQuitMessage(0);
	}
	else if (msg == WM_TIMER) {
		//Exit if AltDrag is not running
		if (FindWindow(APP_NAME,NULL) == NULL) {
			SendMessage(hwnd, WM_CLOSE, 0, 0);
		}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
