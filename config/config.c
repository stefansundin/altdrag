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
	struct {
		int AutoFocus;
		int AutoSnap;
		int AutoRemaximize;
		int Aero;
		int InactiveScroll;
		int HookWindows;
	} AltDrag;
	struct {
		int Cursor;
	} Performance;
	struct {
		wchar_t LMB[20];
		wchar_t MMB[20];
		wchar_t RMB[20];
		wchar_t MB4[20];
		wchar_t MB5[20];
	} Mouse;
} settings;

//Cool stuff
BOOL x64 = FALSE;

//Include stuff
#include "localization/strings.h"
#include "../include/error.c"
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
	//[AltDrag]
	GetPrivateProfileString(APP_NAME, L"AutoFocus", L"0", txt, sizeof(txt)/sizeof(wchar_t), path);
	settings.AltDrag.AutoFocus = _wtoi(txt);
	GetPrivateProfileString(APP_NAME, L"AutoSnap", L"0", txt, sizeof(txt)/sizeof(wchar_t), path);
	settings.AltDrag.AutoSnap = _wtoi(txt);
	GetPrivateProfileString(APP_NAME, L"AutoRemaximize", L"1", txt, sizeof(txt)/sizeof(wchar_t), path);
	settings.AltDrag.AutoRemaximize = _wtoi(txt);
	GetPrivateProfileString(APP_NAME, L"Aero", L"2", txt, sizeof(txt)/sizeof(wchar_t), path);
	settings.AltDrag.Aero = _wtoi(txt);
	GetPrivateProfileString(APP_NAME, L"InactiveScroll", L"2", txt, sizeof(txt)/sizeof(wchar_t), path);
	settings.AltDrag.InactiveScroll = _wtoi(txt);
	GetPrivateProfileString(APP_NAME, L"HookWindows", L"0", txt, sizeof(txt)/sizeof(wchar_t), path);
	settings.AltDrag.HookWindows = _wtoi(txt);
	//[Mouse]
	GetPrivateProfileString(L"Mouse", L"LMB", L"Move", settings.Mouse.LMB, sizeof(settings.Mouse.LMB)/sizeof(wchar_t), path);
	GetPrivateProfileString(L"Mouse", L"MMB", L"Move", settings.Mouse.MMB, sizeof(settings.Mouse.MMB)/sizeof(wchar_t), path);
	GetPrivateProfileString(L"Mouse", L"RMB", L"Move", settings.Mouse.RMB, sizeof(settings.Mouse.RMB)/sizeof(wchar_t), path);
	GetPrivateProfileString(L"Mouse", L"MB4", L"Move", settings.Mouse.MB4, sizeof(settings.Mouse.MB4)/sizeof(wchar_t), path);
	GetPrivateProfileString(L"Mouse", L"MB5", L"Move", settings.Mouse.MB5, sizeof(settings.Mouse.MB5)/sizeof(wchar_t), path);
	
	
	GetPrivateProfileString(APP_NAME, L"Language", L"en-US", txt, sizeof(txt)/sizeof(wchar_t), path);
	int i;
	for (i=0; languages[i].code != NULL; i++) {
		if (!wcsicmp(txt,languages[i].code)) {
			l10n = languages[i].strings;
			break;
		}
	}
	//SendMessage(g_hwnd, WM_UPDATESETTINGS, 0, 0);
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
		//[AltDrag]
		WritePrivateProfileString(APP_NAME, L"AutoFocus",      _itow(settings.AltDrag.AutoFocus,txt,10), path);
		WritePrivateProfileString(APP_NAME, L"AutoSnap",       _itow(settings.AltDrag.AutoSnap,txt,10), path);
		WritePrivateProfileString(APP_NAME, L"Aero",           _itow(settings.AltDrag.Aero,txt,10), path);
		WritePrivateProfileString(APP_NAME, L"InactiveScroll", _itow(settings.AltDrag.InactiveScroll,txt,10), path);
		WritePrivateProfileString(APP_NAME, L"HookWindows",    _itow(settings.AltDrag.HookWindows,txt,10), path);
		WritePrivateProfileString(APP_NAME, L"Language",       l10n->code, path);
		
		//[Mouse]
		WritePrivateProfileString(L"Mouse", L"LMB", settings.Mouse.LMB, path);
		WritePrivateProfileString(L"Mouse", L"MMB", settings.Mouse.MMB, path);
		WritePrivateProfileString(L"Mouse", L"RMB", settings.Mouse.RMB, path);
		WritePrivateProfileString(L"Mouse", L"MB4", settings.Mouse.MB4, path);
		WritePrivateProfileString(L"Mouse", L"MB5", settings.Mouse.MB5, path);
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
		SendDlgItemMessage(hwnd, IDC_AUTOFOCUS, BM_SETCHECK, settings.AltDrag.AutoFocus?BST_CHECKED:BST_UNCHECKED, 0);
		
		SendDlgItemMessage(hwnd, IDC_AUTOSNAP, CB_ADDSTRING, 0, (LPARAM)L"Disabled");
		SendDlgItemMessage(hwnd, IDC_AUTOSNAP, CB_ADDSTRING, 0, (LPARAM)L"To screen borders");
		SendDlgItemMessage(hwnd, IDC_AUTOSNAP, CB_ADDSTRING, 0, (LPARAM)L"+ outside of windows");
		SendDlgItemMessage(hwnd, IDC_AUTOSNAP, CB_ADDSTRING, 0, (LPARAM)L"+ inside of windows");
		SendDlgItemMessage(hwnd, IDC_AUTOSNAP, CB_SETCURSEL, settings.AltDrag.AutoSnap, 0);
		
		SendDlgItemMessage(hwnd, IDC_AERO, BM_SETCHECK, settings.AltDrag.Aero?BST_CHECKED:BST_UNCHECKED, 0);
		SendDlgItemMessage(hwnd, IDC_INACTIVESCROLL, BM_SETCHECK, settings.AltDrag.InactiveScroll?BST_CHECKED:BST_UNCHECKED, 0);
		SendDlgItemMessage(hwnd, IDC_HOOKWINDOWS, BM_SETCHECK, settings.AltDrag.HookWindows?BST_CHECKED:BST_UNCHECKED, 0);
		
		int i;
		for (i=0; languages[i].code != NULL; i++) {
			wchar_t txt[20];
			wsprintf(txt, L"%s (%s)", languages[i].language, languages[i].code);
			SendDlgItemMessage(hwnd, IDC_LANGUAGE, CB_ADDSTRING, 0, (LPARAM)txt);
			if (l10n == languages[i].strings) {
				SendDlgItemMessage(hwnd, IDC_LANGUAGE, CB_SETCURSEL, i, 0);
			}
		}
		
		return TRUE;
	}
	else if (msg == WM_NOTIFY) {
		LPNMHDR pnmh = (LPNMHDR)lParam;
		if (pnmh->code == PSN_APPLY) {
			settings.AltDrag.AutoFocus = SendDlgItemMessage(hwnd, IDC_AUTOFOCUS, BM_GETCHECK, 0, 0);
			settings.AltDrag.AutoSnap = SendDlgItemMessage(hwnd, IDC_AUTOSNAP, CB_GETCURSEL, 0, 0);
			int temp = SendDlgItemMessage(hwnd, IDC_AERO, BM_GETCHECK, 0, 0);
			if (temp != !!settings.AltDrag.Aero) { //Don't destroy Aero=2
				settings.AltDrag.Aero = temp;
			}
			settings.AltDrag.InactiveScroll = SendDlgItemMessage(hwnd, IDC_INACTIVESCROLL, BM_GETCHECK, 0, 0);
			settings.AltDrag.HookWindows = SendDlgItemMessage(hwnd, IDC_HOOKWINDOWS, BM_GETCHECK, 0, 0);
			int i = SendDlgItemMessage(hwnd, IDC_LANGUAGE, CB_GETCURSEL, 0, 0);
			l10n = languages[i].strings;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK MousePageDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_INITDIALOG) {
		struct {
			int item;
			wchar_t *setting;
		} items[] = {
			{IDC_LMB, settings.Mouse.LMB},
			{IDC_MMB, settings.Mouse.MMB},
			{IDC_RMB, settings.Mouse.RMB},
			{IDC_MB4, settings.Mouse.MB4},
			{IDC_MB5, settings.Mouse.MB5},
			{-1}
		};
		wchar_t *actions[] = {L"Move", L"Resize", L"Minimize", L"AlwaysOnTop", L"Center", L"Close", L"Nothing", NULL};
		int i, j;
		for (i=0; items[i].item != -1; i++) {
			for (j=0; actions[j] != NULL; j++) {
				SendDlgItemMessage(hwnd, items[i].item, CB_ADDSTRING, 0, (LPARAM)actions[j]);
				if (!wcscmp(items[i].setting,actions[j])) {
					SendDlgItemMessage(hwnd, items[i].item, CB_SETCURSEL, j, 0);
				}
			}
		}
		return TRUE;
	}
	else if (msg == WM_NOTIFY) {
		LPNMHDR pnmh = (LPNMHDR)lParam;
		if (pnmh->code == PSN_APPLY) {
			int i;
			i = SendDlgItemMessage(hwnd, IDC_LMB, CB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_LMB, CB_GETLBTEXT, i, (LPARAM)settings.Mouse.LMB);
			i = SendDlgItemMessage(hwnd, IDC_MMB, CB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_MMB, CB_GETLBTEXT, i, (LPARAM)settings.Mouse.MMB);
			i = SendDlgItemMessage(hwnd, IDC_RMB, CB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_RMB, CB_GETLBTEXT, i, (LPARAM)settings.Mouse.RMB);
			i = SendDlgItemMessage(hwnd, IDC_MB4, CB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_MB4, CB_GETLBTEXT, i, (LPARAM)settings.Mouse.MB4);
			i = SendDlgItemMessage(hwnd, IDC_MB5, CB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_MB5, CB_GETLBTEXT, i, (LPARAM)settings.Mouse.MB5);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK AboutPageDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_INITDIALOG) {
		
		return TRUE;
	}
	else if (msg == WM_COMMAND) {
		if (wParam == IDC_DONATE) {
			ShellExecute(NULL, L"open", L"http://code.google.com/p/altdrag/wiki/Donate", NULL, NULL, SW_SHOWNORMAL);
		}
	}
	return FALSE;
}
