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
BOOL CALLBACK InputPageDialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK BlacklistPageDialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AdvancedPageDialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutPageDialogProc(HWND, UINT, WPARAM, LPARAM);
UINT WM_UPDATESETTINGS = 0;
UINT WM_ADDTRAY = 0;
UINT WM_HIDETRAY = 0;
wchar_t inipath[MAX_PATH];

//Cool stuff
#define MAXKEYS 10
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
	struct {
		int LeftAlt;
		int RightAlt;
		int LeftWinkey;
		int RightWinkey;
		int LeftCtrl;
		int RightCtrl;
		wchar_t OtherKeys[30];
	} Keyboard;
} settings;
BOOL x64 = FALSE;

//Include stuff
#include "localization/strings.h"
#include "../include/error.c"
#include "window.h"

//Entry point
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
	//Look for instance
	WM_UPDATESETTINGS = RegisterWindowMessage(L"UpdateSettings");
	WM_ADDTRAY = RegisterWindowMessage(L"AddTray");
	WM_HIDETRAY = RegisterWindowMessage(L"HideTray");
	
	//Load settings
	GetModuleFileName(NULL, inipath, sizeof(inipath)/sizeof(wchar_t));
	PathRemoveFileSpec(inipath);
	wcscat(inipath, L"\\"APP_NAME".ini");
	wchar_t txt[1000];
	//[AltDrag]
	GetPrivateProfileString(APP_NAME, L"AutoFocus", L"0", txt, sizeof(txt)/sizeof(wchar_t), inipath);
	settings.AltDrag.AutoFocus = _wtoi(txt);
	GetPrivateProfileString(APP_NAME, L"AutoSnap", L"0", txt, sizeof(txt)/sizeof(wchar_t), inipath);
	settings.AltDrag.AutoSnap = _wtoi(txt);
	GetPrivateProfileString(APP_NAME, L"AutoRemaximize", L"1", txt, sizeof(txt)/sizeof(wchar_t), inipath);
	settings.AltDrag.AutoRemaximize = _wtoi(txt);
	GetPrivateProfileString(APP_NAME, L"Aero", L"2", txt, sizeof(txt)/sizeof(wchar_t), inipath);
	settings.AltDrag.Aero = _wtoi(txt);
	GetPrivateProfileString(APP_NAME, L"InactiveScroll", L"1", txt, sizeof(txt)/sizeof(wchar_t), inipath);
	settings.AltDrag.InactiveScroll = _wtoi(txt);
	GetPrivateProfileString(APP_NAME, L"HookWindows", L"0", txt, sizeof(txt)/sizeof(wchar_t), inipath);
	settings.AltDrag.HookWindows = _wtoi(txt);
	//[Mouse]
	GetPrivateProfileString(L"Mouse", L"LMB", L"Move", settings.Mouse.LMB, sizeof(settings.Mouse.LMB)/sizeof(wchar_t), inipath);
	GetPrivateProfileString(L"Mouse", L"MMB", L"Move", settings.Mouse.MMB, sizeof(settings.Mouse.MMB)/sizeof(wchar_t), inipath);
	GetPrivateProfileString(L"Mouse", L"RMB", L"Move", settings.Mouse.RMB, sizeof(settings.Mouse.RMB)/sizeof(wchar_t), inipath);
	GetPrivateProfileString(L"Mouse", L"MB4", L"Move", settings.Mouse.MB4, sizeof(settings.Mouse.MB4)/sizeof(wchar_t), inipath);
	GetPrivateProfileString(L"Mouse", L"MB5", L"Move", settings.Mouse.MB5, sizeof(settings.Mouse.MB5)/sizeof(wchar_t), inipath);
	//[Keyboard]
	settings.Keyboard.LeftAlt = 0;
	settings.Keyboard.RightAlt = 0;
	settings.Keyboard.LeftWinkey = 0;
	settings.Keyboard.RightWinkey = 0;
	settings.Keyboard.LeftCtrl = 0;
	settings.Keyboard.RightCtrl = 0;
	settings.Keyboard.OtherKeys[0] = '\0';
	unsigned char temp;
	int numread;
	GetPrivateProfileString(L"Keyboard", L"Hotkeys", L"A4 A5", txt, sizeof(txt)/sizeof(wchar_t), inipath);
	wchar_t *pos = txt;
	while (*pos != '\0' && swscanf(pos,L"%02X%n",&temp,&numread) != EOF) {
		//Bail if we are out of space
		if (wcslen(settings.Keyboard.OtherKeys) > 25) {
			break;
		}
		//Store key
		pos += numread;
		//What key was that?
		if      (temp == VK_LMENU)    settings.Keyboard.LeftAlt = 1;
		else if (temp == VK_RMENU)    settings.Keyboard.RightAlt = 1;
		else if (temp == VK_LWIN)     settings.Keyboard.LeftWinkey = 1;
		else if (temp == VK_RWIN)     settings.Keyboard.RightWinkey = 1;
		else if (temp == VK_LCONTROL) settings.Keyboard.LeftCtrl = 1;
		else if (temp == VK_RCONTROL) settings.Keyboard.RightCtrl = 1;
		else swprintf(settings.Keyboard.OtherKeys, L"%s %02X", settings.Keyboard.OtherKeys, temp);
	}
	//Language
	GetPrivateProfileString(APP_NAME, L"Language", L"en-US", txt, sizeof(txt)/sizeof(wchar_t), inipath);
	int i;
	for (i=0; languages[i].code != NULL; i++) {
		if (!wcsicmp(txt,languages[i].code)) {
			l10n = languages[i].strings;
			break;
		}
	}
	IsWow64Process(GetCurrentProcess(), &x64);
	
	//Define the pages
	struct {
		int pszTemplate;
		DLGPROC pfnDlgProc;
	} pages[] = {
		{ IDD_GENERALPAGE,   GeneralPageDialogProc },
		{ IDD_INPUTPAGE,     InputPageDialogProc },
		{ IDD_BLACKLISTPAGE, BlacklistPageDialogProc },
		{ IDD_ADVANCEDPAGE,  AdvancedPageDialogProc },
		{ IDD_ABOUTPAGE,     AboutPageDialogProc },
	};
	#define NUMPAGES sizeof(pages)/sizeof(pages[0])
	
	PROPSHEETPAGE psp[NUMPAGES] = {0};
	for (i=0; i < NUMPAGES; i++) {
		psp[i].dwSize      = sizeof(PROPSHEETPAGE);
		psp[i].hInstance   = hInst;
		psp[i].pszTemplate = MAKEINTRESOURCE(pages[i].pszTemplate);
		//psp[i].lParam      = (LPARAM)&settings;
		psp[i].pfnDlgProc  = pages[i].pfnDlgProc;
	}
	
	//Define the property sheet
	PROPSHEETHEADER psh = {0};
	psh.dwSize          = sizeof(PROPSHEETHEADER);
	psh.dwFlags         = PSH_USEICONID | PSH_PROPSHEETPAGE | PSH_USECALLBACK | PSH_NOAPPLYNOW | PSH_NOCONTEXTHELP;
	psh.hwndParent      = NULL;
	psh.hInstance       = hInst;
	psh.pszIcon         = MAKEINTRESOURCE(IDI_ICON1);
	psh.pszCaption      = APP_NAME L" Configuration";
	psh.nPages          = NUMPAGES;
	psh.ppsp            = (LPCPROPSHEETPAGE)&psp;
	psh.pfnCallback     = PropSheetProc;
	
	//Open the property sheet
	if (PropertySheet(&psh)) {
		//[AltDrag]
		WritePrivateProfileString(APP_NAME, L"AutoFocus",      _itow(settings.AltDrag.AutoFocus,txt,10), inipath);
		WritePrivateProfileString(APP_NAME, L"AutoSnap",       _itow(settings.AltDrag.AutoSnap,txt,10), inipath);
		WritePrivateProfileString(APP_NAME, L"Aero",           _itow(settings.AltDrag.Aero,txt,10), inipath);
		WritePrivateProfileString(APP_NAME, L"InactiveScroll", _itow(settings.AltDrag.InactiveScroll,txt,10), inipath);
		WritePrivateProfileString(APP_NAME, L"HookWindows",    _itow(settings.AltDrag.HookWindows,txt,10), inipath);
		WritePrivateProfileString(APP_NAME, L"Language",       l10n->code, inipath);
		
		//[Mouse]
		WritePrivateProfileString(L"Mouse", L"LMB", settings.Mouse.LMB, inipath);
		WritePrivateProfileString(L"Mouse", L"MMB", settings.Mouse.MMB, inipath);
		WritePrivateProfileString(L"Mouse", L"RMB", settings.Mouse.RMB, inipath);
		WritePrivateProfileString(L"Mouse", L"MB4", settings.Mouse.MB4, inipath);
		WritePrivateProfileString(L"Mouse", L"MB5", settings.Mouse.MB5, inipath);
		
		//[Keyboard]
		txt[0] = '\0';
		if (settings.Keyboard.LeftAlt)     swprintf(txt, L" %02X",         VK_LMENU);
		if (settings.Keyboard.RightAlt)    swprintf(txt, L"%s %02X", txt, VK_RMENU);
		if (settings.Keyboard.LeftWinkey)  swprintf(txt, L"%s %02X", txt, VK_LWIN);
		if (settings.Keyboard.RightWinkey) swprintf(txt, L"%s %02X", txt, VK_RWIN);
		if (settings.Keyboard.LeftCtrl)    swprintf(txt, L"%s %02X", txt, VK_LCONTROL);
		if (settings.Keyboard.RightCtrl)   swprintf(txt, L"%s %02X", txt, VK_RCONTROL);
		wcscat(txt, settings.Keyboard.OtherKeys);
		WritePrivateProfileString(L"Keyboard", L"Hotkeys", txt+1, inipath); //Skip prefix space
		
		//Make AltDrag update its settings
		HWND inst = FindWindow(APP_NAME, NULL);
		if (inst != NULL) {
			PostMessage(inst, WM_UPDATESETTINGS, 0, 0);
		}
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
			int i = SendDlgItemMessage(hwnd, IDC_LANGUAGE, CB_GETCURSEL, 0, 0);
			l10n = languages[i].strings;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK InputPageDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_INITDIALOG) {
		//Mouse actions
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
		wchar_t *actions[] = {L"Move", L"Resize", L"Close", L"Minimize", L"Lower", L"AlwaysOnTop", L"Center", L"Nothing", NULL};
		int i, j;
		for (i=0; items[i].item != -1; i++) {
			for (j=0; actions[j] != NULL; j++) {
				SendDlgItemMessage(hwnd, items[i].item, CB_ADDSTRING, 0, (LPARAM)actions[j]);
				if (!wcscmp(items[i].setting,actions[j])) {
					SendDlgItemMessage(hwnd, items[i].item, CB_SETCURSEL, j, 0);
				}
			}
		}
		
		//Hotkeys
		if (settings.Keyboard.LeftAlt)     SendDlgItemMessage(hwnd, IDC_LEFTALT, BM_SETCHECK, BST_CHECKED, 0);
		if (settings.Keyboard.RightAlt)    SendDlgItemMessage(hwnd, IDC_RIGHTALT, BM_SETCHECK, BST_CHECKED, 0);
		if (settings.Keyboard.LeftWinkey)  SendDlgItemMessage(hwnd, IDC_LEFTWINKEY, BM_SETCHECK, BST_CHECKED, 0);
		if (settings.Keyboard.RightWinkey) SendDlgItemMessage(hwnd, IDC_RIGHTWINKEY, BM_SETCHECK, BST_CHECKED, 0);
		if (settings.Keyboard.LeftCtrl)    SendDlgItemMessage(hwnd, IDC_LEFTCTRL, BM_SETCHECK, BST_CHECKED, 0);
		if (settings.Keyboard.RightCtrl)   SendDlgItemMessage(hwnd, IDC_RIGHTCTRL, BM_SETCHECK, BST_CHECKED, 0);
		
		return TRUE;
	}
	else if (msg == WM_NOTIFY) {
		LPNMHDR pnmh = (LPNMHDR)lParam;
		if (pnmh->code == PSN_APPLY) {
			//Mouse actions
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
			
			//Hotkeys
			settings.Keyboard.LeftAlt     = SendDlgItemMessage(hwnd, IDC_LEFTALT, BM_GETCHECK, 0, 0);
			settings.Keyboard.RightAlt    = SendDlgItemMessage(hwnd, IDC_RIGHTALT, BM_GETCHECK, 0, 0);
			settings.Keyboard.LeftWinkey  = SendDlgItemMessage(hwnd, IDC_LEFTWINKEY, BM_GETCHECK, 0, 0);
			settings.Keyboard.RightWinkey = SendDlgItemMessage(hwnd, IDC_RIGHTWINKEY, BM_GETCHECK, 0, 0);
			settings.Keyboard.LeftCtrl    = SendDlgItemMessage(hwnd, IDC_LEFTCTRL, BM_GETCHECK, 0, 0);
			settings.Keyboard.RightCtrl   = SendDlgItemMessage(hwnd, IDC_RIGHTCTRL, BM_GETCHECK, 0, 0);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK BlacklistPageDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
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

BOOL CALLBACK AdvancedPageDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_INITDIALOG) {
		
		SendDlgItemMessage(hwnd, IDC_HOOKWINDOWS, BM_SETCHECK, settings.AltDrag.HookWindows?BST_CHECKED:BST_UNCHECKED, 0);
		return TRUE;
	}
	else if (msg == WM_NOTIFY) {
		LPNMHDR pnmh = (LPNMHDR)lParam;
		if (pnmh->code == PSN_APPLY) {
			settings.AltDrag.HookWindows = SendDlgItemMessage(hwnd, IDC_HOOKWINDOWS, BM_GETCHECK, 0, 0);
		}
	}
	else if (msg == WM_COMMAND) {
		if (wParam == IDC_OPENINI) {
			ShellExecute(NULL, L"open", inipath, NULL, NULL, SW_SHOWNORMAL);
		}
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
