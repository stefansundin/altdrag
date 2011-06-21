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
#include <commctrl.h>
#include <prsht.h>
#include <windowsx.h>

//App
#define APP_NAME            L"AltDrag"
#define APP_VERSION         "0.9"
#define APP_URL             L"http://code.google.com/p/altdrag/"
#define APP_CONFIG

//Boring stuff
BOOL CALLBACK PropSheetProc(HWND, UINT, LPARAM);
BOOL CALLBACK GeneralPageDialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK InputPageDialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK BlacklistPageDialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AdvancedPageDialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutPageDialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK LinkProc(HWND, UINT, WPARAM, LPARAM);
UINT WM_UPDATESETTINGS = 0;
UINT WM_ADDTRAY = 0;
UINT WM_HIDETRAY = 0;
HINSTANCE g_hinst = NULL;
HWND g_hwnd = NULL;
wchar_t inipath[MAX_PATH];

//Blacklist
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

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
	struct {
		wchar_t ProcessBlacklist[1000];
		wchar_t Blacklist[1000];
		wchar_t Snaplist[1000];
	} Blacklist;
} settings;
BOOL x64 = FALSE;
int startpage = -1;

//Include stuff
#include "localization/strings.h"
#include "../include/error.c"
#include "../include/autostart.c"
#include "window.h"

//Entry point
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
	g_hinst = hInst;
	
	//Check command line
	if (szCmdLine[0] != '\0') {
		startpage = atoi(szCmdLine);
	}
	
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
	//[Blacklist]
	GetPrivateProfileString(L"Blacklist", L"ProcessBlacklist", L"", settings.Blacklist.ProcessBlacklist, sizeof(settings.Blacklist.ProcessBlacklist)/sizeof(wchar_t), inipath);
	GetPrivateProfileString(L"Blacklist", L"Blacklist", L"", settings.Blacklist.Blacklist, sizeof(settings.Blacklist.Blacklist)/sizeof(wchar_t), inipath);
	GetPrivateProfileString(L"Blacklist", L"Snaplist", L"", settings.Blacklist.Snaplist, sizeof(settings.Blacklist.Snaplist)/sizeof(wchar_t), inipath);
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
	
	PROPSHEETPAGE psp[sizeof(pages)/sizeof(pages[0])] = {0};
	for (i=0; i < sizeof(pages)/sizeof(pages[0]); i++) {
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
	psh.nPages          = sizeof(pages)/sizeof(pages[0]);
	psh.ppsp            = (LPCPROPSHEETPAGE)&psp;
	psh.pfnCallback     = PropSheetProc;
	
	//Open the property sheet
	PropertySheet(&psh);
	
	return 0;
}

void UpdateSettings() {
	//Make AltDrag update its settings
	HWND inst = FindWindow(APP_NAME, NULL);
	if (inst != NULL) {
		PostMessage(inst, WM_UPDATESETTINGS, 0, 0);
	}
}

BOOL CALLBACK PropSheetProc(HWND hwnd, UINT msg, LPARAM lParam) {
	if (msg == PSCB_INITIALIZED) {
		g_hwnd = hwnd;
		
		//OK button replaces Cancel button
		SendMessage(g_hwnd, PSM_CANCELTOCLOSE, 0, 0);
		EnableWindow(GetDlgItem(g_hwnd,IDCANCEL), TRUE); //Re-enable to enable escape key
		WINDOWPLACEMENT wndpl;
		wndpl.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(GetDlgItem(g_hwnd,IDCANCEL), &wndpl);
		SetWindowPlacement(GetDlgItem(g_hwnd,IDOK), &wndpl);
		ShowWindow(GetDlgItem(g_hwnd,IDCANCEL), SW_HIDE);
	}
}

BOOL CALLBACK GeneralPageDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	int updatel10n = 0;
	if (msg == WM_INITDIALOG) {
		SendDlgItemMessage(hwnd, IDC_AUTOFOCUS, BM_SETCHECK, settings.AltDrag.AutoFocus?BST_CHECKED:BST_UNCHECKED, 0);
		
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
		
		//Set startpage (this doesn't work in PSCB_INITIALIZED)
		if (startpage != -1) {
			PropSheet_SetCurSel(g_hwnd, NULL, startpage);
		}
	}
	else if (msg == WM_COMMAND) {
		wchar_t txt[1000];
		if (wParam == IDC_AUTOFOCUS) {
			settings.AltDrag.AutoFocus = SendDlgItemMessage(hwnd, IDC_AUTOFOCUS, BM_GETCHECK, 0, 0);
			WritePrivateProfileString(APP_NAME, L"AutoFocus", _itow(settings.AltDrag.AutoFocus,txt,10), inipath);
		}
		else if (LOWORD(wParam) == IDC_AUTOSNAP && HIWORD(wParam) == CBN_SELCHANGE) {
			settings.AltDrag.AutoSnap = SendDlgItemMessage(hwnd, IDC_AUTOSNAP, CB_GETCURSEL, 0, 0);
			WritePrivateProfileString(APP_NAME, L"AutoSnap", _itow(settings.AltDrag.AutoSnap,txt,10), inipath);
		}
		else if (wParam == IDC_AERO) {
			int temp = SendDlgItemMessage(hwnd, IDC_AERO, BM_GETCHECK, 0, 0);
			if (temp != !!settings.AltDrag.Aero) { //Don't destroy Aero=2
				settings.AltDrag.Aero = temp;
			}
			WritePrivateProfileString(APP_NAME, L"Aero", _itow(settings.AltDrag.Aero,txt,10), inipath);
		}
		else if (wParam == IDC_INACTIVESCROLL) {
			settings.AltDrag.InactiveScroll = SendDlgItemMessage(hwnd, IDC_INACTIVESCROLL, BM_GETCHECK, 0, 0);
			WritePrivateProfileString(APP_NAME, L"InactiveScroll", _itow(settings.AltDrag.InactiveScroll,txt,10), inipath);
		}
		else if (LOWORD(wParam) == IDC_LANGUAGE && HIWORD(wParam) == CBN_SELCHANGE) {
			int i = SendDlgItemMessage(hwnd, IDC_LANGUAGE, CB_GETCURSEL, 0, 0);
			l10n = languages[i].strings;
			WritePrivateProfileString(APP_NAME, L"Language", l10n->code, inipath);
			updatel10n = 1;
		}
		else if (wParam == IDC_AUTOSTART) {
			int autostart = SendDlgItemMessage(hwnd, IDC_AUTOSTART, BM_GETCHECK, 0, 0);
			SetAutostart(autostart, 0);
			EnableWindow(GetDlgItem(hwnd,IDC_AUTOSTART_HIDE), autostart);
			if (!autostart) {
				SendDlgItemMessage(hwnd, IDC_AUTOSTART_HIDE, BM_SETCHECK, BST_UNCHECKED, 0);
			}
		}
		else if (wParam == IDC_AUTOSTART_HIDE) {
			int hidden = SendDlgItemMessage(hwnd, IDC_AUTOSTART_HIDE, BM_GETCHECK, 0, 0);
			SetAutostart(1, hidden);
		}
		UpdateSettings();
	}
	else if (msg == WM_NOTIFY) {
		LPNMHDR pnmh = (LPNMHDR)lParam;
		if (pnmh->code == PSN_SETACTIVE) {
			updatel10n = 1;
			
			//Autostart
			int autostart=0, hidden=0;
			CheckAutostart(&autostart, &hidden);
			SendDlgItemMessage(hwnd, IDC_AUTOSTART, BM_SETCHECK, autostart?BST_CHECKED:BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDC_AUTOSTART_HIDE, BM_SETCHECK, hidden?BST_CHECKED:BST_UNCHECKED, 0);
			EnableWindow(GetDlgItem(hwnd,IDC_AUTOSTART_HIDE), autostart);
		}
	}
	if (updatel10n) {
		//Update window title
		PropSheet_SetTitle(g_hwnd, 0, l10n->title);
		
		//Update tab titles
		HWND tc = PropSheet_GetTabControl(g_hwnd);
		wchar_t *titles[] = { l10n->tab_general, l10n->tab_input, l10n->tab_blacklist, l10n->tab_advanced, l10n->tab_about };
		int i;
		for (i=0; i < sizeof(titles)/sizeof(titles[0]); i++) {
			TCITEM ti;
			ti.mask = TCIF_TEXT;
			ti.pszText = titles[i];
			TabCtrl_SetItem(tc, i, &ti);
		}
		
		//Update text
		SetDlgItemText(hwnd, IDC_AUTOFOCUS, l10n->general_autofocus);
		SetDlgItemText(hwnd, IDC_AERO, l10n->general_aero);
		SetDlgItemText(hwnd, IDC_INACTIVESCROLL, l10n->general_inactivescroll);
		SetDlgItemText(hwnd, IDC_AUTOSNAP_HEADER, l10n->general_autosnap);
		SetDlgItemText(hwnd, IDC_LANGUAGE_HEADER, l10n->general_language);
		SetDlgItemText(hwnd, IDC_SUBMITTRANSLATION, l10n->general_submittranslation);
		SetDlgItemText(hwnd, IDC_AUTOSTART_HEADER, l10n->general_autostart_header);
		SetDlgItemText(hwnd, IDC_AUTOSTART, l10n->general_autostart);
		SetDlgItemText(hwnd, IDC_AUTOSTART_HIDE, l10n->general_autostart_hide);
		SetDlgItemText(hwnd, IDC_AUTOSAVE, l10n->general_autosave);
		
		SendDlgItemMessage(hwnd, IDC_AUTOSNAP, CB_RESETCONTENT, 0, 0);
		SendDlgItemMessage(hwnd, IDC_AUTOSNAP, CB_ADDSTRING, 0, (LPARAM)l10n->general_autosnap0);
		SendDlgItemMessage(hwnd, IDC_AUTOSNAP, CB_ADDSTRING, 0, (LPARAM)l10n->general_autosnap1);
		SendDlgItemMessage(hwnd, IDC_AUTOSNAP, CB_ADDSTRING, 0, (LPARAM)l10n->general_autosnap2);
		SendDlgItemMessage(hwnd, IDC_AUTOSNAP, CB_ADDSTRING, 0, (LPARAM)l10n->general_autosnap3);
		SendDlgItemMessage(hwnd, IDC_AUTOSNAP, CB_SETCURSEL, settings.AltDrag.AutoSnap, 0);
	}
	return FALSE;
}

BOOL CALLBACK InputPageDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	//Mouse actions
	struct {
		int control;
		wchar_t *setting;
		wchar_t *option;
	} mouse_buttons[] = {
		{IDC_LMB, settings.Mouse.LMB, L"LMB"},
		{IDC_MMB, settings.Mouse.MMB, L"MMB"},
		{IDC_RMB, settings.Mouse.RMB, L"RMB"},
		{IDC_MB4, settings.Mouse.MB4, L"MB4"},
		{IDC_MB5, settings.Mouse.MB5, L"MB5"},
	};
	wchar_t *mouse_actions[] = {L"Move", L"Resize", L"Close", L"Minimize", L"Lower", L"AlwaysOnTop", L"Center", L"Nothing"};
	
	//Hotkeys
	struct {
		int control;
		int *setting;
		int vkey;
	} hotkeys[] = {
		{IDC_LEFTALT,     &settings.Keyboard.LeftAlt,     VK_LMENU   },
		{IDC_RIGHTALT,    &settings.Keyboard.RightAlt,    VK_RMENU   },
		{IDC_LEFTWINKEY,  &settings.Keyboard.LeftWinkey,  VK_LWIN    },
		{IDC_RIGHTWINKEY, &settings.Keyboard.RightWinkey, VK_RWIN    },
		{IDC_LEFTCTRL,    &settings.Keyboard.LeftCtrl,    VK_LCONTROL},
		{IDC_RIGHTCTRL,   &settings.Keyboard.RightCtrl,   VK_RCONTROL},
	};
	
	if (msg == WM_INITDIALOG) {
		//Mouse actions
		int i, j;
		for (i=0; i < sizeof(mouse_buttons)/sizeof(mouse_buttons[0]); i++) {
			for (j=0; j < sizeof(mouse_actions)/sizeof(mouse_actions[0]); j++) {
				SendDlgItemMessage(hwnd, mouse_buttons[i].control, CB_ADDSTRING, 0, (LPARAM)mouse_actions[j]);
				if (!wcscmp(mouse_buttons[i].setting,mouse_actions[j])) {
					SendDlgItemMessage(hwnd, mouse_buttons[i].control, CB_SETCURSEL, j, 0);
				}
			}
		}
		
		//Hotkeys
		for (i=0; i < sizeof(hotkeys)/sizeof(hotkeys[0]); i++) {
			if (*hotkeys[i].setting) {
				SendDlgItemMessage(hwnd, hotkeys[i].control, BM_SETCHECK, BST_CHECKED, 0);
			}
		}
	}
	else if (msg == WM_COMMAND) {
		int i;
		if (HIWORD(wParam) == CBN_SELCHANGE) {
			//Mouse actions
			int control = LOWORD(wParam);
			for (i=0; i < sizeof(mouse_buttons)/sizeof(mouse_buttons[0]); i++) {
				if (control == mouse_buttons[i].control) {
					int index = SendDlgItemMessage(hwnd, mouse_buttons[i].control, CB_GETCURSEL, 0, 0);
					SendDlgItemMessage(hwnd, mouse_buttons[i].control, CB_GETLBTEXT, index, (LPARAM)mouse_buttons[i].setting);
					WritePrivateProfileString(L"Mouse", mouse_buttons[i].option, mouse_buttons[i].setting, inipath);
				}
			}
		}
		else {
			//Hotkeys
			wchar_t txt[100] = {'\0', '\0'}; //Second \0 needed if no keys are selected
			for (i=0; i < sizeof(hotkeys)/sizeof(hotkeys[0]); i++) {
				*hotkeys[i].setting = SendDlgItemMessage(hwnd, hotkeys[i].control, BM_GETCHECK, 0, 0);
				if (*hotkeys[i].setting) {
					swprintf(txt, L"%s %02X", txt, hotkeys[i].vkey);
				}
			}
			wcscat(txt, settings.Keyboard.OtherKeys);
			WritePrivateProfileString(L"Keyboard", L"Hotkeys", txt+1, inipath); //Skip prefix space
		}
		UpdateSettings();
	}
	else if (msg == WM_NOTIFY) {
		LPNMHDR pnmh = (LPNMHDR)lParam;
		if (pnmh->code == PSN_SETACTIVE) {
			//Update text
			SetDlgItemText(hwnd, IDC_MOUSE_HEADER,   l10n->input_mouse_header);
			SetDlgItemText(hwnd, IDC_LMB_HEADER,     l10n->input_mouse_lmb);
			SetDlgItemText(hwnd, IDC_MMB_HEADER,     l10n->input_mouse_mmb);
			SetDlgItemText(hwnd, IDC_RMB_HEADER,     l10n->input_mouse_rmb);
			SetDlgItemText(hwnd, IDC_MB4_HEADER,     l10n->input_mouse_mb4);
			SetDlgItemText(hwnd, IDC_MB5_HEADER,     l10n->input_mouse_mb5);
			SetDlgItemText(hwnd, IDC_MOUSE_IDEA,     l10n->input_mouse_idea);
			SetDlgItemText(hwnd, IDC_HOTKEYS_HEADER, l10n->input_hotkeys_header);
			SetDlgItemText(hwnd, IDC_LEFTALT,        l10n->input_hotkeys_leftalt);
			SetDlgItemText(hwnd, IDC_RIGHTALT,       l10n->input_hotkeys_rightalt);
			SetDlgItemText(hwnd, IDC_LEFTWINKEY,     l10n->input_hotkeys_leftwinkey);
			SetDlgItemText(hwnd, IDC_RIGHTWINKEY,    l10n->input_hotkeys_rightwinkey);
			SetDlgItemText(hwnd, IDC_LEFTCTRL,       l10n->input_hotkeys_leftctrl);
			SetDlgItemText(hwnd, IDC_RIGHTCTRL,      l10n->input_hotkeys_rightctrl);
			SetDlgItemText(hwnd, IDC_HOTKEYS_MORE,   l10n->input_hotkeys_more);
		}
	}
			
	LinkProc(hwnd, msg, wParam, lParam);
	return FALSE;
}

BOOL CALLBACK BlacklistPageDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_INITDIALOG) {
		SetDlgItemText(hwnd, IDC_PROCESSBLACKLIST, settings.Blacklist.ProcessBlacklist);
		SetDlgItemText(hwnd, IDC_BLACKLIST, settings.Blacklist.Blacklist);
		SetDlgItemText(hwnd, IDC_SNAPLIST, settings.Blacklist.Snaplist);
	}
	else if (msg == WM_COMMAND) {
		int control = LOWORD(wParam);
		if (HIWORD(wParam) == EN_KILLFOCUS) {
			if (control == IDC_PROCESSBLACKLIST) {
				SendDlgItemMessage(hwnd, IDC_PROCESSBLACKLIST, WM_GETTEXT, sizeof(settings.Blacklist.ProcessBlacklist)/sizeof(wchar_t), (LPARAM)settings.Blacklist.ProcessBlacklist);
				WritePrivateProfileString(L"Blacklist", L"ProcessBlacklist", settings.Blacklist.ProcessBlacklist, inipath);
			}
			else if (control == IDC_BLACKLIST) {
				SendDlgItemMessage(hwnd, IDC_BLACKLIST, WM_GETTEXT, sizeof(settings.Blacklist.Blacklist)/sizeof(wchar_t), (LPARAM)settings.Blacklist.Blacklist);
				WritePrivateProfileString(L"Blacklist", L"Blacklist", settings.Blacklist.Blacklist, inipath);
			}
			else if (control == IDC_SNAPLIST) {
				SendDlgItemMessage(hwnd, IDC_SNAPLIST, WM_GETTEXT, sizeof(settings.Blacklist.Snaplist)/sizeof(wchar_t), (LPARAM)settings.Blacklist.Snaplist);
				WritePrivateProfileString(L"Blacklist", L"Snaplist", settings.Blacklist.Snaplist, inipath);
			}
			UpdateSettings();
		}
		else if (HIWORD(wParam) == STN_CLICKED && control == IDC_FINDWINDOW) {
			//Get size of workspace
			int left = GetSystemMetrics(SM_XVIRTUALSCREEN);
			int top = GetSystemMetrics(SM_YVIRTUALSCREEN);
			int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
			int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
			
			//Create window
			WNDCLASSEX wnd = {sizeof(WNDCLASSEX), 0, WindowProc, 0, 0, g_hinst, NULL, NULL, (HBRUSH)(COLOR_WINDOW+1), NULL, APP_NAME, NULL};
			wnd.hCursor = LoadImage(g_hinst, MAKEINTRESOURCE(IDI_FIND), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR);
			RegisterClassEx(&wnd);
			HWND findhwnd = CreateWindowEx(WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_LAYERED, wnd.lpszClassName, APP_NAME, WS_POPUP, left, top, width, height, NULL, NULL, g_hinst, NULL);
			SetLayeredWindowAttributes(findhwnd, 0, 1, LWA_ALPHA); //Almost transparent
			ShowWindowAsync(findhwnd, SW_SHOWNA);
			
			//Hide icon
			ShowWindowAsync(GetDlgItem(hwnd,IDC_FINDWINDOW), SW_HIDE);
		}
	}
	LinkProc(hwnd, msg, wParam, lParam);
	return FALSE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN) {
		ShowWindow(hwnd, SW_HIDE);
		HWND page = PropSheet_GetCurrentPageHwnd(g_hwnd);
		
		if (msg == WM_LBUTTONDOWN) {
			POINT pt;
			pt.x = GET_X_LPARAM(lParam);
			pt.y = GET_Y_LPARAM(lParam);
			
			HWND window = WindowFromPoint(pt);
			window = GetAncestor(window, GA_ROOT);
			
			wchar_t title[256], classname[256];
			GetWindowText(window, title, sizeof(title)/sizeof(wchar_t));
			GetClassName(window, classname, sizeof(classname)/sizeof(wchar_t));
			
			wchar_t txt[1000];
			swprintf(txt, L"%s|%s", title, classname);
			SetDlgItemText(page, IDC_NEWRULE, txt);
		}
		
		//Show icon again
		ShowWindowAsync(GetDlgItem(page,IDC_FINDWINDOW), SW_SHOW);
		
		DestroyWindow(hwnd);
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

BOOL CALLBACK AdvancedPageDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_INITDIALOG) {
		SendDlgItemMessage(hwnd, IDC_HOOKWINDOWS, BM_SETCHECK, settings.AltDrag.HookWindows?BST_CHECKED:BST_UNCHECKED, 0);
	}
	else if (msg == WM_COMMAND) {
		if (wParam == IDC_OPENINI) {
			ShellExecute(NULL, L"open", inipath, NULL, NULL, SW_SHOWNORMAL);
		}
		else {
			wchar_t txt[10];
			settings.AltDrag.HookWindows = SendDlgItemMessage(hwnd, IDC_HOOKWINDOWS, BM_GETCHECK, 0, 0);
			WritePrivateProfileString(APP_NAME, L"HookWindows", _itow(settings.AltDrag.HookWindows,txt,10), inipath);
			UpdateSettings();
		}
	}
	return FALSE;
}

BOOL CALLBACK AboutPageDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_INITDIALOG) {
		
	}
	else if (msg == WM_COMMAND) {
		if (wParam == IDC_DONATE) {
			ShellExecute(NULL, L"open", L"http://code.google.com/p/altdrag/wiki/Donate", NULL, NULL, SW_SHOWNORMAL);
		}
	}
	LinkProc(hwnd, msg, wParam, lParam);
	return FALSE;
}

BOOL CALLBACK LinkProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_NOTIFY) {
		NMLINK *link = (NMLINK*)lParam;
		if (link->hdr.code == NM_CLICK || link->hdr.code == NM_RETURN) {
			ShellExecute(NULL, L"open", link->item.szUrl, NULL, NULL, SW_SHOWDEFAULT);
			return TRUE;
		}
	}
}
