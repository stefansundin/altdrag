/*
	Copyright (C) 2011  Stefan Sundin (recover89@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

#define UNICODE
#define _UNICODE
#define _WIN32_WINNT 0x0501
#define _WIN32_IE 0x0600

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <shlwapi.h>

//App
#define APP_NAME            L"AltDrag"
#define APP_VERSION         "1.0b1"
#define APP_URL             L"http://code.google.com/p/altdrag/"
#define APP_UPDATE_STABLE   L"http://altdrag.googlecode.com/svn/wiki/latest-stable.txt"
#define APP_UPDATE_UNSTABLE L"http://altdrag.googlecode.com/svn/wiki/latest-unstable.txt"

//Messages
#define WM_TRAY                WM_USER+1
#define SWM_TOGGLE             WM_APP+1
#define SWM_HIDE               WM_APP+2
#define SWM_AUTOSTART_ON       WM_APP+3
#define SWM_AUTOSTART_OFF      WM_APP+4
#define SWM_AUTOSTART_HIDE_ON  WM_APP+5
#define SWM_AUTOSTART_HIDE_OFF WM_APP+6
#define SWM_SETTINGS           WM_APP+7
#define SWM_CHECKFORUPDATE     WM_APP+8
#define SWM_UPDATE             WM_APP+9
#define SWM_ABOUT              WM_APP+10
#define SWM_EXIT               WM_APP+11

//Stuff missing in MinGW
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
UINT WM_UPDATESETTINGS = 0;
UINT WM_ADDTRAY = 0;
UINT WM_HIDETRAY = 0;
UINT WM_OPENCONFIG = 0;

//Cool stuff
struct {
	int InactiveScroll;
	int HookWindows;
} settings = {0, 0};
HINSTANCE hinstDLL = NULL;
HHOOK keyhook = NULL;
HHOOK scrollhook = NULL;
HHOOK msghook = NULL;
BOOL x64 = FALSE;

//Include stuff
#include "localization/strings.h"
#include "include/error.c"
#include "include/autostart.c"
#include "include/tray.c"
#include "include/update.c"

//Entry point
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
	g_hinst = hInst;
	
	//Check command line
	if (!strcmp(szCmdLine,"-hide")) {
		hide = 1;
	}
	
	//Look for previous instance
	WM_UPDATESETTINGS = RegisterWindowMessage(L"UpdateSettings");
	WM_ADDTRAY = RegisterWindowMessage(L"AddTray");
	WM_HIDETRAY = RegisterWindowMessage(L"HideTray");
	WM_OPENCONFIG = RegisterWindowMessage(L"OpenConfig");
	HWND previnst = FindWindow(APP_NAME, NULL);
	if (previnst != NULL) {
		PostMessage(previnst, WM_UPDATESETTINGS, 0, 0);
		PostMessage(previnst, (hide?WM_HIDETRAY:WM_ADDTRAY), 0, 0);
		PostMessage(previnst, WM_OPENCONFIG, 0, 0);
		return 0;
	}
	
	//Create window
	WNDCLASSEX wnd = {sizeof(WNDCLASSEX), 0, WindowProc, 0, 0, hInst, NULL, NULL, (HBRUSH)(COLOR_WINDOW+1), NULL, APP_NAME, NULL};
	RegisterClassEx(&wnd);
	g_hwnd = CreateWindowEx(WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_LAYERED, wnd.lpszClassName, APP_NAME, WS_POPUP, 0, 0, 0, 0, NULL, NULL, hInst, NULL);
	SetLayeredWindowAttributes(g_hwnd, 0, 1, LWA_ALPHA); //Almost transparent
	
	//Load settings
	wchar_t path[MAX_PATH];
	GetModuleFileName(NULL, path, sizeof(path)/sizeof(wchar_t));
	PathRemoveFileSpec(path);
	wcscat(path, L"\\"APP_NAME".ini");
	wchar_t txt[10];
	GetPrivateProfileString(APP_NAME, L"Language", L"en-US", txt, sizeof(txt)/sizeof(wchar_t), path);
	int i;
	for (i=0; languages[i].code != NULL; i++) {
		if (!wcsicmp(txt,languages[i].code)) {
			l10n = languages[i].strings;
			break;
		}
	}
	SendMessage(g_hwnd, WM_UPDATESETTINGS, 0, 0);
	IsWow64Process(GetCurrentProcess(), &x64);
	
	//Tray icon
	InitTray();
	UpdateTray();
	
	//Hook system
	HookSystem();
	
	//Add tray if hook failed, even though -hide was supplied
	if (hide && (!keyhook || (settings.HookWindows && !msghook))) {
		hide = 0;
		UpdateTray();
	}
	
	//Check for update
	GetPrivateProfileString(L"Update", L"CheckOnStartup", L"0", txt, sizeof(txt)/sizeof(wchar_t), path);
	int checkforupdate = _wtoi(txt);
	if (checkforupdate) {
		CheckForUpdate(0);
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
	if (keyhook && scrollhook && msghook) {
		//System already hooked
		return 1;
	}
	
	//Load library
	if (!hinstDLL) {
		wchar_t path[MAX_PATH];
		GetModuleFileName(NULL, path, sizeof(path)/sizeof(wchar_t));
		PathRemoveFileSpec(path);
		wcscat(path, L"\\hooks.dll");
		hinstDLL = LoadLibrary(path);
		if (hinstDLL == NULL) {
			Error(L"LoadLibrary('hooks.dll')", L"This probably means that the file hooks.dll is missing.\nYou can try to download "APP_NAME" again from the website.", GetLastError(), TEXT(__FILE__), __LINE__);
			return 1;
		}
	}
	
	HOOKPROC procaddr;
	if (!keyhook) {
		//Get address to keyboard hook (beware name mangling)
		procaddr = (HOOKPROC)GetProcAddress(hinstDLL, "LowLevelKeyboardProc@12");
		if (procaddr == NULL) {
			Error(L"GetProcAddress('LowLevelKeyboardProc@12')", L"This probably means that the file hooks.dll is from an old version or corrupt.\nYou can try to download "APP_NAME" again from the website.", GetLastError(), TEXT(__FILE__), __LINE__);
			return 1;
		}
		//Set up the keyboard hook
		keyhook = SetWindowsHookEx(WH_KEYBOARD_LL, procaddr, hinstDLL, 0);
		if (keyhook == NULL) {
			Error(L"SetWindowsHookEx(WH_KEYBOARD_LL)", L"Could not hook keyboard. Another program might be interfering.", GetLastError(), TEXT(__FILE__), __LINE__);
			return 1;
		}
	}
	
	if (!scrollhook && settings.InactiveScroll) {
		//Get address to scroll hook (beware name mangling)
		procaddr = (HOOKPROC)GetProcAddress(hinstDLL, "ScrollHook@12");
		if (procaddr == NULL) {
			Error(L"GetProcAddress('ScrollHook@12')", L"This probably means that the file hooks.dll is from an old version or corrupt.\nYou can try to download "APP_NAME" again from the website.", GetLastError(), TEXT(__FILE__), __LINE__);
			return 1;
		}
		//Set up the scroll hook
		scrollhook = SetWindowsHookEx(WH_MOUSE_LL, procaddr, hinstDLL, 0);
		if (scrollhook == NULL) {
			Error(L"SetWindowsHookEx(WH_MOUSE_LL)", L"Could not scroll hook. Another program might be interfering.", GetLastError(), TEXT(__FILE__), __LINE__);
			return 1;
		}
	}
	
	if (!msghook && settings.HookWindows) {
		//Get address to message hook (beware name mangling)
		procaddr = (HOOKPROC)GetProcAddress(hinstDLL, "CallWndProc@12");
		if (procaddr == NULL) {
			Error(L"GetProcAddress('CallWndProc@12')", L"This probably means that the file hooks.dll is from an old version or corrupt.\nYou can try to download "APP_NAME" again from the website.", GetLastError(), TEXT(__FILE__), __LINE__);
			return 1;
		}
		//Set up the message hook
		msghook = SetWindowsHookEx(WH_CALLWNDPROC, procaddr, hinstDLL, 0);
		if (msghook == NULL) {
			Error(L"SetWindowsHookEx(WH_CALLWNDPROC)", L"Could not hook windows. Another program might be interfering.", GetLastError(), TEXT(__FILE__), __LINE__);
			return 1;
		}
		
		//x64
		if (x64) {
			wchar_t path[MAX_PATH];
			GetModuleFileName(NULL, path, sizeof(path)/sizeof(wchar_t));
			PathRemoveFileSpec(path);
			wcscat(path, L"\\HookWindows_x64.exe");
			ShellExecute(NULL, L"open", path, L"nowarning", NULL, SW_SHOWNORMAL);
		}
	}
	
	//Success
	UpdateTray();
	return 0;
}

//Force processes to unload hooks.dll by sending them a dummy message
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	PostMessage(hwnd, WM_NULL, 0, 0);
	return TRUE;
}

int UnhookSystem() {
	if (!keyhook && !scrollhook && !msghook) {
		//System not hooked
		return 1;
	}
	
	//Remove keyboard hook
	if (keyhook) {
		if (UnhookWindowsHookEx(keyhook) == 0) {
			Error(L"UnhookWindowsHookEx(keyhook)", L"Could not unhook keyboard. Try restarting "APP_NAME".", GetLastError(), TEXT(__FILE__), __LINE__);
		}
		keyhook = NULL;
	}
	
	//Remove mouse hook
	if (scrollhook) {
		if (UnhookWindowsHookEx(scrollhook) == 0) {
			Error(L"UnhookWindowsHookEx(scrollhook)", L"Could not unhook mouse. Try restarting "APP_NAME".", GetLastError(), TEXT(__FILE__), __LINE__);
		}
		scrollhook = NULL;
	}
	
	//Remove message hook
	if (msghook) {
		if (UnhookWindowsHookEx(msghook) == 0) {
			Error(L"UnhookWindowsHookEx(msghook)", L"Could not unhook windows. Try restarting "APP_NAME".", GetLastError(), TEXT(__FILE__), __LINE__);
		}
		msghook = NULL;
		
		//Close HookWindows_x64.exe
		if (x64) {
			HWND window = FindWindow(L"AltDrag-x64", NULL);
			if (window != NULL) {
				PostMessage(window, WM_CLOSE, 0, 0);
			}
		}
		
		//Send dummy messages to all processes to make them unload hooks.dll
		EnumWindows(EnumWindowsProc, 0);
	}
	
	//Clear sharedsettings_loaded flag in dll (sometimes it isn't cleared because msghook keeps it alive somehow)
	void (*ClearSettings)() = (void*)GetProcAddress(hinstDLL, "ClearSettings");
	ClearSettings();
	
	//Unload library
	if (hinstDLL) {
		if (FreeLibrary(hinstDLL) == 0) {
			Error(L"FreeLibrary()", L"Could not free hooks.dll. Try restarting "APP_NAME".", GetLastError(), TEXT(__FILE__), __LINE__);
		}
		hinstDLL = NULL;
	}
	
	//Success
	UpdateTray();
	return 0;
}

int enabled() {
	return (keyhook || scrollhook || msghook);
}

void ToggleState() {
	if (enabled()) {
		UnhookSystem();
	}
	else {
		SendMessage(g_hwnd, WM_UPDATESETTINGS, 0, 0);
		HookSystem();
	}
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_TRAY) {
		if (lParam == WM_LBUTTONDOWN || lParam == WM_LBUTTONDBLCLK) {
			ToggleState();
			if (lParam == WM_LBUTTONDBLCLK) {
				SendMessage(hwnd, WM_COMMAND, SWM_SETTINGS, 0);
			}
		}
		else if (lParam == WM_MBUTTONDOWN) {
			wchar_t path[MAX_PATH];
			GetModuleFileName(NULL, path, sizeof(path)/sizeof(wchar_t));
			PathRemoveFileSpec(path);
			wcscat(path, L"\\"APP_NAME".ini");
			ShellExecute(NULL, L"open", path, NULL, NULL, SW_SHOWNORMAL);
		}
		else if (lParam == WM_RBUTTONDOWN) {
			ShowContextMenu(hwnd);
		}
		else if (lParam == NIN_BALLOONUSERCLICK) {
			hide=0;
			SendMessage(hwnd, WM_COMMAND, SWM_UPDATE, 0);
		}
		else if (lParam == NIN_BALLOONTIMEOUT) {
			if (hide) {
				RemoveTray();
			}
		}
	}
	else if (msg == WM_UPDATESETTINGS) {
		//Load settings
		wchar_t path[MAX_PATH];
		GetModuleFileName(NULL, path, sizeof(path)/sizeof(wchar_t));
		PathRemoveFileSpec(path);
		wcscat(path, L"\\"APP_NAME".ini");
		wchar_t txt[10];
		//InactiveScroll
		GetPrivateProfileString(APP_NAME, L"InactiveScroll", L"0", txt, sizeof(txt)/sizeof(wchar_t), path);
		swscanf(txt, L"%d", &settings.InactiveScroll);
		//HookWindows
		GetPrivateProfileString(APP_NAME, L"HookWindows", L"0", txt, sizeof(txt)/sizeof(wchar_t), path);
		swscanf(txt, L"%d", &settings.HookWindows);
		//Language
		GetPrivateProfileString(APP_NAME, L"Language", L"en-US", txt, sizeof(txt)/sizeof(wchar_t), path);
		int i;
		for (i=0; languages[i].code != NULL; i++) {
			if (!wcsicmp(txt,languages[i].code)) {
				l10n = languages[i].strings;
				break;
			}
		}
		//Reload hooks
		if (enabled()) {
			UnhookSystem();
			HookSystem();
		}
	}
	else if (msg == WM_ADDTRAY) {
		hide = 0;
		UpdateTray();
	}
	else if (msg == WM_HIDETRAY) {
		hide = 1;
		RemoveTray();
	}
	else if (msg == WM_OPENCONFIG) {
		wchar_t path[MAX_PATH];
		GetModuleFileName(NULL, path, sizeof(path)/sizeof(wchar_t));
		PathRemoveFileSpec(path);
		wcscat(path, L"\\Config.exe");
		ShellExecute(NULL, L"open", path, NULL, NULL, SW_SHOWNORMAL);
	}
	else if (msg == WM_TASKBARCREATED) {
		tray_added = 0;
		UpdateTray();
	}
	else if (msg == WM_COMMAND) {
		int wmId=LOWORD(wParam), wmEvent=HIWORD(wParam);
		if (wmId == SWM_TOGGLE) {
			ToggleState();
		}
		else if (wmId == SWM_HIDE) {
			hide = 1;
			RemoveTray();
		}
		else if (wmId == SWM_AUTOSTART_ON) {
			SetAutostart(1, 0);
		}
		else if (wmId == SWM_AUTOSTART_OFF) {
			SetAutostart(0, 0);
		}
		else if (wmId == SWM_AUTOSTART_HIDE_ON) {
			SetAutostart(1, 1);
		}
		else if (wmId == SWM_AUTOSTART_HIDE_OFF) {
			SetAutostart(1, 0);
		}
		else if (wmId == SWM_CHECKFORUPDATE) {
			CheckForUpdate(1);
		}
		else if (wmId == SWM_UPDATE) {
			if (MessageBox(NULL, l10n->update_dialog, APP_NAME, MB_ICONINFORMATION|MB_YESNO|MB_SYSTEMMODAL) == IDYES) {
				ShellExecute(NULL, L"open", APP_URL, NULL, NULL, SW_SHOWNORMAL);
			}
		}
		else if (wmId == SWM_SETTINGS || wmId == SWM_ABOUT) {
			wchar_t path[MAX_PATH];
			GetModuleFileName(NULL, path, sizeof(path)/sizeof(wchar_t));
			PathRemoveFileSpec(path);
			wcscat(path, L"\\Config.exe");
			ShellExecute(NULL, L"open", path, (wmId==SWM_ABOUT)?L"4":NULL, NULL, SW_SHOWNORMAL);
		}
		else if (wmId == SWM_EXIT) {
			DestroyWindow(hwnd);
		}
	}
	else if (msg == WM_DESTROY) {
		showerror = 0;
		UnhookSystem();
		RemoveTray();
		PostQuitMessage(0);
		return 0;
	}
	else if (msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN) {
		//Hide cursorwnd if clicked on, this might happen if it wasn't hidden by hooks.c for some reason
		ShowWindow(hwnd, SW_HIDE);
	}
	else if (msg == WM_POWERBROADCAST && wParam == PBT_APMRESUMEAUTOMATIC && settings.InactiveScroll == 1 && enabled()) {
		//Silently rehook scroll hook when resuming from hibernation
		//This is because Windows sometimes drops mouse hooks after hibernation
		//HACK: Set InactiveScroll=2 to disable this behavior
		UnhookWindowsHookEx(scrollhook);
		HOOKPROC procaddr = (HOOKPROC)GetProcAddress(hinstDLL, "ScrollHook@12");
		scrollhook = SetWindowsHookEx(WH_MOUSE_LL, procaddr, hinstDLL, 0);
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
