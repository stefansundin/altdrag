/*
	AltDrag - Drag windows with the mouse when pressing the alt key
	Copyright (C) 2008  Stefan Sundin (recover89@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

//Tray messages
#define WM_ICONTRAY            WM_USER+1
#define SWM_TOGGLE             WM_APP+1
#define SWM_HIDE               WM_APP+2
#define SWM_AUTOSTART_ON       WM_APP+3
#define SWM_AUTOSTART_OFF      WM_APP+4
#define SWM_AUTOSTART_HIDE_ON  WM_APP+5
#define SWM_AUTOSTART_HIDE_OFF WM_APP+6
#define SWM_ABOUT              WM_APP+7
#define SWM_EXIT               WM_APP+8

//Stuff
LRESULT CALLBACK MyWndProc(HWND, UINT, WPARAM, LPARAM);

//Hook data
static HINSTANCE hinstDLL;
static HHOOK keyhook, mousehook;

//Global info
static HICON icon[2];
static NOTIFYICONDATA traydata;
static UINT WM_TASKBARCREATED;
static int tray_added=0;
static int hook_installed=0;
static int hide=0;

static char msg[100];

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
	//Check command line
	if (!strcmp(szCmdLine,"-hide")) {
		hide=1;
	}

	//Create window class
	WNDCLASS wnd;
	wnd.style=CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc=MyWndProc;
	wnd.cbClsExtra=0;
	wnd.cbWndExtra=0;
	wnd.hInstance=hInst;
	wnd.hIcon=LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	wnd.hCursor=LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR);
	wnd.hbrBackground=(HBRUSH)(COLOR_BACKGROUND+1);
	wnd.lpszMenuName=NULL;
	wnd.lpszClassName="AltDrag";
	
	//Register class
	if (RegisterClass(&wnd) == 0) {
		sprintf(msg,"RegisterClass() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Error", MB_ICONERROR|MB_OK);
		return 1;
	}
	
	//Create window
	HWND hWnd;
	hWnd=CreateWindow(wnd.lpszClassName, wnd.lpszClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);
	//ShowWindow(hWnd, iCmdShow); //Show
	//UpdateWindow(hWnd); //Update

	//Register TaskbarCreated so we can readd the tray icon if explorer.exe crashes
	if ((WM_TASKBARCREATED=RegisterWindowMessage("TaskbarCreated")) == 0) {
		sprintf(msg,"RegisterWindowMessage() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
	}
	
	//Load tray icons
	if ((icon[0] = LoadImage(hInst, "tray-disabled", IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR)) == NULL) {
		sprintf(msg,"LoadImage() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Error", MB_ICONERROR|MB_OK);
		PostQuitMessage(1);
	}
	if ((icon[1] = LoadImage(hInst, "tray-enabled", IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR)) == NULL) {
		sprintf(msg,"LoadImage() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Error", MB_ICONERROR|MB_OK);
		PostQuitMessage(1);
	}
	
	//Create icondata
	traydata.cbSize=sizeof(NOTIFYICONDATA);
	traydata.uID=0;
	traydata.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;
	traydata.hWnd=hWnd;
	traydata.uCallbackMessage=WM_ICONTRAY;
	
	//Add tray icon
	if (!hide) {
		UpdateTray();
	}
	
	//Install hook
	InstallHook();
	
	if (!hook_installed && hide) {
		PostQuitMessage(1);
	}
	
	//Message loop
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

void ShowContextMenu(HWND hWnd) {
	POINT pt;
	GetCursorPos(&pt);
	HMENU hMenu, hAutostartMenu;
	if ((hMenu = CreatePopupMenu()) == NULL) {
		sprintf(msg,"CreatePopupMenu() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
	}
	
	//Toggle
	InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_TOGGLE, (hook_installed?"Disable":"Enable"));
	//InsertMenu(hMenu, -1, MF_BYPOSITION|MF_SEPARATOR, SWM_ABOUT, "");
	
	//Hide
	InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_HIDE, "Hide tray");
	
	//Autostart
	//Check registry
	int autostart_enabled=0, autostart_hide=0;
	//Open key
	HKEY key;
	if (RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_QUERY_VALUE,&key) != ERROR_SUCCESS) {
		sprintf(msg,"RegOpenKeyEx() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
	}
	//Read value
	char autostart_value[MAX_PATH+10];
	DWORD len=sizeof(autostart_value);
	DWORD res=RegQueryValueEx(key,"AltDrag",NULL,NULL,(LPBYTE)autostart_value,&len);
	if (res != ERROR_FILE_NOT_FOUND && res != ERROR_SUCCESS) {
		sprintf(msg,"RegQueryValueEx() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
	}
	//Close key
	if (RegCloseKey(key) != ERROR_SUCCESS) {
		sprintf(msg,"RegCloseKey() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
	}
	//Get path
	char path[MAX_PATH];
	if (GetModuleFileName(NULL,path,sizeof(path)) == 0) {
		sprintf(msg,"GetModuleFileName() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
	}
	//Compare
	char pathcmp[MAX_PATH+10];
	sprintf(pathcmp,"\"%s\"",path);
	if (!strcmp(pathcmp,autostart_value)) {
		autostart_enabled=1;
	}
	sprintf(pathcmp,"\"%s\" -hide",path);
	if (!strcmp(pathcmp,autostart_value)) {
		autostart_enabled=1;
		autostart_hide=1;
	}
	
	if ((hAutostartMenu = CreatePopupMenu()) == NULL) {
		sprintf(msg,"CreatePopupMenu() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
	}
	InsertMenu(hAutostartMenu, -1, MF_BYPOSITION|(autostart_enabled?MF_CHECKED:0), (autostart_enabled?SWM_AUTOSTART_OFF:SWM_AUTOSTART_ON), "Autostart");
	InsertMenu(hAutostartMenu, -1, MF_BYPOSITION|(autostart_hide?MF_CHECKED:0), (autostart_hide?SWM_AUTOSTART_HIDE_OFF:SWM_AUTOSTART_HIDE_ON), "Hide tray");
	InsertMenu(hMenu, -1, MF_BYPOSITION|MF_POPUP, (UINT)hAutostartMenu, "Autostart");
	InsertMenu(hMenu, -1, MF_BYPOSITION|MF_SEPARATOR, SWM_ABOUT, "");
	
	//About
	InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_ABOUT, "About");
	
	//Exit
	InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_EXIT, "Exit");

	//Must set window to the foreground, or else the menu won't disappear when clicking outside it
	SetForegroundWindow(hWnd);

	TrackPopupMenu(hMenu, TPM_BOTTOMALIGN, pt.x, pt.y, 0, hWnd, NULL );
	DestroyMenu(hMenu);
}

int UpdateTray() {
	strncpy(traydata.szTip,(hook_installed?"AltDrag (enabled)":"AltDrag (disabled)"),sizeof(traydata.szTip));
	traydata.hIcon=icon[hook_installed];
	
	if (Shell_NotifyIcon((tray_added?NIM_MODIFY:NIM_ADD),&traydata) == FALSE) {
		sprintf(msg,"Shell_NotifyIcon() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "ShutdownGuard Warning", MB_ICONWARNING|MB_OK);
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
		sprintf(msg,"Shell_NotifyIcon() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
		return 1;
	}
	
	//Success
	tray_added=0;
}

int InstallHook() {
	if (hook_installed) {
		//Hook already installed
		return 1;
	}
	
	//Load dll
	if ((hinstDLL=LoadLibrary((LPCTSTR)"hooks.dll")) == NULL) {
		sprintf(msg,"LoadLibrary() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
		return 1;
	}
	
	//Get address to keyboard hook (beware name mangling)
	HOOKPROC procaddr;
	if ((procaddr=(HOOKPROC)GetProcAddress(hinstDLL,"KeyboardProc@12")) == NULL) {
		sprintf(msg,"GetProcAddress() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
		return 1;
	}
	
	//Set up the keyboard hook
	if ((keyhook=SetWindowsHookEx(WH_KEYBOARD_LL,procaddr,hinstDLL,0)) == NULL) {
		sprintf(msg,"SetWindowsHookEx() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
		return 1;
	}
	
	//Get address to mouse hook (beware name mangling)
	if ((procaddr=(HOOKPROC)GetProcAddress(hinstDLL,"MouseProc@12")) == NULL) {
		sprintf(msg,"GetProcAddress() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
		return 1;
	}
	
	//Set up the mouse hook
	if ((mousehook=SetWindowsHookEx(WH_MOUSE_LL,procaddr,hinstDLL,0)) == NULL) {
		sprintf(msg,"SetWindowsHookEx() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
		return 1;
	}
	
	//Success
	hook_installed=1;
	if (!hide) {
		UpdateTray();
	}
	return 0;
}

int RemoveHook() {
	if (!hook_installed) {
		//Hook not installed
		return 1;
	}
	
	//Remove keyboard hook
	if (UnhookWindowsHookEx(keyhook) == 0) {
		sprintf(msg,"UnhookWindowsHookEx() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
		return 1;
	}
	
	//Remove mouse hook
	if (UnhookWindowsHookEx(mousehook) == 0) {
		sprintf(msg,"UnhookWindowsHookEx() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
		return 1;
	}
	
	//Unload dll
	if (FreeLibrary(hinstDLL) == 0) {
		sprintf(msg,"FreeLibrary() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
		return 1;
	}
	
	//Success
	hook_installed=0;
	if (!hide) {
		UpdateTray();
	}
	return 0;
}

void ToggleHook() {
	if (hook_installed) {
		RemoveHook();
	}
	else {
		InstallHook();
	}
}

void SetAutostart(int on, int hide) {
	//Open key
	HKEY key;
	if (RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_SET_VALUE,&key) != ERROR_SUCCESS) {
		sprintf(msg,"RegOpenKeyEx() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
		return;
	}
	if (on) {
		//Get path
		char path[MAX_PATH];
		if (GetModuleFileName(NULL,path,sizeof(path)) == 0) {
			sprintf(msg,"GetModuleFileName() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
			MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
			return;
		}
		//Add
		char value[MAX_PATH+10];
		sprintf(value,(hide?"\"%s\" -hide":"\"%s\""),path);
		if (RegSetValueEx(key,"AltDrag",0,REG_SZ,value,strlen(value)+1) != ERROR_SUCCESS) {
			sprintf(msg,"RegSetValueEx() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
			MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
			return;
		}
	}
	else {
		//Remove
		if (RegDeleteValue(key,"AltDrag") != ERROR_SUCCESS) {
			sprintf(msg,"RegDeleteValue() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
			MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
			return;
		}
	}
	//Close key
	if (RegCloseKey(key) != ERROR_SUCCESS) {
		sprintf(msg,"RegCloseKey() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
		return;
	}
}

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_COMMAND) {
		int wmId=LOWORD(wParam), wmEvent=HIWORD(wParam);
		if (wmId == SWM_TOGGLE) {
			ToggleHook();
		}
		else if (wmId == SWM_HIDE) {
			hide=1;
			RemoveTray();
		}
		else if (wmId == SWM_AUTOSTART_ON) {
			SetAutostart(1,0);
		}
		else if (wmId == SWM_AUTOSTART_OFF) {
			SetAutostart(0,0);
		}
		else if (wmId == SWM_AUTOSTART_HIDE_ON) {
			SetAutostart(1,1);
		}
		else if (wmId == SWM_AUTOSTART_HIDE_OFF) {
			SetAutostart(1,0);
		}
		else if (wmId == SWM_ABOUT) {
			MessageBox(NULL, "AltDrag - 0.1\nhttp://altdrag.googlecode.com/\nrecover89@gmail.com\n\nDrag windows with the mouse when pressing the alt key.\n\nYou can use -hide as a parameter to hide the tray icon.\n\nSend feedback to recover89@gmail.com", "About AltDrag", MB_ICONINFORMATION|MB_OK);
		}
		else if (wmId == SWM_EXIT) {
			DestroyWindow(hWnd);
		}
	}
	else if (msg == WM_ICONTRAY) {
		if (lParam == WM_LBUTTONDOWN) {
			ToggleHook();
		}
		else if (lParam == WM_RBUTTONDOWN) {
			ShowContextMenu(hWnd);
		}
	}
	else if (msg == WM_TASKBARCREATED) {
		tray_added=0;
		if (!hide) {
			UpdateTray();
		}
	}
	else if (msg == WM_DESTROY) {
		if (hook_installed) {
			RemoveHook();
		}
		if (tray_added) {
			RemoveTray();
		}
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
