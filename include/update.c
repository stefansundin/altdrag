/*
	Check for update.
	Copyright (C) 2010  Stefan Sundin (recover89@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

#include <wininet.h>

int update = 0;

DWORD WINAPI _CheckForUpdate(LPVOID arg) {
	int verbose = *(int*)arg;
	free(arg);
	
	//Check if we should check for beta
	wchar_t path[MAX_PATH];
	GetModuleFileName(NULL, path, sizeof(path)/sizeof(wchar_t));
	PathRemoveFileSpec(path);
	wcscat(path, L"\\"APP_NAME".ini");
	wchar_t txt[10];
	GetPrivateProfileString(L"Update", L"Beta", L"0", txt, sizeof(txt)/sizeof(wchar_t), path);
	int beta = _wtoi(txt);
	
	//Check if we are connected to the internet
	DWORD flags; //Not really used
	int tries = 0; //Try at least ten times, sleep one second between each attempt
	while (InternetGetConnectedState(&flags,0) == FALSE) {
		tries++;
		if (!verbose) {
			Sleep(1000);
		}
		if (tries >= 10 || verbose) {
			if (verbose) {
				Error(L"InternetGetConnectedState()", L"No internet connection.\nPlease check for update manually at "APP_URL, GetLastError(), TEXT(__FILE__), __LINE__);
			}
			return 1;
		}
	}
	
	//Open connection
	HINTERNET http = InternetOpen(APP_NAME" - "APP_VERSION, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (http == NULL) {
		if (verbose) {
			Error(L"InternetOpen()", L"Could not establish connection.\nPlease check for update manually at "APP_URL, GetLastError(), TEXT(__FILE__), __LINE__);
		}
		return 1;
	}
	HINTERNET file = InternetOpenUrl(http, (beta?APP_UPDATE_UNSTABLE:APP_UPDATE_STABLE), NULL, 0, INTERNET_FLAG_RELOAD|INTERNET_FLAG_NO_CACHE_WRITE|INTERNET_FLAG_NO_AUTH|INTERNET_FLAG_NO_AUTO_REDIRECT|INTERNET_FLAG_NO_COOKIES|INTERNET_FLAG_NO_UI, 0);
	if (file == NULL) {
		if (verbose) {
			Error(L"InternetOpenUrl()", L"Could not establish connection.\nPlease check for update manually at "APP_URL, GetLastError(), TEXT(__FILE__), __LINE__);
		}
		InternetCloseHandle(http);
		return 1;
	}
	//Read file
	char data[20];
	DWORD numread;
	if (InternetReadFile(file,data,sizeof(data),&numread) == FALSE) {
		if (verbose) {
			Error(L"InternetReadFile()", L"Could not read file.\nPlease check for update manually at "APP_URL, GetLastError(), TEXT(__FILE__), __LINE__);
		}
		InternetCloseHandle(file);
		InternetCloseHandle(http);
		return 1;
	}
	data[numread] = '\0';
	//Get error code and mime type
	wchar_t code[4], mime[12];
	DWORD len = sizeof(code);
	HttpQueryInfo(file, HTTP_QUERY_STATUS_CODE, &code, &len, NULL);
	len = sizeof(mime);
	HttpQueryInfo(file, HTTP_QUERY_CONTENT_TYPE, &mime, &len, NULL);
	//Close connection
	InternetCloseHandle(file);
	InternetCloseHandle(http);
	
	//Make sure the server returned code 200 and mime text/plain
	if (wcscmp(code,L"200") || wcscmp(mime,L"text/plain")) {
		if (verbose) {
			MessageBox(NULL, L"Could not determine if an update is available.\n\nPlease check for update manually at "APP_URL, APP_NAME, MB_ICONWARNING|MB_OK);
		}
		return 2;
	}
	
	//New version available?
	if (strcmp(data,APP_VERSION) > 0) {
		update = 1;
		if (verbose) {
			SendMessage(g_hwnd, WM_COMMAND, SWM_UPDATE, 0);
		}
		else {
			wcsncpy(tray.szInfo, l10n->update_balloon, sizeof(tray.szInfo)/sizeof(wchar_t));
			tray.uFlags |= NIF_INFO;
			UpdateTray();
			tray.uFlags ^= NIF_INFO;
		}
	}
	else {
		update = 0;
		if (verbose) {
			MessageBox(NULL, l10n->update_nonew, APP_NAME, MB_ICONINFORMATION|MB_OK);
		}
	}
	return 0;
}

void CheckForUpdate(int p_verbose) {
	int *verbose = malloc(sizeof(p_verbose));
	*verbose = p_verbose;
	HANDLE thread = CreateThread(NULL, 0, _CheckForUpdate, verbose, 0, NULL);
	CloseHandle(thread);
}
