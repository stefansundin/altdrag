/*
	Autostart functions.
	Copyright (C) 2010  Stefan Sundin (recover89@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

//No error reporting since we don't want the user to be interrupted when opening the tray menu
void CheckAutostart(int *on, int *hidden) {
	*on = 0;
	*hidden = 0;
	//Read registry
	HKEY key;
	wchar_t value[MAX_PATH+10] = L"";
	DWORD len = sizeof(value);
	RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_QUERY_VALUE, &key);
	RegQueryValueEx(key, APP_NAME, NULL, NULL, (LPBYTE)value, &len);
	RegCloseKey(key);
	//Compare
	wchar_t path[MAX_PATH];
	GetModuleFileName(NULL, path, sizeof(path)/sizeof(wchar_t));
	wchar_t compare[MAX_PATH+10];
	swprintf(compare, L"\"%s\"", path);
	if (!wcscmp(compare,value)) {
		*on = 1;
	}
	else {
		swprintf(compare, L"\"%s\" -hide", path);
		if (!wcscmp(compare,value)) {
			*on = 1;
			*hidden = 1;
		}
	}
}

void SetAutostart(int on, int hide) {
	//Open key
	HKEY key;
	int error = RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, 0, KEY_SET_VALUE, NULL, &key, NULL);
	if (error != ERROR_SUCCESS) {
		Error(L"RegCreateKeyEx(HKEY_CURRENT_USER,'Software\\Microsoft\\Windows\\CurrentVersion\\Run')", L"Error opening the registry.", error, TEXT(__FILE__), __LINE__);
		return;
	}
	if (on) {
		//Get path
		wchar_t path[MAX_PATH];
		GetModuleFileName(NULL, path, sizeof(path)/sizeof(wchar_t));
		//Set autostart
		wchar_t value[MAX_PATH+10];
		swprintf(value, (hide?L"\"%s\" -hide":L"\"%s\""), path);
		error = RegSetValueEx(key, APP_NAME, 0, REG_SZ, (LPBYTE)value, (wcslen(value)+1)*sizeof(wchar_t));
		if (error != ERROR_SUCCESS) {
			Error(L"RegSetValueEx('"APP_NAME"')", L"SetAutostart()", error, TEXT(__FILE__), __LINE__);
			return;
		}
	}
	else {
		//Remove
		error = RegDeleteValue(key, APP_NAME);
		if (error != ERROR_SUCCESS) {
			Error(L"RegDeleteValue('"APP_NAME"')", L"SetAutostart()", error, TEXT(__FILE__), __LINE__);
			return;
		}
	}
	//Close key
	RegCloseKey(key);
}
