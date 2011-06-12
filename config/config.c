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
#include <prsht.h>

//App
#define APP_NAME            L"AltDrag"
#define APP_VERSION         "0.9"
#define APP_URL             L"http://code.google.com/p/altdrag/"
#define APP_UPDATE_STABLE   L"http://altdrag.googlecode.com/svn/wiki/latest-stable.txt"
#define APP_UPDATE_UNSTABLE L"http://altdrag.googlecode.com/svn/wiki/latest-unstable.txt"

//Boring stuff
BOOL CALLBACK PropSheetProc(HWND, UINT, LPARAM);
BOOL CALLBACK DialogProcOptions(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hinst = NULL;
HWND g_hwnd = NULL;
UINT WM_UPDATESETTINGS = 0;
UINT WM_ADDTRAY = 0;
UINT WM_HIDETRAY = 0;
struct {
	int InactiveScroll;
	int HookWindows;
} settings = {0, 0};

//Cool stuff
BOOL x64 = FALSE;

//Include stuff
#include "../localization/strings.h"
#include "../include/error.c"
#include "../include/autostart.c"
//#include "../include/update.c"
#include "window.h"

//Entry point
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
	g_hinst = hInst;
	
	//Look for instance
	WM_UPDATESETTINGS = RegisterWindowMessage(L"UpdateSettings");
	WM_ADDTRAY = RegisterWindowMessage(L"AddTray");
	WM_HIDETRAY = RegisterWindowMessage(L"HideTray");
	
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
	
	// Define the property pages...
	PROPSHEETPAGE psp[3] = {0};
	for (i = 0; i < sizeof(psp)/sizeof(PROPSHEETPAGE); i++) {
		psp[i].dwSize      = sizeof(PROPSHEETPAGE);
		psp[i].hInstance   = hInst;
		psp[i].pszTemplate = MAKEINTRESOURCE(IDD_PROPPAGE1+i);
		//psp[i].lParam      = (LPARAM)&settings;
		psp[i].pfnDlgProc  = DialogProcOptions;
	}
	// Define the property sheet...
	PROPSHEETHEADER psh = {0};
	psh.dwSize          = sizeof(PROPSHEETHEADER);
	psh.dwFlags         = PSH_USEICONID | PSH_PROPSHEETPAGE | PSH_USECALLBACK | PSH_NOAPPLYNOW | PSH_NOCONTEXTHELP;
	psh.hwndParent      = NULL;
	psh.hInstance       = hInst;
	psh.pszIcon         = MAKEINTRESOURCE(IDI_ICON1);
	psh.pszCaption      = APP_NAME L" Configuration";
	psh.nPages          = sizeof(psp)/sizeof(PROPSHEETPAGE);
	psh.ppsp            = (LPCPROPSHEETPAGE)&psp;
	psh.pfnCallback     = PropSheetProc;
	if (PropertySheet(&psh)) {
		//SaveSettings(&settings);
	}
	
	return 0;
}

BOOL CALLBACK PropSheetProc(HWND hwnd, UINT msg, LPARAM lParam) {
	if (msg == PSCB_INITIALIZED) {
		LOGFONT lf = {0};
		lf.lfCharSet = DEFAULT_CHARSET;
		lf.lfHeight = 13;
		lstrcpy(lf.lfFaceName, TEXT("Tahoma"));
		HFONT hFont = CreateFontIndirect(&lf);
		SendMessage(PropSheet_GetTabControl(hwnd), WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(GetDlgItem(hwnd,IDOK),         WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(GetDlgItem(hwnd,IDCANCEL),     WM_SETFONT, (WPARAM)hFont, TRUE);
		return TRUE;
	}
	return 0;
}

BOOL CALLBACK DialogProcOptions(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	//static PSETTINGS pSettings;
	if (msg == WM_INITDIALOG) {
		//pSettings = (PSETTINGS)((LPPROPSHEETPAGE)lParam)->lParam;
		return TRUE;
	}
	else if (msg == WM_NOTIFY) {
		int index = PropSheet_HwndToIndex(GetParent(hwnd), hwnd);
		switch (((LPNMHDR)lParam)->code) {
			case PSN_SETACTIVE:
				switch (index) {
					case 0:
						//SetWindowText(GetDlgItem(hwnd,IDC_EDIT1), pSettings->szEdit1);
						break;
					case 1:
						//SetWindowText(GetDlgItem(hwnd,IDC_EDIT2), pSettings->szEdit2);
						break;
					case 2:
						//SetWindowText(GetDlgItem(hwnd,IDC_EDIT3), pSettings->szEdit3);
						break;
				}
				break;
			case PSN_APPLY:
				switch (index) {
					case 0:
						//GetWindowText(GetDlgItem(hwnd,IDC_EDIT1), pSettings->szEdit1, sizeof(pSettings->szEdit1)/sizeof(TCHAR));
						break;
					case 1:
						//GetWindowText(GetDlgItem(hwnd,IDC_EDIT2), pSettings->szEdit2, sizeof(pSettings->szEdit2)/sizeof(TCHAR));
						break;
					case 2:
						//GetWindowText(GetDlgItem(hwnd,IDC_EDIT3), pSettings->szEdit3, sizeof(pSettings->szEdit3)/sizeof(TCHAR));
						break;
				}
				break;
		}
		return TRUE;
	}
	return FALSE;
}
