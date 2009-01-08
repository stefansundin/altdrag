/*
	WindowFinder - Get the title and classname of windows
	Copyright (C) 2008  Stefan Sundin (recover89@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

#define UNICODE
#define _UNICODE

#include <stdio.h>
#include <stdlib.h>
#define _WIN32_WINNT 0x0500
#define _WIN32_IE 0x0600
#include <windows.h>
#include <shlwapi.h>
#include <wininet.h>

//App
#define APP_NAME      L"WindowFinder"
#define APP_VERSION   "0.1"
#define APP_URL       L"http://code.google.com/p/altdrag/wiki/WindowFinder"
#define APP_UPDATEURL L"http://altdrag.googlecode.com/svn/wiki/windowfinder-latest-stable.txt"
//#define DEBUG

//Localization
#ifndef L10N_FILE
#define L10N_FILE "localization/en-US/strings.h"
#endif
#include L10N_FILE
#if L10N_VERSION != 1
#error Localization not up to date!
#endif

//Messages
#define WM_ICONTRAY            WM_USER+1
#define SWM_FIND               WM_APP+1
#define SWM_FINDDELAY          WM_APP+2
#define SWM_FINDALL            WM_APP+3
#define SWM_UPDATE             WM_APP+4
#define SWM_ABOUT              WM_APP+5
#define SWM_EXIT               WM_APP+6

//Balloon stuff missing in MinGW
#define NIIF_USER 4
#define NIN_BALLOONSHOW        WM_USER+2
#define NIN_BALLOONHIDE        WM_USER+3
#define NIN_BALLOONTIMEOUT     WM_USER+4
#define NIN_BALLOONUSERCLICK   WM_USER+5

//Boring stuff
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
static HICON icon;
static NOTIFYICONDATA traydata;
static UINT WM_TASKBARCREATED=0;
static int tray_added=0;
static int update=0;
struct {
	int CheckForUpdate;
} settings={0};
static wchar_t txt[1000];

//Cool stuff
static HINSTANCE hinstDLL=NULL;
static HHOOK mousehook=NULL;
static HWND cursorwnd=NULL;
static int winxp=0;
static HWND *wnds=NULL;
static int numwnds=0;

//Error message handling
static int showerror=1;

LRESULT CALLBACK ErrorMsgProc(INT nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HCBT_ACTIVATE) {
		//Edit the caption of the buttons
		SetDlgItemText((HWND)wParam,IDYES,L"Copy error");
		SetDlgItemText((HWND)wParam,IDNO,L"OK");
	}
	return 0;
}

void Error(wchar_t *func, wchar_t *info, int errorcode, int line) {
	if (showerror) {
		//Format message
		wchar_t errormsg[100];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL,errorcode,0,errormsg,sizeof(errormsg)/sizeof(wchar_t),NULL);
		errormsg[wcslen(errormsg)-2]='\0'; //Remove that damn newline at the end of the formatted error message
		swprintf(txt,L"%s failed in file %s, line %d.\nError: %s (%d)\n\n%s", func, TEXT(__FILE__), line, errormsg, errorcode, info);
		//Display message
		HHOOK hhk=SetWindowsHookEx(WH_CBT, &ErrorMsgProc, 0, GetCurrentThreadId());
		int response=MessageBox(NULL, txt, APP_NAME" Error", MB_ICONERROR|MB_YESNO|MB_DEFBUTTON2);
		UnhookWindowsHookEx(hhk);
		if (response == IDYES) {
			//Copy message to clipboard
			OpenClipboard(NULL);
			EmptyClipboard();
			wchar_t *data=LocalAlloc(LMEM_FIXED,(wcslen(txt)+1)*sizeof(wchar_t));
			memcpy(data,txt,(wcslen(txt)+1)*sizeof(wchar_t));
			SetClipboardData(CF_UNICODETEXT,data);
			CloseClipboard();
		}
	}
}

//Check for update
DWORD WINAPI _CheckForUpdate() {
	//Open connection
	HINTERNET http, file;
	if ((http=InternetOpen(APP_NAME" - "APP_VERSION,INTERNET_OPEN_TYPE_DIRECT,NULL,NULL,0)) == NULL) {
		#ifdef DEBUG
		Error(L"InternetOpen()",L"Could not establish connection.\nPlease check for update manually at "APP_URL,GetLastError(),__LINE__);
		#endif
		return;
	}
	if ((file=InternetOpenUrl(http,APP_UPDATEURL,NULL,0,INTERNET_FLAG_NO_AUTH|INTERNET_FLAG_NO_AUTO_REDIRECT|INTERNET_FLAG_NO_CACHE_WRITE|INTERNET_FLAG_NO_COOKIES|INTERNET_FLAG_NO_UI,0)) == NULL) {
		#ifdef DEBUG
		Error(L"InternetOpenUrl()",L"Could not establish connection.\nPlease check for update manually at "APP_URL,GetLastError(),__LINE__);
		#endif
		return;
	}
	//Read file
	char data[20];
	DWORD numread;
	if (InternetReadFile(file,data,sizeof(data),&numread) == FALSE) {
		#ifdef DEBUG
		Error(L"InternetReadFile()",L"Could not read file.\nPlease check for update manually at "APP_URL,GetLastError(),__LINE__);
		#endif
		return;
	}
	data[numread]='\0';
	//Get error code
	wchar_t code[4];
	DWORD len=sizeof(code);
	HttpQueryInfo(file,HTTP_QUERY_STATUS_CODE,&code,&len,NULL);
	//Close connection
	InternetCloseHandle(file);
	InternetCloseHandle(http);
	
	//Make sure the server returned 200
	if (wcscmp(code,L"200")) {
		#ifdef DEBUG
		swprintf(txt,L"Server returned %s error when checking for update.\nPlease check for update manually at "APP_URL,code);
		MessageBox(NULL, txt, APP_NAME, MB_ICONWARNING|MB_OK);
		#endif
		return;
	}
	
	//New version available?
	if (strcmp(data,APP_VERSION)) {
		update=1;
		traydata.uFlags|=NIF_INFO;
		UpdateTray();
		traydata.uFlags^=NIF_INFO;
	}
}

void CheckForUpdate() {
	CreateThread(NULL,0,_CheckForUpdate,NULL,0,NULL);
}

//Entry point
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
	//Look for previous instance
	HWND previnst;
	if ((previnst=FindWindow(APP_NAME,NULL)) != NULL) {
		SendMessage(previnst,WM_COMMAND,SWM_FIND,0);
		return 0;
	}
	
	//Create window class
	WNDCLASSEX wnd;
	wnd.cbSize=sizeof(WNDCLASSEX);
	wnd.style=0;
	wnd.lpfnWndProc=WindowProc;
	wnd.cbClsExtra=0;
	wnd.cbWndExtra=0;
	wnd.hInstance=hInst;
	wnd.hIcon=NULL;
	wnd.hIconSm=NULL;
	wnd.hCursor=LoadImage(hInst, L"find", IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR);
	wnd.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	wnd.lpszMenuName=NULL;
	wnd.lpszClassName=APP_NAME;
	
	//Register class
	RegisterClassEx(&wnd);
	
	//Create window
	HWND hwnd=CreateWindowEx(/*WS_EX_LAYERED|*/WS_EX_TOOLWINDOW|WS_EX_TOPMOST, wnd.lpszClassName, APP_NAME, WS_POPUP, 0, 0, 0, 0, NULL, NULL, hInst, NULL);
	cursorwnd=hwnd;
	
	//Load icon
	if ((icon = LoadImage(hInst, L"icon", IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR)) == NULL) {
		Error(L"LoadImage('tray-disabled')",L"Fatal error.",GetLastError(),__LINE__);
		PostQuitMessage(1);
	}
	
	//Create icondata
	traydata.cbSize=sizeof(NOTIFYICONDATA);
	traydata.uID=0;
	traydata.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;
	traydata.hWnd=hwnd;
	traydata.uCallbackMessage=WM_ICONTRAY;
	wcsncpy(traydata.szTip,APP_NAME,sizeof(traydata.szTip)/sizeof(wchar_t));
	traydata.hIcon=icon;
	//Balloon tooltip
	traydata.uTimeout=10000;
	wcsncpy(traydata.szInfoTitle,APP_NAME,sizeof(traydata.szInfoTitle)/sizeof(wchar_t));
	wcsncpy(traydata.szInfo,L10N_UPDATE_BALLOON,sizeof(traydata.szInfo)/sizeof(wchar_t));
	traydata.dwInfoFlags=NIIF_USER;
	
	//Register TaskbarCreated so we can re-add the tray icon if explorer.exe crashes
	WM_TASKBARCREATED=RegisterWindowMessage(L"TaskbarCreated");
	
	//Update tray icon
	UpdateTray();
	
	//Check if OS is WinXP/Server 2003
	OSVERSIONINFO vi;
	vi.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	GetVersionEx(&vi);
	if (vi.dwMajorVersion == 5 && vi.dwMinorVersion >= 1) {
		//Now we will set cursorwnd to 99% transparent to make it work in XP
		winxp=1;
	}
	
	//Load settings
	wchar_t path[MAX_PATH];
	GetModuleFileName(NULL,path,sizeof(path)/sizeof(wchar_t));
	PathRenameExtension(path,L".ini");
	GetPrivateProfileString(L"Update",L"CheckForUpdate",L"0",txt,sizeof(txt)/sizeof(wchar_t),path);
	swscanf(txt,L"%d",&settings.CheckForUpdate);
	
	//Check for update
	if (settings.CheckForUpdate) {
		CheckForUpdate();
	}
	
	//Hook mouse
	if ((GetAsyncKeyState(VK_SHIFT)&0x8000)) {
		HookMouse();
	}
	
	//Message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

void ShowContextMenu(HWND hwnd) {
	POINT pt;
	GetCursorPos(&pt);
	HMENU hMenu=CreatePopupMenu();
	
	//Find
	InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_FIND, L10N_MENU_FIND);
	InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_FINDDELAY, L10N_MENU_FINDDELAY);
	InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_FINDALL, L10N_MENU_FINDALL);
	InsertMenu(hMenu, -1, MF_BYPOSITION|MF_SEPARATOR, 0, NULL);
	
	//Update
	if (update) {
		InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_UPDATE, L10N_MENU_UPDATE);
		InsertMenu(hMenu, -1, MF_BYPOSITION|MF_SEPARATOR, 0, NULL);
	}
	
	//About
	InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_ABOUT, L10N_MENU_ABOUT);
	
	//Exit
	InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_EXIT, L10N_MENU_EXIT);

	//Track menu
	SetForegroundWindow(hwnd);
	TrackPopupMenu(hMenu, TPM_BOTTOMALIGN, pt.x, pt.y, 0, hwnd, NULL);
	DestroyMenu(hMenu);
}

int UpdateTray() {
	if (Shell_NotifyIcon((tray_added?NIM_MODIFY:NIM_ADD),&traydata) == FALSE) {
		Error(L"Shell_NotifyIcon(NIM_ADD/NIM_MODIFY)",L"Failed to add tray icon.",GetLastError(),__LINE__);
		return 1;
	}
	
	//Success
	tray_added=1;
	return 0;
}

int RemoveTray() {
	if (!tray_added) {
		//Tray not added
		return 1;
	}
	
	if (Shell_NotifyIcon(NIM_DELETE,&traydata) == FALSE) {
		Error(L"Shell_NotifyIcon(NIM_DELETE)",L"Failed to remove tray icon.",GetLastError(),__LINE__);
		return 1;
	}
	
	//Success
	tray_added=0;
	return 0;
}

LRESULT CALLBACK WndDetailsMsgProc(INT nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HCBT_ACTIVATE) {
		//Edit the caption of the buttons
		SetDlgItemText((HWND)wParam,IDYES,L"Copy");
		SetDlgItemText((HWND)wParam,IDNO,L"OK");
	}
	return 0;
}

DWORD WINAPI FindWnd(LPVOID arg) {
	POINT pt=*(POINT*)arg;
	
	HWND hwnd_component, hwnd;
	if ((hwnd_component=WindowFromPoint(pt)) == NULL) {
		#ifdef DEBUG
		Error(L"WindowFromPoint()",L"GetWindows()",GetLastError(),__LINE__);
		#endif
	}
	hwnd=GetAncestor(hwnd_component,GA_ROOT);
	
	//Get child window
	RECT wnd;
	if (GetWindowRect(hwnd,&wnd) == 0) {
		#ifdef DEBUG
		Error(L"GetWindowRect()",L"MoveWnd()",GetLastError(),__LINE__);
		#endif
	}
	POINT pt_child;
	pt_child.x=pt.x-wnd.left;
	pt_child.y=pt.y-wnd.top;
	HWND hwnd_child;
	if ((hwnd_child=ChildWindowFromPoint(hwnd,pt_child)) == NULL) {
		#ifdef DEBUG
		Error(L"ChildWindowFromPoint()",L"GetWindows()",GetLastError(),__LINE__);
		#endif
	}
	
	//Get title and class
	wchar_t title[256], title_child[256], title_component[256];
	wchar_t classname[256], classname_child[256], classname_component[256];
	GetWindowText(hwnd,title,sizeof(title)/sizeof(wchar_t));
	GetClassName(hwnd,classname,sizeof(classname)/sizeof(wchar_t));
	GetWindowText(hwnd_child,title_child,sizeof(title_child)/sizeof(wchar_t));
	GetClassName(hwnd_child,classname_child,sizeof(classname_child)/sizeof(wchar_t));
	GetWindowText(hwnd_component,title_component,sizeof(title_component)/sizeof(wchar_t));
	GetClassName(hwnd_component,classname_component,sizeof(classname_component)/sizeof(wchar_t));
	
	//Show message
	swprintf(txt,L"Window:\n title: %s\n class: %s",title,classname);
	if (hwnd_child != hwnd) {
		swprintf(txt,L"%s\n\nChild:\n title: %s\n class: %s",txt,title_child,classname_child);
	}
	if (hwnd_component != hwnd_child && hwnd_component != hwnd) {
		swprintf(txt,L"%s\n\nComponent:\n title: %s\n class: %s",txt,title_component,classname_component);
	}
	HHOOK hhk=SetWindowsHookEx(WH_CBT, &WndDetailsMsgProc, 0, GetCurrentThreadId());
	int response=MessageBox(NULL, txt, L10N_WNDDETAILS, MB_ICONINFORMATION|MB_YESNO|MB_DEFBUTTON2);
	UnhookWindowsHookEx(hhk);
	if (response == IDYES) {
		//Copy message to clipboard
		OpenClipboard(NULL);
		EmptyClipboard();
		wchar_t *data=LocalAlloc(LMEM_FIXED,(wcslen(txt)+1)*sizeof(wchar_t));
		memcpy(data,txt,(wcslen(txt)+1)*sizeof(wchar_t));
		SetClipboardData(CF_UNICODETEXT,data);
		CloseClipboard();
	}
	
	free(arg);
}

static int wnds_alloc=0;
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	//Make sure we have enough space allocated
	if (numwnds == wnds_alloc) {
		wnds_alloc+=100;
		wnds=realloc(wnds,wnds_alloc*sizeof(HWND));
	}
	//Store window if it's visible
	if (IsWindowVisible(hwnd)) {
		wnds[numwnds++]=hwnd;
	}
	return TRUE;
}

void FindAllWnds() {
	//Enumerate windows
	numwnds=0;
	EnumWindows(EnumWindowsProc,0);
	wchar_t title[256];
	wchar_t classname[256];
	int i;
	swprintf(txt,L"");
	for (i=0; i < numwnds; i++) {
		GetWindowText(wnds[i],title,sizeof(title)/sizeof(wchar_t));
		GetClassName(wnds[i],classname,sizeof(classname)/sizeof(wchar_t));
		swprintf(txt,L"%s%s|%s",txt,title,classname);
		if (i+1 < numwnds) {
			wcscat(txt,L"\n");
		}
	}
	
	//Show message
	HHOOK hhk=SetWindowsHookEx(WH_CBT, &WndDetailsMsgProc, 0, GetCurrentThreadId());
	int response=MessageBox(NULL, txt, L10N_ALLWNDS, MB_ICONINFORMATION|MB_YESNO|MB_DEFBUTTON2);
	UnhookWindowsHookEx(hhk);
	if (response == IDYES) {
		//Copy message to clipboard
		OpenClipboard(NULL);
		EmptyClipboard();
		wchar_t *data=LocalAlloc(LMEM_FIXED,(wcslen(txt)+1)*sizeof(wchar_t));
		memcpy(data,txt,(wcslen(txt)+1)*sizeof(wchar_t));
		SetClipboardData(CF_UNICODETEXT,data);
		CloseClipboard();
	}
}

//Hooks
_declspec(dllexport) LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		if (wParam == WM_LBUTTONDOWN) {
			POINT pt=((PMSLLHOOKSTRUCT)lParam)->pt;
			
			//Make sure cursorwnd isn't in the way
			ShowWindow(cursorwnd,SW_HIDE);
			SetWindowLongPtr(cursorwnd,GWL_EXSTYLE,WS_EX_TOOLWINDOW); //Workaround for http://support.microsoft.com/kb/270624/
			
			//Print window info
			POINT *p_pt=malloc(sizeof(pt));
			*p_pt=pt;
			CreateThread(NULL,0,FindWnd,p_pt,0,NULL);
			
			//Unhook mouse
			UnhookMouse();
			
			//Prevent mousedown from propagating
			return 1;
		}
		else if (wParam == WM_RBUTTONDOWN) {
			//Unhook mouse
			UnhookMouse();
			
			//Prevent mousedown from propagating (this won't have any effect since the hook is removed by now)
			return 1;
		}
	}
	
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int HookMouse() {
	if (mousehook) {
		//Mouse already hooked
		return 1;
	}
	
	//Load library
	wchar_t path[MAX_PATH];
	GetModuleFileName(NULL,path,sizeof(path)/sizeof(wchar_t));
	if ((hinstDLL=LoadLibrary(path)) == NULL) {
		Error(L"LoadLibrary()",L"Check the "APP_NAME" website if there is an update, if the latest version doesn't fix this, please report it.",GetLastError(),__LINE__);
		return 1;
	}
	
	//Get address to mouse hook (beware name mangling)
	HOOKPROC procaddr;
	if ((procaddr=(HOOKPROC)GetProcAddress(hinstDLL,"LowLevelMouseProc@12")) == NULL) {
		Error(L"GetProcAddress('LowLevelMouseProc@12')",L"Check the "APP_NAME" website if there is an update, if the latest version doesn't fix this, please report it.",GetLastError(),__LINE__);
		return 1;
	}
	
	//Set up the hook
	if ((mousehook=SetWindowsHookEx(WH_MOUSE_LL,procaddr,hinstDLL,0)) == NULL) {
		Error(L"SetWindowsHookEx(WH_MOUSE_LL)",L"Check the "APP_NAME" website if there is an update, if the latest version doesn't fix this, please report it.",GetLastError(),__LINE__);
		return 1;
	}
	
	//Show cursor
	RECT desktop;
	if (GetWindowRect(GetDesktopWindow(),&desktop) == 0) {
		#ifdef DEBUG
		Error(L"GetWindowRect(GetDesktopWindow())",L"HookMouse()",GetLastError(),__LINE__);
		#endif
	}
	MoveWindow(cursorwnd,desktop.left,desktop.top,desktop.right-desktop.left,desktop.bottom-desktop.top,FALSE);
	SetWindowLongPtr(cursorwnd,GWL_EXSTYLE,WS_EX_LAYERED|WS_EX_TOOLWINDOW); //Workaround for http://support.microsoft.com/kb/270624/
	if (winxp) {
		SetLayeredWindowAttributes(cursorwnd,0,1,LWA_ALPHA); //Almost transparent (XP fix)
	}
	ShowWindowAsync(cursorwnd,SW_SHOWNA);
	
	//Success
	return 0;
}

int UnhookMouse() {
	if (!mousehook) {
		//Mouse not hooked
		return 1;
	}
	
	//Unhook the mouse hook
	if (UnhookWindowsHookEx(mousehook) == 0) {
		#ifdef DEBUG
		Error(L"UnhookWindowsHookEx(mousehook)",L"UnhookMouse()",GetLastError(),__LINE__);
		#endif
		return 1;
	}
	
	//Hide cursor
	ShowWindow(cursorwnd,SW_HIDE);
	SetWindowLongPtr(cursorwnd,GWL_EXSTYLE,WS_EX_TOOLWINDOW); //Workaround for http://support.microsoft.com/kb/270624/
	
	//Success
	mousehook=NULL;
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_ICONTRAY) {
		if (lParam == WM_LBUTTONDOWN) {
			HookMouse();
		}
		else if (lParam == WM_RBUTTONDOWN) {
			ShowContextMenu(hwnd);
		}
		else if (lParam == NIN_BALLOONUSERCLICK) {
			SendMessage(hwnd,WM_COMMAND,SWM_UPDATE,0);
		}
	}
	else if (msg == WM_TASKBARCREATED) {
		tray_added=0;
		UpdateTray();
	}
	else if (msg == WM_COMMAND) {
		int wmId=LOWORD(wParam), wmEvent=HIWORD(wParam);
		if (wmId == SWM_FIND) {
			HookMouse();
		}
		else if (wmId == SWM_FINDDELAY) {
			Sleep(3000);
			HookMouse();
		}
		else if (wmId == SWM_FINDALL) {
			FindAllWnds();
		}
		else if (wmId == SWM_UPDATE) {
			if (MessageBox(NULL, L10N_UPDATE_DIALOG, APP_NAME, MB_ICONINFORMATION|MB_YESNO) == IDYES) {
				ShellExecute(NULL, L"open", APP_URL, NULL, NULL, SW_SHOWNORMAL);
			}
		}
		else if (wmId == SWM_ABOUT) {
			MessageBox(NULL, L10N_ABOUT, L10N_ABOUT_TITLE, MB_ICONINFORMATION|MB_OK);
		}
		else if (wmId == SWM_EXIT) {
			DestroyWindow(hwnd);
		}
	}
	else if (msg == WM_DESTROY) {
		showerror=0;
		UnhookMouse();
		RemoveTray();
		PostQuitMessage(0);
		return 0;
	}
	else if (msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN) {
		//Hide the window if clicked on, this might happen if it wasn't hidden by the hooks for some reason
		ShowWindow(hwnd,SW_HIDE);
		SetWindowLongPtr(hwnd,GWL_EXSTYLE,WS_EX_TOOLWINDOW); //Workaround for http://support.microsoft.com/kb/270624/
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
