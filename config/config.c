/*
	Copyright (C) 2011  Stefan Sundin (recover89@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

#include <commctrl.h>
#include <prsht.h>
#include <windowsx.h>

//Boring stuff
BOOL CALLBACK PropSheetProc(HWND, UINT, LPARAM);
INT_PTR CALLBACK GeneralPageDialogProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK InputPageDialogProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK BlacklistPageDialogProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK AdvancedPageDialogProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK AboutPageDialogProc(HWND, UINT, WPARAM, LPARAM);
void LinkProc(HWND, UINT, WPARAM, LPARAM);
HWND g_cfgwnd = NULL;

//Blacklist
LRESULT CALLBACK CursorProc(HWND, UINT, WPARAM, LPARAM);

//Include stuff
#include "resource.h"
#include "../include/autostart.c"

//Entry point
void OpenConfig(int startpage) {
	if (IsWindow(g_cfgwnd)) {
		PropSheet_SetCurSel(g_cfgwnd, 0, startpage);
		SetForegroundWindow(g_cfgwnd);
		return;
	}
	
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
	int i;
	for (i=0; i < sizeof(pages)/sizeof(pages[0]); i++) {
		psp[i].dwSize      = sizeof(PROPSHEETPAGE);
		psp[i].hInstance   = g_hinst;
		psp[i].pszTemplate = MAKEINTRESOURCE(pages[i].pszTemplate);
		psp[i].pfnDlgProc  = pages[i].pfnDlgProc;
	}
	
	//Define the property sheet
	PROPSHEETHEADER psh = {0};
	psh.dwSize          = sizeof(PROPSHEETHEADER);
	psh.dwFlags         = PSH_PROPSHEETPAGE | PSH_USECALLBACK | PSH_USEHICON | PSH_NOAPPLYNOW | PSH_NOCONTEXTHELP;
	psh.hwndParent      = NULL;
	psh.hInstance       = g_hinst;
	psh.hIcon           = icon[1];
	psh.pszCaption      = APP_NAME;
	psh.nPages          = sizeof(pages)/sizeof(pages[0]);
	psh.ppsp            = (LPCPROPSHEETPAGE)&psp;
	psh.pfnCallback     = PropSheetProc;
	psh.nStartPage      = startpage;
	
	//Open the property sheet
	PropertySheet(&psh);
}

void UpdateSettings() {
	PostMessage(g_hwnd, WM_UPDATESETTINGS, 0, 0);
}

void UpdateL10n() {
	//Update window title
	PropSheet_SetTitle(g_cfgwnd, 0, l10n->title);
	
	//Update tab titles
	HWND tc = PropSheet_GetTabControl(g_cfgwnd);
	int numrows_prev = TabCtrl_GetRowCount(tc);
	wchar_t *titles[] = { l10n->tab_general, l10n->tab_input, l10n->tab_blacklist, l10n->tab_advanced, l10n->tab_about };
	int i;
	for (i=0; i < sizeof(titles)/sizeof(titles[0]); i++) {
		TCITEM ti;
		ti.mask = TCIF_TEXT;
		ti.pszText = titles[i];
		TabCtrl_SetItem(tc, i, &ti);
	}
	
	//Modify UI if number of rows have changed
	int numrows = TabCtrl_GetRowCount(tc);
	if (numrows_prev != numrows) {
		HWND page = PropSheet_GetCurrentPageHwnd(g_cfgwnd);
		if (page != NULL) {
			int diffrows = numrows-numrows_prev;
			WINDOWPLACEMENT wndpl;
			wndpl.length = sizeof(WINDOWPLACEMENT);
			//Resize window
			GetWindowPlacement(g_cfgwnd, &wndpl);
			wndpl.rcNormalPosition.bottom += 18*diffrows;
			SetWindowPlacement(g_cfgwnd, &wndpl);
			//Resize tabcontrol
			GetWindowPlacement(tc, &wndpl);
			wndpl.rcNormalPosition.bottom += 18*diffrows;
			SetWindowPlacement(tc, &wndpl);
			//Move button
			HWND button = GetDlgItem(g_cfgwnd, IDOK);
			GetWindowPlacement(button, &wndpl);
			int height = wndpl.rcNormalPosition.bottom-wndpl.rcNormalPosition.top;
			wndpl.rcNormalPosition.top += 18*diffrows;
			wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top+height;
			SetWindowPlacement(button, &wndpl);
			//Re-select tab
			PropSheet_SetCurSel(g_cfgwnd, page, 0);
			//Invalidate region
			GetWindowPlacement(g_cfgwnd, &wndpl);
			InvalidateRect(g_cfgwnd, &wndpl.rcNormalPosition, TRUE);
		}
	}
}

BOOL CALLBACK PropSheetProc(HWND hwnd, UINT msg, LPARAM lParam) {
	if (msg == PSCB_INITIALIZED) {
		g_cfgwnd = hwnd;
		UpdateL10n();
		
		//OK button replaces Cancel button
		SendMessage(g_cfgwnd, PSM_CANCELTOCLOSE, 0, 0);
		EnableWindow(GetDlgItem(g_cfgwnd,IDCANCEL), TRUE); //Re-enable to enable escape key
		WINDOWPLACEMENT wndpl;
		wndpl.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(GetDlgItem(g_cfgwnd,IDCANCEL), &wndpl);
		SetWindowPlacement(GetDlgItem(g_cfgwnd,IDOK), &wndpl);
		ShowWindow(GetDlgItem(g_cfgwnd,IDCANCEL), SW_HIDE);
	}
}

INT_PTR CALLBACK GeneralPageDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	int updatel10n = 0;
	if (msg == WM_INITDIALOG) {
		wchar_t txt[20];
		GetPrivateProfileString(APP_NAME, L"AutoFocus", L"0", txt, sizeof(txt)/sizeof(wchar_t), inipath);
		Button_SetCheck(GetDlgItem(hwnd,IDC_AUTOFOCUS), _wtoi(txt)?BST_CHECKED:BST_UNCHECKED);
		
		GetPrivateProfileString(APP_NAME, L"Aero", L"2", txt, sizeof(txt)/sizeof(wchar_t), inipath);
		Button_SetCheck(GetDlgItem(hwnd,IDC_AERO), _wtoi(txt)?BST_CHECKED:BST_UNCHECKED);
		
		GetPrivateProfileString(APP_NAME, L"InactiveScroll", L"1", txt, sizeof(txt)/sizeof(wchar_t), inipath);
		Button_SetCheck(GetDlgItem(hwnd,IDC_INACTIVESCROLL), _wtoi(txt)?BST_CHECKED:BST_UNCHECKED);
		
		int i;
		for (i=0; languages[i].code != NULL; i++) {
			ComboBox_AddString(GetDlgItem(hwnd,IDC_LANGUAGE), languages[i].strings->lang);
			if (l10n == languages[i].strings) {
				ComboBox_SetCurSel(GetDlgItem(hwnd,IDC_LANGUAGE), i);
			}
		}
	}
	else if (msg == WM_COMMAND) {
		int id = LOWORD(wParam);
		int event = HIWORD(wParam);
		HWND control = GetDlgItem(hwnd, id);
		int val = Button_GetCheck(control);
		wchar_t txt[10];
		
		if (id == IDC_AUTOFOCUS) {
			WritePrivateProfileString(APP_NAME, L"AutoFocus", _itow(val,txt,10), inipath);
		}
		else if (id == IDC_AUTOSNAP && event == CBN_SELCHANGE) {
			val = ComboBox_GetCurSel(control);
			WritePrivateProfileString(APP_NAME, L"AutoSnap", _itow(val,txt,10), inipath);
		}
		else if (id == IDC_AERO) {
			WritePrivateProfileString(APP_NAME, L"Aero", _itow(val,txt,10), inipath);
		}
		else if (id == IDC_INACTIVESCROLL) {
			WritePrivateProfileString(APP_NAME, L"InactiveScroll", _itow(val,txt,10), inipath);
		}
		else if (id == IDC_LANGUAGE && event == CBN_SELCHANGE) {
			int i = ComboBox_GetCurSel(control);
			if (languages[i].code == NULL) {
				ShellExecute(NULL, L"open", L"http://code.google.com/p/altdrag/wiki/Translate", NULL, NULL, SW_SHOWNORMAL);
				for (i=0; l10n != languages[i].strings; i++) {}
				ComboBox_SetCurSel(control, i);
			}
			else {
				l10n = languages[i].strings;
				WritePrivateProfileString(APP_NAME, L"Language", languages[i].code, inipath);
				updatel10n = 1;
				UpdateL10n();
			}
		}
		else if (id == IDC_AUTOSTART) {
			SetAutostart(val, 0);
			EnableWindow(GetDlgItem(hwnd,IDC_AUTOSTART_HIDE), val);
			if (!val) {
				Button_SetCheck(GetDlgItem(hwnd,IDC_AUTOSTART_HIDE), BST_UNCHECKED);
			}
		}
		else if (id == IDC_AUTOSTART_HIDE) {
			SetAutostart(1, val);
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
			Button_SetCheck(GetDlgItem(hwnd,IDC_AUTOSTART), autostart?BST_CHECKED:BST_UNCHECKED);
			Button_SetCheck(GetDlgItem(hwnd,IDC_AUTOSTART_HIDE), hidden?BST_CHECKED:BST_UNCHECKED);
			Button_Enable(GetDlgItem(hwnd,IDC_AUTOSTART_HIDE), autostart);
		}
	}
	if (updatel10n) {
		//Update text
		SetDlgItemText(hwnd, IDC_GENERAL_BOX,        l10n->general_box);
		SetDlgItemText(hwnd, IDC_AUTOFOCUS,          l10n->general_autofocus);
		SetDlgItemText(hwnd, IDC_AERO,               l10n->general_aero);
		SetDlgItemText(hwnd, IDC_INACTIVESCROLL,     l10n->general_inactivescroll);
		SetDlgItemText(hwnd, IDC_AUTOSNAP_HEADER,    l10n->general_autosnap);
		SetDlgItemText(hwnd, IDC_LANGUAGE_HEADER,    l10n->general_language);
		SetDlgItemText(hwnd, IDC_AUTOSTART_BOX,      l10n->general_autostart_box);
		SetDlgItemText(hwnd, IDC_AUTOSTART,          l10n->general_autostart);
		SetDlgItemText(hwnd, IDC_AUTOSTART_HIDE,     l10n->general_autostart_hide);
		SetDlgItemText(hwnd, IDC_AUTOSAVE,           l10n->general_autosave);
		
		//AutoSnap
		ComboBox_ResetContent(GetDlgItem(hwnd,IDC_AUTOSNAP));
		ComboBox_AddString(GetDlgItem(hwnd,IDC_AUTOSNAP), l10n->general_autosnap0);
		ComboBox_AddString(GetDlgItem(hwnd,IDC_AUTOSNAP), l10n->general_autosnap1);
		ComboBox_AddString(GetDlgItem(hwnd,IDC_AUTOSNAP), l10n->general_autosnap2);
		ComboBox_AddString(GetDlgItem(hwnd,IDC_AUTOSNAP), l10n->general_autosnap3);
		wchar_t txt[10];
		GetPrivateProfileString(APP_NAME, L"AutoSnap", L"0", txt, sizeof(txt)/sizeof(wchar_t), inipath);
		ComboBox_SetCurSel(GetDlgItem(hwnd,IDC_AUTOSNAP), _wtoi(txt));
		
		//Language
		ComboBox_DeleteString(GetDlgItem(hwnd,IDC_LANGUAGE), sizeof(languages)/sizeof(languages[0])-1);
		ComboBox_AddString(GetDlgItem(hwnd,IDC_LANGUAGE), l10n->general_helptranslate);
	}
	return FALSE;
}

INT_PTR CALLBACK InputPageDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	//Mouse actions
	struct {
		int control;
		wchar_t *option;
	} mouse_buttons[] = {
		{ IDC_LMB, L"LMB" },
		{ IDC_MMB, L"MMB" },
		{ IDC_RMB, L"RMB" },
		{ IDC_MB4, L"MB4" },
		{ IDC_MB5, L"MB5" },
	};
	wchar_t *mouse_actions[] = {L"Move", L"Resize", L"Close", L"Minimize", L"Lower", L"AlwaysOnTop", L"Center", L"Nothing"};
	
	//Hotkeys
	struct {
		int control;
		int vkey;
	} hotkeys[] = {
		{ IDC_LEFTALT,     VK_LMENU },
		{ IDC_RIGHTALT,    VK_RMENU },
		{ IDC_LEFTWINKEY,  VK_LWIN },
		{ IDC_RIGHTWINKEY, VK_RWIN },
		{ IDC_LEFTCTRL,    VK_LCONTROL },
		{ IDC_RIGHTCTRL,   VK_RCONTROL },
	};
	
	if (msg == WM_INITDIALOG) {
		wchar_t txt[50];
		//Mouse actions
		int i, j;
		for (i=0; i < sizeof(mouse_buttons)/sizeof(mouse_buttons[0]); i++) {
			GetPrivateProfileString(L"Mouse", mouse_buttons[i].option, L"Nothing", txt, sizeof(txt)/sizeof(wchar_t), inipath);
			for (j=0; j < sizeof(mouse_actions)/sizeof(mouse_actions[0]); j++) {
				ComboBox_AddString(GetDlgItem(hwnd,mouse_buttons[i].control), mouse_actions[j]);
				if (!wcscmp(txt,mouse_actions[j])) {
					ComboBox_SetCurSel(GetDlgItem(hwnd,mouse_buttons[i].control), j);
				}
			}
		}
		
		//Hotkeys
		unsigned int temp;
		int numread;
		GetPrivateProfileString(L"Keyboard", L"Hotkeys", L"A4 A5", txt, sizeof(txt)/sizeof(wchar_t), inipath);
		wchar_t *pos = txt;
		while (*pos != '\0' && swscanf(pos,L"%02X%n",&temp,&numread) != EOF) {
			pos += numread;
			//What key was that?
			for (i=0; i < sizeof(hotkeys)/sizeof(hotkeys[0]); i++) {
				if (temp == hotkeys[i].vkey) {
					Button_SetCheck(GetDlgItem(hwnd,hotkeys[i].control), BST_CHECKED);
					break;
				}
			}
		}
	}
	else if (msg == WM_COMMAND) {
		wchar_t txt[50] = L"";
		int i;
		if (HIWORD(wParam) == CBN_SELCHANGE) {
			//Mouse actions
			int control = LOWORD(wParam);
			for (i=0; i < sizeof(mouse_buttons)/sizeof(mouse_buttons[0]); i++) {
				if (control == mouse_buttons[i].control) {
					ComboBox_GetText(GetDlgItem(hwnd,mouse_buttons[i].control), txt, sizeof(txt)/sizeof(wchar_t));
					WritePrivateProfileString(L"Mouse", mouse_buttons[i].option, txt, inipath);
					break;
				}
			}
		}
		else {
			//Hotkeys
			int vkey = 0;
			for (i=0; i < sizeof(hotkeys)/sizeof(hotkeys[0]); i++) {
				if (wParam == hotkeys[i].control) {
					vkey = hotkeys[i].vkey;
					break;
				}
			}
			if (!vkey) return FALSE;
			
			wchar_t keys[50];
			GetPrivateProfileString(L"Keyboard", L"Hotkeys", L"", keys, sizeof(keys)/sizeof(wchar_t), inipath);
			int add = Button_GetCheck(GetDlgItem(hwnd,wParam));
			if (add) {
				if (*keys != '\0') {
					wcscat(keys, L" ");
				}
				swprintf(txt, L"%s%02X", keys, vkey);
			}
			else {
				unsigned int temp;
				int numread;
				wchar_t *pos = keys;
				while (*pos != '\0' && swscanf(pos,L"%02X%n",&temp,&numread) != EOF) {
					if (temp == vkey) {
						wcsncpy(txt, keys, pos-keys);
						wcscat(txt, pos+numread);
						break;
					}
					pos += numread;
				}
			}
			WritePrivateProfileString(L"Keyboard", L"Hotkeys", txt, inipath);
		}
		UpdateSettings();
	}
	else if (msg == WM_NOTIFY) {
		LPNMHDR pnmh = (LPNMHDR)lParam;
		if (pnmh->code == PSN_SETACTIVE) {
			//Update text
			SetDlgItemText(hwnd, IDC_MOUSE_BOX,      l10n->input_mouse_box);
			SetDlgItemText(hwnd, IDC_LMB_HEADER,     l10n->input_mouse_lmb);
			SetDlgItemText(hwnd, IDC_MMB_HEADER,     l10n->input_mouse_mmb);
			SetDlgItemText(hwnd, IDC_RMB_HEADER,     l10n->input_mouse_rmb);
			SetDlgItemText(hwnd, IDC_MB4_HEADER,     l10n->input_mouse_mb4);
			SetDlgItemText(hwnd, IDC_MB5_HEADER,     l10n->input_mouse_mb5);
			SetDlgItemText(hwnd, IDC_MOUSE_MORE,     l10n->input_mouse_more);
			SetDlgItemText(hwnd, IDC_HOTKEYS_BOX,    l10n->input_hotkeys_box);
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

INT_PTR CALLBACK BlacklistPageDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_INITDIALOG) {
		wchar_t txt[1000];
		GetPrivateProfileString(L"Blacklist", L"ProcessBlacklist", L"", txt, sizeof(txt)/sizeof(wchar_t), inipath);
		SetDlgItemText(hwnd, IDC_PROCESSBLACKLIST, txt);
		GetPrivateProfileString(L"Blacklist", L"Blacklist", L"", txt, sizeof(txt)/sizeof(wchar_t), inipath);
		SetDlgItemText(hwnd, IDC_BLACKLIST, txt);
		GetPrivateProfileString(L"Blacklist", L"Snaplist", L"", txt, sizeof(txt)/sizeof(wchar_t), inipath);
		SetDlgItemText(hwnd, IDC_SNAPLIST, txt);
	}
	else if (msg == WM_COMMAND) {
		wchar_t txt[1000];
		int control = LOWORD(wParam);
		if (HIWORD(wParam) == EN_KILLFOCUS) {
			Edit_GetText(GetDlgItem(hwnd,control), txt, sizeof(txt)/sizeof(wchar_t));
			if (control == IDC_PROCESSBLACKLIST) {
				WritePrivateProfileString(L"Blacklist", L"ProcessBlacklist", txt, inipath);
			}
			else if (control == IDC_BLACKLIST) {
				WritePrivateProfileString(L"Blacklist", L"Blacklist", txt, inipath);
			}
			else if (control == IDC_SNAPLIST) {
				WritePrivateProfileString(L"Blacklist", L"Snaplist", txt, inipath);
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
			WNDCLASSEX wnd = {sizeof(WNDCLASSEX), 0, CursorProc, 0, 0, g_hinst, NULL, NULL, (HBRUSH)(COLOR_WINDOW+1), NULL, APP_NAME"-find", NULL};
			wnd.hCursor = LoadImage(g_hinst, MAKEINTRESOURCE(IDI_FIND), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR);
			RegisterClassEx(&wnd);
			HWND findhwnd = CreateWindowEx(WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_LAYERED, wnd.lpszClassName, NULL, WS_POPUP, left, top, width, height, NULL, NULL, g_hinst, NULL);
			SetLayeredWindowAttributes(findhwnd, 0, 1, LWA_ALPHA); //Almost transparent
			ShowWindowAsync(findhwnd, SW_SHOWNA);
			
			//Hide icon
			ShowWindowAsync(GetDlgItem(hwnd,IDC_FINDWINDOW), SW_HIDE);
		}
	}
	else if (msg == WM_NOTIFY) {
		LPNMHDR pnmh = (LPNMHDR)lParam;
		if (pnmh->code == PSN_SETACTIVE) {
			//Update text
			SetDlgItemText(hwnd, IDC_BLACKLIST_BOX,           l10n->blacklist_box);
			SetDlgItemText(hwnd, IDC_PROCESSBLACKLIST_HEADER, l10n->blacklist_processblacklist);
			SetDlgItemText(hwnd, IDC_BLACKLIST_HEADER,        l10n->blacklist_blacklist);
			SetDlgItemText(hwnd, IDC_SNAPLIST_HEADER,         l10n->blacklist_snaplist);
			SetDlgItemText(hwnd, IDC_BLACKLIST_EXPLANATION,   l10n->blacklist_explanation);
			SetDlgItemText(hwnd, IDC_FINDWINDOW_BOX,          l10n->blacklist_findwindow_box);
			SetDlgItemText(hwnd, IDC_FINDWINDOW_EXPLANATION,  l10n->blacklist_findwindow_explanation);
		}
	}
	
	LinkProc(hwnd, msg, wParam, lParam);
	return FALSE;
}

LRESULT CALLBACK CursorProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN) {
		ShowWindow(hwnd, SW_HIDE);
		HWND page = PropSheet_GetCurrentPageHwnd(g_cfgwnd);
		
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

INT_PTR CALLBACK AdvancedPageDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_INITDIALOG) {
		wchar_t txt[10];
		GetPrivateProfileString(APP_NAME, L"HookWindows", L"0", txt, sizeof(txt)/sizeof(wchar_t), inipath);
		Button_SetCheck(GetDlgItem(hwnd,IDC_HOOKWINDOWS), _wtoi(txt)?BST_CHECKED:BST_UNCHECKED);
		GetPrivateProfileString(L"Update", L"CheckOnStartup", L"0", txt, sizeof(txt)/sizeof(wchar_t), inipath);
		Button_SetCheck(GetDlgItem(hwnd,IDC_CHECKONSTARTUP), _wtoi(txt)?BST_CHECKED:BST_UNCHECKED);
		GetPrivateProfileString(L"Update", L"Beta", L"0", txt, sizeof(txt)/sizeof(wchar_t), inipath);
		Button_SetCheck(GetDlgItem(hwnd,IDC_BETA), _wtoi(txt)?BST_CHECKED:BST_UNCHECKED);
	}
	else if (msg == WM_COMMAND) {
		if (wParam == IDC_OPENINI) {
			ShellExecute(NULL, L"open", inipath, NULL, NULL, SW_SHOWNORMAL);
		}
		else {
			wchar_t txt[10];
			int val = Button_GetCheck(GetDlgItem(hwnd,wParam));
			if (wParam == IDC_HOOKWINDOWS) {
				if (val && MessageBox(NULL, l10n->advanced_hookwindows_warn, APP_NAME, MB_ICONINFORMATION|MB_YESNO) == IDNO) {
					Button_SetCheck(GetDlgItem(hwnd,IDC_HOOKWINDOWS), BST_UNCHECKED);
					return;
				}
				WritePrivateProfileString(APP_NAME, L"HookWindows", _itow(val,txt,10), inipath);
				UpdateSettings();
			}
			else if (wParam == IDC_CHECKONSTARTUP) {
				WritePrivateProfileString(L"Update", L"CheckOnStartup", _itow(val,txt,10), inipath);
			}
			else if (wParam == IDC_BETA) {
				WritePrivateProfileString(L"Update", L"Beta", _itow(val,txt,10), inipath);
			}
			else if (wParam == IDC_CHECKNOW) {
				CheckForUpdate(1);
			}
		}
	}
	else if (msg == WM_NOTIFY) {
		LPNMHDR pnmh = (LPNMHDR)lParam;
		if (pnmh->code == PSN_SETACTIVE) {
			//Update text
			SetDlgItemText(hwnd, IDC_ADVANCED_BOX,    l10n->advanced_box);
			SetDlgItemText(hwnd, IDC_HOOKWINDOWS,     l10n->advanced_hookwindows);
			SetDlgItemText(hwnd, IDC_CHECKONSTARTUP,  l10n->advanced_checkonstartup);
			SetDlgItemText(hwnd, IDC_BETA,            l10n->advanced_beta);
			SetDlgItemText(hwnd, IDC_CHECKNOW,        l10n->advanced_checknow);
			SetDlgItemText(hwnd, IDC_INI,             l10n->advanced_ini);
			SetDlgItemText(hwnd, IDC_OPENINI,         l10n->advanced_openini);
		}
	}
	return FALSE;
}

INT_PTR CALLBACK AboutPageDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_COMMAND) {
		if (wParam == IDC_DONATE) {
			ShellExecute(NULL, L"open", L"http://code.google.com/p/altdrag/wiki/Donate", NULL, NULL, SW_SHOWNORMAL);
		}
	}
	else if (msg == WM_NOTIFY) {
		LPNMHDR pnmh = (LPNMHDR)lParam;
		if (pnmh->code == PSN_SETACTIVE) {
			//Update text
			SetDlgItemText(hwnd, IDC_ABOUT_BOX, l10n->about_box);
			SetDlgItemText(hwnd, IDC_VERSION,   l10n->about_version);
			SetDlgItemText(hwnd, IDC_AUTHOR,    l10n->about_author);
			SetDlgItemText(hwnd, IDC_LICENSE,   l10n->about_license);
			wchar_t txt[50] = L"       ";
			wcscat(txt, l10n->about_donations_box);
			SetDlgItemText(hwnd, IDC_DONATIONS_BOX, txt);
			SetDlgItemText(hwnd, IDC_DONATIONS_PLEA, l10n->about_donations_plea);
			SetDlgItemText(hwnd, IDC_DONATE,         l10n->about_donate);
		}
	}
	
	LinkProc(hwnd, msg, wParam, lParam);
	return FALSE;
}

void LinkProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_NOTIFY) {
		NMLINK *link = (NMLINK*)lParam;
		if (link->hdr.code == NM_CLICK || link->hdr.code == NM_RETURN) {
			ShellExecute(NULL, L"open", link->item.szUrl, NULL, NULL, SW_SHOWDEFAULT);
		}
	}
}
