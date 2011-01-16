/*
	Tray functions.
	Copyright (C) 2011  Stefan Sundin (recover89@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

NOTIFYICONDATA tray;
HICON icon;
int tray_added = 0;
int hide = 0;
extern int update;

int InitTray() {
	//Load icons
	icon = LoadImage(g_hinst, L"app_icon", IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	if (icon == NULL) {
		Error(L"LoadImage('app_icon')", L"Could not load tray icon.", GetLastError(), TEXT(__FILE__), __LINE__);
		return 1;
	}
	
	//Create icondata
	tray.cbSize = sizeof(NOTIFYICONDATA);
	tray.uID = 0;
	tray.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP;
	tray.hWnd = g_hwnd;
	tray.uCallbackMessage = WM_TRAY;
	tray.hIcon = icon;
	wcsncpy(tray.szTip, APP_NAME, sizeof(tray.szTip)/sizeof(wchar_t));
	//Balloon tooltip
	tray.uTimeout = 10000;
	wcsncpy(tray.szInfoTitle, APP_NAME, sizeof(tray.szInfoTitle)/sizeof(wchar_t));
	tray.dwInfoFlags = NIIF_USER;
	
	//Register TaskbarCreated so we can re-add the tray icon if (when) explorer.exe crashes
	WM_TASKBARCREATED = RegisterWindowMessage(L"TaskbarCreated");
	
	return 0;
}

int UpdateTray() {
	//Only add or modify if not hidden or if balloon will be displayed
	if (!hide || tray.uFlags&NIF_INFO) {
		//Try until it succeeds, sleep 100 ms between each attempt
		while (Shell_NotifyIcon((tray_added?NIM_MODIFY:NIM_ADD),&tray) == FALSE) {
			Sleep(100);
		}
		
		//Success
		tray_added = 1;
	}
	return 0;
}

int RemoveTray() {
	if (!tray_added) {
		//Tray not added
		return 1;
	}
	
	if (Shell_NotifyIcon(NIM_DELETE,&tray) == FALSE) {
		Error(L"Shell_NotifyIcon(NIM_DELETE)", L"Failed to remove tray icon.", GetLastError(), TEXT(__FILE__), __LINE__);
		return 1;
	}
	
	//Success
	tray_added = 0;
	return 0;
}

void ShowContextMenu(HWND hwnd) {
	POINT pt;
	GetCursorPos(&pt);
	HMENU menu = CreatePopupMenu();
	
	//Find
	InsertMenu(menu, -1, MF_BYPOSITION, SWM_FIND, l10n->menu_find);
	InsertMenu(menu, -1, MF_BYPOSITION, SWM_FINDDELAY, l10n->menu_finddelay);
	InsertMenu(menu, -1, MF_BYPOSITION, SWM_FINDALL, l10n->menu_findall);
	InsertMenu(menu, -1, MF_BYPOSITION|MF_SEPARATOR, 0, NULL);
	
	//About
	InsertMenu(menu, -1, MF_BYPOSITION, SWM_ABOUT, l10n->menu_about);
	
	//Exit
	InsertMenu(menu, -1, MF_BYPOSITION, SWM_EXIT, l10n->menu_exit);
	
	//Track menu
	SetForegroundWindow(hwnd);
	TrackPopupMenu(menu, TPM_BOTTOMALIGN, pt.x, pt.y, 0, hwnd, NULL);
	DestroyMenu(menu);
}
