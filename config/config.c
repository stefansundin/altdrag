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
BOOL CALLBACK GeneralPageDialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK MousePageDialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutPageDialogProc(HWND, UINT, WPARAM, LPARAM);
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
	struct {
		int pszTemplate;
		DLGPROC pfnDlgProc;
	} pages[] = {
		{ IDD_GENERALPAGE, GeneralPageDialogProc },
		{ IDD_MOUSEPAGE,   MousePageDialogProc },
		{ IDD_ABOUTPAGE,   AboutPageDialogProc },
	};
	
	PROPSHEETPAGE psp[3] = {0};
	for (i=0; i < sizeof(psp)/sizeof(PROPSHEETPAGE); i++) {
		psp[i].dwSize      = sizeof(PROPSHEETPAGE);
		psp[i].hInstance   = hInst;
		psp[i].pszTemplate = MAKEINTRESOURCE(pages[i].pszTemplate);
		//psp[i].lParam      = (LPARAM)&settings;
		psp[i].pfnDlgProc  = pages[i].pfnDlgProc;
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

BOOL CALLBACK GeneralPageDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_INITDIALOG) {
		HWND item = GetDlgItem(hwnd, IDC_AUTOSNAP);
		SendMessage(item, CB_ADDSTRING, 0, (LPARAM)L"Disabled");
		SendMessage(item, CB_ADDSTRING, 0, (LPARAM)L"To screen borders");
		SendMessage(item, CB_ADDSTRING, 0, (LPARAM)L"+ outside of windows");
		SendMessage(item, CB_ADDSTRING, 0, (LPARAM)L"+ inside of windows");
		SendMessage(item, CB_SETCURSEL, 0, 0);
		
		item = GetDlgItem(hwnd, IDC_LANGUAGE);
		SendMessage(item, CB_ADDSTRING, 0, (LPARAM)L"English (en-US)");
		SendMessage(item, CB_ADDSTRING, 0, (LPARAM)L"Spanish (es-ES)");
		SendMessage(item, CB_ADDSTRING, 0, (LPARAM)L"Galician (gl-ES)");
		SendMessage(item, CB_SETCURSEL, 0, 0);
		
		return TRUE;
	}
	else if (msg == WM_NOTIFY) {
		LPNMHDR pnmh = (LPNMHDR)lParam;
		if (pnmh->code == PSN_APPLY) {
			//Store stuff
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK MousePageDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_INITDIALOG) {
		int items[] = {IDC_LMB, IDC_MMB, IDC_RMB, IDC_MB4, IDC_MB5};
		int i;
		for (i=0; i < sizeof(items)/sizeof(int); i++) {
			HWND item = GetDlgItem(hwnd, items[i]);
			SendMessage(item, CB_ADDSTRING, 0, (LPARAM)L"Move");
			SendMessage(item, CB_ADDSTRING, 0, (LPARAM)L"Resize");
			SendMessage(item, CB_ADDSTRING, 0, (LPARAM)L"Minimize");
			SendMessage(item, CB_ADDSTRING, 0, (LPARAM)L"AlwaysOnTop");
			SendMessage(item, CB_ADDSTRING, 0, (LPARAM)L"Center");
			SendMessage(item, CB_ADDSTRING, 0, (LPARAM)L"Close");
			SendMessage(item, CB_ADDSTRING, 0, (LPARAM)L"Nothing");
			SendMessage(item, CB_SETCURSEL, 0, 0);
		}
		return TRUE;
	}
	else if (msg == WM_NOTIFY) {
		LPNMHDR pnmh = (LPNMHDR)lParam;
		if (pnmh->code == PSN_APPLY) {
			//Store stuff
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK AboutPageDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_INITDIALOG) {
		
		return TRUE;
	}
	return FALSE;
}
