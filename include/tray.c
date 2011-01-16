/*
	Tray functions.
	Copyright (C) 2011  Stefan Sundin (recover89@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

NOTIFYICONDATA tray;
HICON icon[2];
int tray_added = 0;
int hide = 0;
extern int update;

int InitTray() {
	//Load icons
	icon[0] = LoadImage(g_hinst, L"tray_disabled", IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	icon[1] = LoadImage(g_hinst, L"tray_enabled", IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	if (icon[0] == NULL || icon[1] == NULL) {
		Error(L"LoadImage('tray_*')", L"Could not load tray icons.", GetLastError(), TEXT(__FILE__), __LINE__);
		return 1;
	}
	
	//Create icondata
	tray.cbSize = sizeof(NOTIFYICONDATA);
	tray.uID = 0;
	tray.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP;
	tray.hWnd = g_hwnd;
	tray.uCallbackMessage = WM_TRAY;
	//Balloon tooltip
	tray.uTimeout = 10000;
	wcsncpy(tray.szInfoTitle, APP_NAME, sizeof(tray.szInfoTitle)/sizeof(wchar_t));
	tray.dwInfoFlags = NIIF_USER;
	
	//Register TaskbarCreated so we can re-add the tray icon if (when) explorer.exe crashes
	WM_TASKBARCREATED = RegisterWindowMessage(L"TaskbarCreated");
	
	return 0;
}

int UpdateTray() {
	wcsncpy(tray.szTip, (enabled()?l10n->tray_enabled:l10n->tray_disabled), sizeof(tray.szTip)/sizeof(wchar_t));
	tray.hIcon = icon[enabled()?1:0];
	
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
	
	//Toggle
	InsertMenu(menu, -1, MF_BYPOSITION, SWM_TOGGLE, (enabled()?l10n->menu_disable:l10n->menu_enable));
	
	//Hide
	InsertMenu(menu, -1, MF_BYPOSITION, SWM_HIDE, l10n->menu_hide);
	
	//Check autostart
	int autostart=0, hidden=0;
	CheckAutostart(&autostart, &hidden);
	//Options
	HMENU menu_options = CreatePopupMenu();
	InsertMenu(menu_options, -1, MF_BYPOSITION|(autostart?MF_CHECKED:0), (autostart?SWM_AUTOSTART_OFF:SWM_AUTOSTART_ON), l10n->menu_autostart);
	InsertMenu(menu_options, -1, MF_BYPOSITION|(hidden?MF_CHECKED:0), (hidden?SWM_AUTOSTART_HIDE_OFF:SWM_AUTOSTART_HIDE_ON), l10n->menu_hide);
	InsertMenu(menu_options, -1, MF_BYPOSITION|MF_SEPARATOR, 0, NULL);
	InsertMenu(menu_options, -1, MF_BYPOSITION, SWM_SETTINGS, l10n->menu_settings);
	InsertMenu(menu_options, -1, MF_BYPOSITION, SWM_CHECKFORUPDATE, l10n->menu_chkupdate);
	InsertMenu(menu, -1, MF_BYPOSITION|MF_POPUP, (UINT_PTR)menu_options, l10n->menu_options);
	InsertMenu(menu, -1, MF_BYPOSITION|MF_SEPARATOR, 0, NULL);
	
	//Update
	if (update) {
		InsertMenu(menu, -1, MF_BYPOSITION, SWM_UPDATE, l10n->menu_update);
	}
	
	//About
	InsertMenu(menu, -1, MF_BYPOSITION, SWM_ABOUT, l10n->menu_about);
	
	//Exit
	InsertMenu(menu, -1, MF_BYPOSITION, SWM_EXIT, l10n->menu_exit);

	//Track menu
	SetForegroundWindow(hwnd);
	TrackPopupMenu(menu, TPM_BOTTOMALIGN, pt.x, pt.y, 0, hwnd, NULL);
	DestroyMenu(menu);
}
