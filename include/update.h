/*
	Check for update
	Copyright (C) 2009  Stefan Sundin (recover89@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

#include <wininet.h>

int update = 0;

DWORD WINAPI _CheckForUpdate() {
	//Check if we are connected to the internet
	DWORD flags; //Not used
	int tries = 0; //Try at least ten times, sleep one second between each attempt
	while (InternetGetConnectedState(&flags,0) == FALSE) {
		tries++;
		Sleep(1000);
		if (tries >= 10) {
			#ifdef DEBUG
			Error(L"InternetGetConnectedState()", L"No internet connection.\nPlease check for update manually at "APP_URL, GetLastError(), __LINE__);
			#endif
			return 1;
		}
	}
	
	//Open connection
	HINTERNET http, file;
	http = InternetOpen(APP_NAME" - "APP_VERSION,INTERNET_OPEN_TYPE_DIRECT,NULL,NULL,0);
	if (http == NULL) {
		#ifdef DEBUG
		Error(L"InternetOpen()", L"Could not establish connection.\nPlease check for update manually at "APP_URL, GetLastError(), __LINE__);
		#endif
		return 1;
	}
	file = InternetOpenUrl(http,APP_UPDATEURL,NULL,0,INTERNET_FLAG_NO_AUTH|INTERNET_FLAG_NO_AUTO_REDIRECT|INTERNET_FLAG_NO_CACHE_WRITE|INTERNET_FLAG_NO_COOKIES|INTERNET_FLAG_NO_UI,0);
	if (file == NULL) {
		#ifdef DEBUG
		Error(L"InternetOpenUrl()", L"Could not establish connection.\nPlease check for update manually at "APP_URL, GetLastError(), __LINE__);
		#endif
		return 1;
	}
	//Read file
	char data[20];
	DWORD numread;
	if (InternetReadFile(file,data,sizeof(data),&numread) == FALSE) {
		#ifdef DEBUG
		Error(L"InternetReadFile()", L"Could not read file.\nPlease check for update manually at "APP_URL, GetLastError(), __LINE__);
		#endif
		return 1;
	}
	data[numread] = '\0';
	//Get error code
	wchar_t code[4];
	DWORD len = sizeof(code);
	HttpQueryInfo(file, HTTP_QUERY_STATUS_CODE, &code, &len, NULL);
	//Close connection
	InternetCloseHandle(file);
	InternetCloseHandle(http);
	
	//Make sure the server returned 200
	if (wcscmp(code,L"200")) {
		#ifdef DEBUG
		swprintf(txt, L"Server returned %s error when checking for update.\nPlease check for update manually at "APP_URL, code);
		MessageBox(NULL, txt, APP_NAME, MB_ICONWARNING|MB_OK);
		#endif
		return 2;
	}
	
	//New version available?
	if (strcmp(data,APP_VERSION) > 0) {
		update = 1;
		wcsncpy(traydata.szInfo, l10n->update_balloon, sizeof(traydata.szInfo)/sizeof(wchar_t));
		traydata.uFlags |= NIF_INFO;
		UpdateTray();
		traydata.uFlags ^= NIF_INFO;
	}
	return 0;
}

void CheckForUpdate() {
	CreateThread(NULL, 0, _CheckForUpdate, NULL, 0, NULL);
}
