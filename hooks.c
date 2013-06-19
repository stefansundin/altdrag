/*
	Copyright (C) 2012  Stefan Sundin (recover89@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

#define UNICODE
#define _UNICODE
#define _WIN32_WINNT 0x0600
#define COBJMACROS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <shlwapi.h>
#include <commctrl.h>
#include <psapi.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

//Stuff missing in MinGW
#define WM_MOUSEHWHEEL 0x020E //WM_MOUSEHWHEEL is only defined for >= Vista
CLSID my_CLSID_MMDeviceEnumerator = {0xBCDE0395,0xE52F,0x467C,{0x8E,0x3D,0xC4,0x57,0x92,0x91,0x69,0x2E}};
GUID my_IID_IMMDeviceEnumerator = {0xA95664D2,0x9614,0x4F35,{0xA7,0x46,0xDE,0x8D,0xB6,0x36,0x17,0xE6}};
GUID my_IID_IAudioEndpointVolume = {0x5CDF2C82,0x841E,0x4546,{0x97,0x22,0x0C,0xF7,0x40,0x78,0x22,0x9A}};

//App
#define APP_NAME L"AltDrag"
#define AERO_THRESHOLD 5

//Timers
HWND g_hwnd;
#define RESTORE_TIMER WM_APP+1
#define MOVE_TIMER    WM_APP+2
#define REHOOK_TIMER  WM_APP+3
#define INIT_TIMER    WM_APP+4
#define FOCUS_TIMER   WM_APP+5

//Enumerators
enum action {ACTION_NONE=0, ACTION_MOVE, ACTION_RESIZE, ACTION_MINIMIZE, ACTION_CENTER, ACTION_ALWAYSONTOP, ACTION_CLOSE, ACTION_LOWER, ACTION_ALTTAB, ACTION_VOLUME};
enum button {BUTTON_NONE=0, BUTTON_LMB, BUTTON_MMB, BUTTON_RMB, BUTTON_MB4, BUTTON_MB5};
enum resize {RESIZE_NONE=0, RESIZE_TOP, RESIZE_RIGHT, RESIZE_BOTTOM, RESIZE_LEFT, RESIZE_CENTER};
enum cursor {HAND, SIZENWSE, SIZENESW, SIZENS, SIZEWE, SIZEALL};

//Some variables must be shared so that CallWndProc hooks can access them
#define shareattr __attribute__((section ("shared"), shared))

//Window database
#define NUMWNDDB 30
struct wnddata {
	HWND hwnd;
	short restore;
	int width;
	int height;
	struct {
		int width;
		int height;
	} last;
};
struct {
	struct wnddata items[NUMWNDDB];
	struct wnddata *pos;
} wnddb;

//State
struct {
	HWND hwnd;
	short alt;
	short ctrl;
	short interrupted;
	short updaterate;
	unsigned int clicktime;
	POINT clickpt;
	POINT prevpt;
	POINT offset;
	struct {
		enum resize x, y;
	} resize;
	short blockaltup;
	short blockmouseup;
	short ignorectrl;
	short locked;
	struct wnddata *wndentry;
	struct {
		HMONITOR monitor;
		short maximized;
		int width;
		int height;
		int right;
		int bottom;
	} origin;
	struct {
		POINT ptMinTrackSize;
		POINT ptMaxTrackSize;
	} mmi;
} state;

struct {
	short shift;
	short snap;
	enum action action;
} sharedstate shareattr = {0, 0, ACTION_NONE};

//Snap
RECT *monitors = NULL;
int nummonitors = 0;
RECT *wnds = NULL;
int numwnds = 0;
HWND *hwnds = NULL;
int numhwnds = 0;
HWND progman = NULL;

//Settings
#define MAXKEYS 10
struct {
	int AutoFocus;
	int AutoSnap;
	int AutoRemaximize;
	int Aero;
	int InactiveScroll;
	int SnapThreshold;
	int FocusOnTyping;
	struct {
		int Cursor;
		int MoveRate;
		int ResizeRate;
	} Performance;
	struct {
		unsigned char keys[MAXKEYS];
		int length;
	} Hotkeys;
	struct {
		enum action LMB, MMB, RMB, MB4, MB5, Scroll;
	} Mouse;
} sharedsettings shareattr;
short sharedsettings_loaded shareattr = 0;
wchar_t inipath[MAX_PATH] shareattr;

//Blacklist (not shared since dynamically allocated)
struct blacklistitem {
	wchar_t *title;
	wchar_t *classname;
};
struct blacklist {
	struct blacklistitem *items;
	int length;
	wchar_t *data;
};
struct {
	struct blacklist ProcessBlacklist;
	struct blacklist Blacklist;
	struct blacklist Snaplist;
} settings = {{NULL,0}, {NULL,0}, {NULL,0}};

//Cursor data
HWND cursorwnd shareattr = NULL;
HCURSOR cursors[6];

//Hook data
HINSTANCE hinstDLL = NULL;
HHOOK mousehook = NULL;
HHOOK scrollhook = NULL;

//Msghook data
BOOL subclassed = FALSE;
enum action msgaction shareattr = ACTION_NONE;

//Error()
#ifdef DEBUG
#define ERROR_WRITETOFILE
#include "include/error.c"
#else
#define Error(a,b,c,d,e)
#endif

//Blacklist
int blacklisted(HWND hwnd, struct blacklist *list) {
	wchar_t title[256]=L"", classname[256]=L"";
	int i;
	
	//Do not check if list is empty
	if (list->length == 0) {
		return 0;
	}
	
	//ProcessBlacklist is case-insensitive
	if (list == &settings.ProcessBlacklist) {
		DWORD pid;
		GetWindowThreadProcessId(hwnd, &pid);
		HANDLE proc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
		GetProcessImageFileName(proc, title, sizeof(title)/sizeof(wchar_t));
		CloseHandle(proc);
		PathStripPath(title);
		for (i=0; i < list->length; i++) {
			if (!wcsicmp(title,list->items[i].title)) {
				return 1;
			}
		}
		return 0;
	}
	
	GetWindowText(hwnd, title, sizeof(title)/sizeof(wchar_t));
	GetClassName(hwnd, classname, sizeof(classname)/sizeof(wchar_t));
	for (i=0; i < list->length; i++) {
		if ((list->items[i].title == NULL && !wcscmp(classname,list->items[i].classname))
		 || (list->items[i].classname == NULL && !wcscmp(title,list->items[i].title))
		 || (list->items[i].title != NULL && list->items[i].classname != NULL && !wcscmp(title,list->items[i].title) && !wcscmp(classname,list->items[i].classname))) {
			return 1;
		}
	}
	return 0;
}

//Enumerate
int monitors_alloc = 0;
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	//Make sure we have enough space allocated
	if (nummonitors == monitors_alloc) {
		monitors_alloc++;
		monitors = realloc(monitors, monitors_alloc*sizeof(RECT));
	}
	//Add monitor
	monitors[nummonitors++] = *lprcMonitor;
	return TRUE;
}

int wnds_alloc = 0;
BOOL CALLBACK EnumWindowsProc(HWND window, LPARAM lParam) {
	//Make sure we have enough space allocated
	if (numwnds == wnds_alloc) {
		wnds_alloc += 20;
		wnds = realloc(wnds, wnds_alloc*sizeof(RECT));
	}
	
	//Only store window if it's visible, not minimized to taskbar, not the window we are dragging and not blacklisted
	RECT wnd;
	if (window != state.hwnd && window != progman
	 && IsWindowVisible(window) && !IsIconic(window)
	 && (GetWindowLongPtr(window,GWL_STYLE)&WS_CAPTION || blacklisted(window,&settings.Snaplist))
	 && GetWindowRect(window,&wnd) != 0
	) {
		//Maximized?
		if (IsZoomed(window)) {
			//Get monitor size
			HMONITOR monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
			MONITORINFO monitorinfo = { sizeof(MONITORINFO) };
			GetMonitorInfo(monitor, &monitorinfo);
			RECT mon = monitorinfo.rcMonitor;
			//Crop this window so that it does not exceed the size of the monitor
			//I do this since the window has an extra, invisible, border when maximized (a border that stretches onto other monitors)
			wnd.left = (wnd.left < mon.left)?mon.left:wnd.left;
			wnd.top = (wnd.top < mon.top)?mon.top:wnd.top;
			wnd.right = (wnd.right > mon.right)?mon.right:wnd.right;
			wnd.bottom = (wnd.bottom > mon.bottom)?mon.bottom:wnd.bottom;
		}
		
		//Return if this window is overlapped by another window
		int i;
		for (i=0; i < numwnds; i++) {
			if (wnd.left >= wnds[i].left && wnd.top >= wnds[i].top && wnd.right <= wnds[i].right && wnd.bottom <= wnds[i].bottom) {
				return TRUE;
			}
		}
		
		//Add window
		wnds[numwnds++] = wnd;
		
		//Use this to print the title and classname of the windows that are snapable
		/*FILE *f = fopen("C:\\altdrag-log.txt", "ab");
		char title[100], classname[100];
		GetWindowTextA(window, title, 100);
		GetClassNameA(window, classname, 100);
		fprintf(f, "window: %s|%s\n", title, classname);
		fclose(f);*/
	}
	return TRUE;
}

int hwnds_alloc = 0;
BOOL CALLBACK EnumAltTabWindows(HWND window, LPARAM lParam) {
	//Make sure we have enough space allocated
	if (numhwnds == hwnds_alloc) {
		hwnds_alloc += 20;
		hwnds = realloc(hwnds, hwnds_alloc*sizeof(HWND));
	}
	
	//Only store window if it's visible, not minimized to taskbar and on the same monitor as the cursor
	if (IsWindowVisible(window) && !IsIconic(window)
	 && GetWindowLongPtr(window,GWL_STYLE)&WS_CAPTION
	 && state.origin.monitor == MonitorFromWindow(window,MONITOR_DEFAULTTONULL)
	) {
		hwnds[numhwnds++] = window;
	}
	return TRUE;
}

void Enum() {
	//Update handle to progman
	if (!IsWindow(progman)) {
		progman = FindWindow(L"Progman", L"Program Manager");
	}
	
	//Enumerate monitors
	nummonitors = 0;
	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);
	
	//Enumerate windows
	numwnds = 0;
	if (sharedstate.snap) {
		HWND taskbar = FindWindow(L"Shell_TrayWnd", NULL);
		RECT wnd;
		if (taskbar != NULL && GetWindowRect(taskbar,&wnd) != 0) {
			wnds[numwnds++] = wnd;
		}
		if (sharedstate.snap >= 2) {
			EnumWindows(EnumWindowsProc, 0);
		}
	}
	
	//Use this to print the monitors and windows
	/*FILE *f = fopen("C:\\altdrag-log.txt", "wb");
	fprintf(f, "nummonitors: %d\n", nummonitors);
	int k;
	for (k=0; k < nummonitors; k++) {
		fprintf(f, "mon #%02d: left %d, top %d, right %d, bottom %d\n", k, monitors[k].left, monitors[k].top, monitors[k].right, monitors[k].bottom);
	}
	fprintf(f, "\n");
	fprintf(f, "numwnds: %d\n", numwnds);
	//char title[100], classname[100];
	for (k=0; k < numwnds; k++) {
		//GetWindowTextA(wnds[k], title, 100);
		//GetClassNameA(wnds[k], classname, 100);
		//RECT wnd;
		//GetWindowRect(wnds[k], &wnd);
		//fprintf(f, "wnd #%02d: %s [%s] (%dx%d @ %dx%d)\n", k, title, classname, wnd.right-wnd.left, wnd.bottom-wnd.top, wnd.left, wnd.top);
		fprintf(f, "wnd #%02d: %dx%d @ %dx%d\n", k, wnds[k].right-wnds[k].left, wnds[k].bottom-wnds[k].top, wnds[k].left, wnds[k].top);
	}
	fclose(f);*/
}

void MoveSnap(int *posx, int *posy, int wndwidth, int wndheight) {
	//Enumerate monitors and windows
	Enum();
	
	//thresholdx and thresholdy will shrink to make sure the dragged window will snap to the closest windows
	int i, j, thresholdx, thresholdy, stuckx=0, stucky=0, stickx=0, sticky=0;
	thresholdx = thresholdy = sharedsettings.SnapThreshold;
	//Loop monitors and windows
	for (i=0, j=0; i < nummonitors || j < numwnds; ) {
		RECT snapwnd;
		int snapinside;
		
		//Get snapwnd
		if (i < nummonitors) {
			snapwnd = monitors[i];
			snapinside = 1;
			i++;
		}
		else if (j < numwnds) {
			snapwnd = wnds[j];
			snapinside = (sharedstate.snap != 2);
			j++;
		}
		
		//Check if posx snaps
		if ((snapwnd.top-thresholdx < *posy && *posy < snapwnd.bottom+thresholdx)
		 || (*posy-thresholdx < snapwnd.top && snapwnd.top < *posy+wndheight+thresholdx)) {
			int snapinside_cond = (snapinside || *posy+wndheight-thresholdx < snapwnd.top || snapwnd.bottom < *posy+thresholdx);
			if (*posx-thresholdx < snapwnd.right && snapwnd.right < *posx+thresholdx) {
				//The left edge of the dragged window will snap to this window's right edge
				stuckx = 1;
				stickx = snapwnd.right;
				thresholdx = snapwnd.right-*posx;
			}
			else if (snapinside_cond && *posx+wndwidth-thresholdx < snapwnd.right && snapwnd.right < *posx+wndwidth+thresholdx) {
				//The right edge of the dragged window will snap to this window's right edge
				stuckx = 1;
				stickx = snapwnd.right-wndwidth;
				thresholdx = snapwnd.right-(*posx+wndwidth);
			}
			else if (snapinside_cond && *posx-thresholdx < snapwnd.left && snapwnd.left < *posx+thresholdx) {
				//The left edge of the dragged window will snap to this window's left edge
				stuckx = 1;
				stickx = snapwnd.left;
				thresholdx = snapwnd.left-*posx;
			}
			else if (*posx+wndwidth-thresholdx < snapwnd.left && snapwnd.left < *posx+wndwidth+thresholdx) {
				//The right edge of the dragged window will snap to this window's left edge
				stuckx = 1;
				stickx = snapwnd.left-wndwidth;
				thresholdx = snapwnd.left-(*posx+wndwidth);
			}
		}
		
		//Check if posy snaps
		if ((snapwnd.left-thresholdy < *posx && *posx < snapwnd.right+thresholdy)
		 || (*posx-thresholdy < snapwnd.left && snapwnd.left < *posx+wndwidth+thresholdy)) {
			int snapinside_cond = (snapinside || *posx+wndwidth-thresholdy < snapwnd.left || snapwnd.right < *posx+thresholdy);
			if (*posy-thresholdy < snapwnd.bottom && snapwnd.bottom < *posy+thresholdy) {
				//The top edge of the dragged window will snap to this window's bottom edge
				stucky = 1;
				sticky = snapwnd.bottom;
				thresholdy = snapwnd.bottom-*posy;
			}
			else if (snapinside_cond && *posy+wndheight-thresholdy < snapwnd.bottom && snapwnd.bottom < *posy+wndheight+thresholdy) {
				//The bottom edge of the dragged window will snap to this window's bottom edge
				stucky = 1;
				sticky = snapwnd.bottom-wndheight;
				thresholdy = snapwnd.bottom-(*posy+wndheight);
			}
			else if (snapinside_cond && *posy-thresholdy < snapwnd.top && snapwnd.top < *posy+thresholdy) {
				//The top edge of the dragged window will snap to this window's top edge
				stucky = 1;
				sticky = snapwnd.top;
				thresholdy = snapwnd.top-*posy;
			}
			else if (*posy+wndheight-thresholdy < snapwnd.top && snapwnd.top < *posy+wndheight+thresholdy) {
				//The bottom edge of the dragged window will snap to this window's top edge
				stucky = 1;
				sticky = snapwnd.top-wndheight;
				thresholdy = snapwnd.top-(*posy+wndheight);
			}
		}
	}
	
	//Update posx and posy
	if (stuckx) {
		*posx = stickx;
	}
	if (stucky) {
		*posy = sticky;
	}
}

void ResizeSnap(int *posx, int *posy, int *wndwidth, int *wndheight) {
	//Enumerate monitors and windows
	Enum();
	
	//thresholdx and thresholdy will shrink to make sure the dragged window will snap to the closest windows
	int i, j, thresholdx, thresholdy, stuckleft=0, stucktop=0, stuckright=0, stuckbottom=0, stickleft=0, sticktop=0, stickright=0, stickbottom=0;
	thresholdx = thresholdy = sharedsettings.SnapThreshold;
	//Loop monitors and windows
	for (i=0, j=0; i < nummonitors || j < numwnds; ) {
		RECT snapwnd;
		int snapinside;
		
		//Get snapwnd
		if (i < nummonitors) {
			snapwnd = monitors[i];
			snapinside = 1;
			i++;
		}
		else if (j < numwnds) {
			snapwnd = wnds[j];
			snapinside = (sharedstate.snap != 2);
			j++;
		}
		
		//Check if posx snaps
		if ((snapwnd.top-thresholdx < *posy && *posy < snapwnd.bottom+thresholdx)
		 || (*posy-thresholdx < snapwnd.top && snapwnd.top < *posy+*wndheight+thresholdx)) {
			int snapinside_cond = (snapinside || *posy+*wndheight-thresholdx < snapwnd.top || snapwnd.bottom < *posy+thresholdx);
			if (state.resize.x == RESIZE_LEFT && *posx-thresholdx < snapwnd.right && snapwnd.right < *posx+thresholdx) {
				//The left edge of the dragged window will snap to this window's right edge
				stuckleft = 1;
				stickleft = snapwnd.right;
				thresholdx = snapwnd.right-*posx;
			}
			else if (snapinside_cond && state.resize.x == RESIZE_RIGHT && *posx+*wndwidth-thresholdx < snapwnd.right && snapwnd.right < *posx+*wndwidth+thresholdx) {
				//The right edge of the dragged window will snap to this window's right edge
				stuckright = 1;
				stickright = snapwnd.right;
				thresholdx = snapwnd.right-(*posx+*wndwidth);
			}
			else if (snapinside_cond && state.resize.x == RESIZE_LEFT && *posx-thresholdx < snapwnd.left && snapwnd.left < *posx+thresholdx) {
				//The left edge of the dragged window will snap to this window's left edge
				stuckleft = 1;
				stickleft = snapwnd.left;
				thresholdx = snapwnd.left-*posx;
			}
			else if (state.resize.x == RESIZE_RIGHT && *posx+*wndwidth-thresholdx < snapwnd.left && snapwnd.left < *posx+*wndwidth+thresholdx) {
				//The right edge of the dragged window will snap to this window's left edge
				stuckright = 1;
				stickright = snapwnd.left;
				thresholdx = snapwnd.left-(*posx+*wndwidth);
			}
		}
		
		//Check if posy snaps
		if ((snapwnd.left-thresholdy < *posx && *posx < snapwnd.right+thresholdy)
		 || (*posx-thresholdy < snapwnd.left && snapwnd.left < *posx+*wndwidth+thresholdy)) {
			int snapinside_cond = (snapinside || *posx+*wndwidth-thresholdy < snapwnd.left || snapwnd.right < *posx+thresholdy);
			if (state.resize.y == RESIZE_TOP && *posy-thresholdy < snapwnd.bottom && snapwnd.bottom < *posy+thresholdy) {
				//The top edge of the dragged window will snap to this window's bottom edge
				stucktop = 1;
				sticktop = snapwnd.bottom;
				thresholdy = snapwnd.bottom-*posy;
			}
			else if (snapinside_cond && state.resize.y == RESIZE_BOTTOM && *posy+*wndheight-thresholdy < snapwnd.bottom && snapwnd.bottom < *posy+*wndheight+thresholdy) {
				//The bottom edge of the dragged window will snap to this window's bottom edge
				stuckbottom = 1;
				stickbottom = snapwnd.bottom;
				thresholdy = snapwnd.bottom-(*posy+*wndheight);
			}
			else if (snapinside_cond && state.resize.y == RESIZE_TOP && *posy-thresholdy < snapwnd.top && snapwnd.top < *posy+thresholdy) {
				//The top edge of the dragged window will snap to this window's top edge
				stucktop = 1;
				sticktop = snapwnd.top;
				thresholdy = snapwnd.top-*posy;
			}
			else if (state.resize.y == RESIZE_BOTTOM && *posy+*wndheight-thresholdy < snapwnd.top && snapwnd.top < *posy+*wndheight+thresholdy) {
				//The bottom edge of the dragged window will snap to this window's top edge
				stuckbottom = 1;
				stickbottom = snapwnd.top;
				thresholdy = snapwnd.top-(*posy+*wndheight);
			}
		}
	}
	
	//Update pos, posy, wndwidth and widthheight
	if (stuckleft) {
		*wndwidth = *wndwidth+*posx-stickleft;
		*posx = stickleft;
	}
	if (stucktop) {
		*wndheight = *wndheight+*posy-sticktop;
		*posy = sticktop;
	}
	if (stuckright) {
		*wndwidth = stickright-*posx;
	}
	if (stuckbottom) {
		*wndheight = stickbottom-*posy;
	}
}

#ifdef _WIN64

//x64 keyhook needs only to check when the shift key is depressed
__declspec(dllexport) LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		int vkey = ((PKBDLLHOOKSTRUCT)lParam)->vkCode;
		
		if (vkey == VK_LSHIFT || vkey == VK_RSHIFT) {
			if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
				sharedstate.shift = 1;
				sharedstate.snap = 3;
			}
			else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
				sharedstate.shift = 0;
				sharedstate.snap = sharedsettings.AutoSnap;
			}
		}
		else if (vkey == VK_SPACE
		     && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
		     && (sharedstate.action || msgaction) && sharedstate.snap) {
			sharedstate.snap = 0;
		}
	}
	
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

#else

//Get action of button
int GetAction(int button) {
	if (button == BUTTON_LMB) return sharedsettings.Mouse.LMB;
	if (button == BUTTON_MMB) return sharedsettings.Mouse.MMB;
	if (button == BUTTON_RMB) return sharedsettings.Mouse.RMB;
	if (button == BUTTON_MB4) return sharedsettings.Mouse.MB4;
	if (button == BUTTON_MB5) return sharedsettings.Mouse.MB5;
	return ACTION_NONE;
}

//Check if key is assigned
int IsHotkey(int key) {
	int i;
	for (i=0; i < sharedsettings.Hotkeys.length; i++) {
		if (key == sharedsettings.Hotkeys.keys[i]) {
			return 1;
		}
	}
	return 0;
}

void MouseMove() {
	int posx, posy, wndwidth, wndheight;
	
	//Make sure we got something to do
	if (sharedstate.action != ACTION_MOVE && sharedstate.action != ACTION_RESIZE) {
		return;
	}
	
	//Check if window still exists
	if (!IsWindow(state.hwnd)) {
		UnhookMouse();
		return;
	}
	
	//Get cursor
	POINT pt;
	GetCursorPos(&pt);
	
	//Restrict pt within origin monitor if Ctrl is being pressed
	if (GetAsyncKeyState(VK_CONTROL)&0x8000 && !state.ignorectrl) {
		MONITORINFO monitorinfo = { sizeof(MONITORINFO) };
		GetMonitorInfo(state.origin.monitor, &monitorinfo);
		RECT fmon = monitorinfo.rcMonitor;
		pt.x = (pt.x<fmon.left)?fmon.left: (pt.x>=fmon.right)?fmon.right-1: pt.x;
		pt.y = (pt.y<fmon.top)?fmon.top: (pt.y>=fmon.bottom)?fmon.bottom-1: pt.y;
	}
	
	//Get window state
	int maximized = IsZoomed(state.hwnd);
	HMONITOR monitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
	
	//AutoRemaximize has priority over locked flag
	if (sharedstate.action == ACTION_MOVE && sharedsettings.AutoRemaximize && state.origin.maximized && monitor != state.origin.monitor) {
		//Get monitor rect
		MONITORINFO monitorinfo = { sizeof(MONITORINFO) };
		GetMonitorInfo(monitor, &monitorinfo);
		RECT mon = monitorinfo.rcWork;
		RECT fmon = monitorinfo.rcMonitor;
		//Center window on monitor and maximize it
		WINDOWPLACEMENT wndpl = { sizeof(WINDOWPLACEMENT) };
		GetWindowPlacement(state.hwnd, &wndpl);
		wndpl.rcNormalPosition.left = fmon.left+(mon.right-mon.left)/2-state.origin.width/2;
		wndpl.rcNormalPosition.top = fmon.top+(mon.bottom-mon.top)/2-state.origin.height/2;
		wndpl.rcNormalPosition.right = wndpl.rcNormalPosition.left+state.origin.width;
		wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top+state.origin.height;
		if (maximized) {
			wndpl.showCmd = SW_RESTORE;
			SetWindowPlacement(state.hwnd, &wndpl);
		}
		wndpl.showCmd = SW_MAXIMIZE;
		SetWindowPlacement(state.hwnd, &wndpl);
		//Set this monitor as the origin (dirty hack maybe)
		state.origin.monitor = monitor;
		//Lock the current state
		state.locked = 1;
		//Restore window after a timeout if AutoRemaximize=2
		if (sharedsettings.AutoRemaximize == 2) {
			SetTimer(g_hwnd, RESTORE_TIMER, 1000, NULL);
		}
		return;
	}
	
	//Return if state is locked
	if (state.locked) {
		return;
	}
	
	//Double check if the shift key is being pressed
	if (sharedstate.shift && !(GetAsyncKeyState(VK_SHIFT)&0x8000)) {
		sharedstate.shift = 0;
		sharedstate.snap = sharedsettings.AutoSnap;
	}
	
	//Get window size
	RECT wnd;
	if (GetWindowRect(state.hwnd,&wnd) == 0) {
		return;
	}
	
	//Get monitor info
	MONITORINFO monitorinfo = { sizeof(MONITORINFO) };
	GetMonitorInfo(monitor, &monitorinfo);
	RECT mon = monitorinfo.rcWork;
	RECT fmon = monitorinfo.rcMonitor;
	
	//Get new position for window
	if (sharedstate.action == ACTION_MOVE) {
		posx = pt.x-state.offset.x;
		posy = pt.y-state.offset.y;
		wndwidth = wnd.right-wnd.left;
		wndheight = wnd.bottom-wnd.top;
		
		//Aero Snap
		if (sharedsettings.Aero) {
			//Restore window?
			if (maximized
			 && ((pt.y >= mon.top+AERO_THRESHOLD)
			  || (fmon.left < pt.x && pt.x < mon.left+2*AERO_THRESHOLD)
			  || (mon.right-2*AERO_THRESHOLD < pt.x && pt.x < fmon.right))) {
				//Restore window
				WINDOWPLACEMENT wndpl = { sizeof(WINDOWPLACEMENT) };
				GetWindowPlacement(state.hwnd, &wndpl);
				wndpl.showCmd = SW_RESTORE;
				SetWindowPlacement(state.hwnd, &wndpl);
				//Update wndwidth and wndheight
				wndwidth = wndpl.rcNormalPosition.right-wndpl.rcNormalPosition.left;
				wndheight = wndpl.rcNormalPosition.bottom-wndpl.rcNormalPosition.top;
			}
			
			//Move window
			if (pt.x < mon.left+2*AERO_THRESHOLD && pt.y < mon.top+2*AERO_THRESHOLD) {
				//Top left
				state.wndentry->restore = 1;
				wndwidth = (mon.right-mon.left)/2;
				wndheight = (mon.bottom-mon.top)/2;
				posx = mon.left;
				posy = mon.top;
			}
			else if (mon.right-2*AERO_THRESHOLD < pt.x && pt.y < mon.top+2*AERO_THRESHOLD) {
				//Top right
				state.wndentry->restore = 1;
				wndwidth = max(min((mon.right-mon.left)/2, state.mmi.ptMaxTrackSize.x), state.mmi.ptMinTrackSize.x);
				wndheight = (mon.bottom-mon.top)/2;
				posx = mon.right-wndwidth;
				posy = mon.top;
			}
			else if (pt.x < mon.left+2*AERO_THRESHOLD && mon.bottom-2*AERO_THRESHOLD < pt.y) {
				//Bottom left
				state.wndentry->restore = 1;
				wndwidth = (mon.right-mon.left)/2;
				wndheight = max(min((mon.bottom-mon.top)/2, state.mmi.ptMaxTrackSize.y), state.mmi.ptMinTrackSize.y);
				posx = mon.left;
				posy = mon.bottom-wndheight;
			}
			else if (mon.right-2*AERO_THRESHOLD < pt.x && mon.bottom-2*AERO_THRESHOLD < pt.y) {
				//Bottom right
				state.wndentry->restore = 1;
				wndwidth = max(min((mon.right-mon.left)/2, state.mmi.ptMaxTrackSize.x), state.mmi.ptMinTrackSize.x);
				wndheight = max(min((mon.bottom-mon.top)/2, state.mmi.ptMaxTrackSize.y), state.mmi.ptMinTrackSize.y);
				posx = mon.right-wndwidth;
				posy = mon.bottom-wndheight;
			}
			else if (pt.y < mon.top+AERO_THRESHOLD) {
				//Top
				if (!maximized) {
					state.wndentry->restore = 0;
					//Center window on monitor and maximize it
					WINDOWPLACEMENT wndpl = { sizeof(WINDOWPLACEMENT) };
					GetWindowPlacement(state.hwnd, &wndpl);
					wndpl.rcNormalPosition.left = fmon.left+(mon.right-mon.left)/2-state.origin.width/2;
					wndpl.rcNormalPosition.top = fmon.top+(mon.bottom-mon.top)/2-state.origin.height/2;
					wndpl.rcNormalPosition.right = wndpl.rcNormalPosition.left+state.origin.width;
					wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top+state.origin.height;
					wndpl.showCmd = SW_MAXIMIZE;
					SetWindowPlacement(state.hwnd, &wndpl);
				}
				return;
			}
			else if (pt.y < mon.top+2*AERO_THRESHOLD) {
				//Top
				state.wndentry->restore = 1;
				wndwidth = max(min((mon.right-mon.left), state.mmi.ptMaxTrackSize.x), state.mmi.ptMinTrackSize.x);
				wndheight = (mon.bottom-mon.top)/2;
				posx = (mon.right-mon.left)/2-wndwidth/2; //Center horizontally (if window has a max width)
				posy = mon.top;
			}
			else if (mon.bottom-AERO_THRESHOLD < pt.y) {
				//Bottom
				state.wndentry->restore = 1;
				wndwidth = max(min((mon.right-mon.left), state.mmi.ptMaxTrackSize.x), state.mmi.ptMinTrackSize.x);
				wndheight = max(min((mon.bottom-mon.top)/2, state.mmi.ptMaxTrackSize.y), state.mmi.ptMinTrackSize.y);
				posx = (mon.right-mon.left)/2-wndwidth/2; //Center horizontally (if window has a max width)
				posy = mon.bottom-wndheight;
			}
			else if (pt.x < mon.left+AERO_THRESHOLD) {
				//Left
				state.wndentry->restore = 1;
				wndwidth = (mon.right-mon.left)/2;
				wndheight = max(min((mon.bottom-mon.top), state.mmi.ptMaxTrackSize.y), state.mmi.ptMinTrackSize.y);
				posx = mon.left;
				posy = (mon.bottom-mon.top)/2-wndheight/2; //Center vertically (if window has a max height)
			}
			else if (mon.right-AERO_THRESHOLD < pt.x) {
				//Right
				state.wndentry->restore = 1;
				wndwidth = max(min((mon.right-mon.left)/2, state.mmi.ptMaxTrackSize.x), state.mmi.ptMinTrackSize.x);
				wndheight = max(min((mon.bottom-mon.top), state.mmi.ptMaxTrackSize.y), state.mmi.ptMinTrackSize.y);
				posx = mon.right-wndwidth;
				posy = (mon.bottom-mon.top)/2-wndheight/2; //Center vertically (if window has a max height)
			}
			else if (state.wndentry->restore) {
				//Restore original window size
				state.wndentry->restore = 0;
				wndwidth = state.origin.width;
				wndheight = state.origin.height;
			}
			
			//Aero-move the window?
			if (state.wndentry->restore) {
				state.wndentry->width = state.origin.width;
				state.wndentry->height = state.origin.height;
				
				//Move
				MoveWindow(state.hwnd, posx, posy, wndwidth, wndheight, TRUE);
				
				//Get new size after move
				//Doing this since wndwidth and wndheight might be wrong if the window is resized in chunks
				GetWindowRect(state.hwnd, &wnd);
				state.wndentry->last.width = wnd.right-wnd.left;
				state.wndentry->last.height = wnd.bottom-wnd.top;
				
				//We are done
				return;
			}
			
			/*
			FILE *f = fopen("C:\\altdrag-wnddb.txt", "wb");
			int k;
			for (k=0; k < NUMWNDDB; k++) {
				struct wnddata *wnd = &wnddb.items[k];
				fprintf(f, "%02d: hwnd:%10d, restore:%d, origin:%4dx%4d, last:%4dx%4d", k, wnd->hwnd, wnd->restore, wnd->width, wnd->height, wnd->last.width, wnd->last.height);
				if (wnd == wnddb.pos) {
					fprintf(f, " <--");
				}
				fprintf(f, "\n");
			}
			fclose(f);
			*/
		}
		
		//Check if the window will snap anywhere
		if (sharedstate.snap) {
			MoveSnap(&posx, &posy, wndwidth, wndheight);
		}
	}
	else if (sharedstate.action == ACTION_RESIZE) {
		//Clear restore flag
		state.wndentry->restore = 0;
		
		//Figure out new placement
		if (state.resize.x == RESIZE_CENTER && state.resize.y == RESIZE_CENTER) {
			posx = wnd.left-(pt.x-state.offset.x);
			posy = wnd.top-(pt.y-state.offset.y);
			wndwidth = wnd.right-wnd.left+2*(pt.x-state.offset.x);
			wndheight = wnd.bottom-wnd.top+2*(pt.y-state.offset.y);
			state.offset.x = pt.x;
			state.offset.y = pt.y;
		}
		else {
			if (state.resize.y == RESIZE_TOP) {
				wndheight = max(min((wnd.bottom-pt.y+state.offset.y), state.mmi.ptMaxTrackSize.y), state.mmi.ptMinTrackSize.y);
				posy = state.origin.bottom-wndheight;
			}
			else if (state.resize.y == RESIZE_CENTER) {
				posy = wnd.top;
				wndheight = wnd.bottom-wnd.top;
			}
			else if (state.resize.y == RESIZE_BOTTOM) {
				posy = wnd.top;
				wndheight = pt.y-wnd.top+state.offset.y;
			}
			if (state.resize.x == RESIZE_LEFT) {
				wndwidth = max(min((wnd.right-pt.x+state.offset.x), state.mmi.ptMaxTrackSize.x), state.mmi.ptMinTrackSize.x);
				posx = state.origin.right-wndwidth;
			}
			else if (state.resize.x == RESIZE_CENTER) {
				posx = wnd.left;
				wndwidth = wnd.right-wnd.left;
			}
			else if (state.resize.x == RESIZE_RIGHT) {
				posx = wnd.left;
				wndwidth = pt.x-wnd.left+state.offset.x;
			}
			
			//Check if the window will snap anywhere
			if (sharedstate.snap) {
				ResizeSnap(&posx, &posy, &wndwidth, &wndheight);
			}
		}
	}
	
	//Move
	MoveWindow(state.hwnd, posx, posy, wndwidth, wndheight, TRUE);
}

__declspec(dllexport) LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		int vkey = ((PKBDLLHOOKSTRUCT)lParam)->vkCode;
		
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
			if (IsHotkey(vkey)) {
				//Don't do anything if we're already pressing a hotkey
				if (state.alt) {
					return CallNextHookEx(NULL, nCode, wParam, lParam);
				}
				
				//Update state
				state.alt = 1;
				state.blockaltup = 0;
				state.blockmouseup = 0;
				state.interrupted = 0;
				
				//Ctrl as hotkey should not trigger Ctrl-focusing when starting dragging, releasing and pressing it again will focus though
				if (!sharedstate.action && (vkey == VK_LCONTROL || vkey == VK_RCONTROL)) {
					state.ignorectrl = 1;
				}
				
				//Hook mouse
				HookMouse();
			}
			else if (vkey == VK_LSHIFT || vkey == VK_RSHIFT) {
				sharedstate.shift = 1;
				sharedstate.snap = 3;
				MouseMove();
				
				//Block keydown to prevent Windows from changing keyboard layout
				if (state.alt && sharedstate.action) {
					return 1;
				}
			}
			else if (vkey == VK_SPACE && (sharedstate.action || msgaction) && sharedstate.snap) {
				sharedstate.snap = 0;
				return 1;
			}
			else if (vkey == VK_ESCAPE) {
				UnhookMouse();
			}
			else {
				state.interrupted = 1;
			}
			if (sharedstate.action && (vkey == VK_LCONTROL || vkey == VK_RCONTROL) && !state.ignorectrl && !state.ctrl) {
				POINT pt;
				GetCursorPos(&pt);
				state.locked = 0;
				state.origin.maximized = 0;
				state.origin.monitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
				SetForegroundWindow(state.hwnd);
				MouseMove();
				state.ctrl = 1;
			}
			
			if (sharedsettings.FocusOnTyping && !sharedstate.action && !state.alt) {
				POINT pt;
				GetCursorPos(&pt);
				HWND hoverwnd = WindowFromPoint(pt);
				if (hoverwnd == NULL) {
					return CallNextHookEx(NULL, nCode, wParam, lParam);
				}
				hoverwnd = GetAncestor(hoverwnd, GA_ROOT);
				HWND foregroundwnd = GetForegroundWindow();
				if (foregroundwnd != hoverwnd) {
					//Ignore if mouse is over taskbar
					HWND taskbar = FindWindow(L"Shell_TrayWnd", NULL);
					if (taskbar == hoverwnd) {
						return CallNextHookEx(NULL, nCode, wParam, lParam);
					}
					
					//Focus and raise?
					if (sharedsettings.FocusOnTyping == 1) {
						SetForegroundWindow(hoverwnd);
						return CallNextHookEx(NULL, nCode, wParam, lParam);
					}
					
					//Do not raise, this is a major hack
					SystemParametersInfo(SPI_SETACTIVEWINDOWTRACKING, 0, (PVOID)TRUE, 0);
					
					//Move mouse to bottom right corner
					POINT pt0 = {0, 0};
					HMONITOR monitor = MonitorFromPoint(pt0, MONITOR_DEFAULTTOPRIMARY);
					MONITORINFO mi = { sizeof(MONITORINFO) };
					GetMonitorInfo(monitor, &mi);
					MOUSEINPUT mm1 = {65535, 65535, 0, MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE, 0, GetMessageExtraInfo()};
					INPUT input1 = {INPUT_MOUSE, {.mi=mm1}};
					SendInput(1, &input1, sizeof(INPUT));
					
					//Sleep a while to let the mouse move happen
					Sleep(1);
					
					//Move mouse back
					int normalizedX = ceil(pt.x*65536.0/mi.rcMonitor.right);
					int normalizedY = ceil(pt.y*65536.0/mi.rcMonitor.bottom);
					MOUSEINPUT mm2 = {normalizedX, normalizedY, 0, MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE, 0, GetMessageExtraInfo()};
					INPUT input2 = {INPUT_MOUSE, {.mi=mm2}};
					SendInput(1, &input2, sizeof(INPUT));
					
					//Set timer to disable window tracking
					SetTimer(g_hwnd, FOCUS_TIMER, 10, NULL);
				}
			}
		}
		else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
			if (vkey == VK_LCONTROL || vkey == VK_RCONTROL) {
				if (sharedstate.action && !state.ignorectrl) {
					//Check this first or else we will miss it if Ctrl is a hotkey
					MouseMove();
				}
				state.ctrl = 0;
			}
			if (IsHotkey(vkey)) {
				//Double check that all the hotkeys have been released
				int i;
				for (i=0; i < sharedsettings.Hotkeys.length; i++) {
					if (vkey != sharedsettings.Hotkeys.keys[i] && GetAsyncKeyState(sharedsettings.Hotkeys.keys[i])&0x8000) {
						return CallNextHookEx(NULL, nCode, wParam, lParam);
					}
				}
				
				//Block the alt keyup to prevent the window menu to be selected.
				//The way this works is that the alt key is "disguised" by sending ctrl keydown/keyup events just before the altup (see issue 20).
				if (state.blockaltup || sharedstate.action) {
					state.ignorectrl = 1;
					KEYBDINPUT ctrl[2] = {{VK_CONTROL,0,0,0}, {VK_CONTROL,0,KEYEVENTF_KEYUP,0}};
					ctrl[0].dwExtraInfo = ctrl[1].dwExtraInfo = GetMessageExtraInfo();
					INPUT input[2] = {{INPUT_KEYBOARD,{.ki=ctrl[0]}}, {INPUT_KEYBOARD,{.ki=ctrl[1]}}};
					SendInput(2, input, sizeof(INPUT));
				}
				state.ignorectrl = 0;
				
				//Okay, all hotkeys have been released
				state.alt = 0;
				
				//Unhook mouse if not moving or resizing
				if (!sharedstate.action) {
					UnhookMouse();
				}
			}
			else if (vkey == VK_LSHIFT || vkey == VK_RSHIFT) {
				sharedstate.shift = 0;
				sharedstate.snap = sharedsettings.AutoSnap;
				MouseMove();
			}
		}
	}
	
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

__declspec(dllexport) LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		//Set up some variables
		PMSLLHOOKSTRUCT msg = (PMSLLHOOKSTRUCT)lParam;
		int button =
			(wParam==WM_LBUTTONDOWN||wParam==WM_LBUTTONUP)?BUTTON_LMB:
			(wParam==WM_MBUTTONDOWN||wParam==WM_MBUTTONUP)?BUTTON_MMB:
			(wParam==WM_RBUTTONDOWN||wParam==WM_RBUTTONUP)?BUTTON_RMB:
			(HIWORD(msg->mouseData)==XBUTTON1)?BUTTON_MB4:
			(HIWORD(msg->mouseData)==XBUTTON2)?BUTTON_MB5:BUTTON_NONE;
		enum {STATE_NONE, STATE_DOWN, STATE_UP} buttonstate =
			(wParam==WM_LBUTTONDOWN||wParam==WM_MBUTTONDOWN||wParam==WM_RBUTTONDOWN||wParam==WM_XBUTTONDOWN)?STATE_DOWN:
			(wParam==WM_LBUTTONUP||wParam==WM_MBUTTONUP||wParam==WM_RBUTTONUP||wParam==WM_XBUTTONUP)?STATE_UP:STATE_NONE;
		int action = GetAction(button);
		POINT pt = msg->pt;
		
		//Handle mouse move and scroll
		if (wParam == WM_MOUSEMOVE) {
			//Move the window
			if (sharedstate.action == ACTION_MOVE || sharedstate.action == ACTION_RESIZE) {
				state.updaterate = (state.updaterate+1)%(sharedstate.action==ACTION_MOVE?sharedsettings.Performance.MoveRate:sharedsettings.Performance.ResizeRate);
				if (state.updaterate == 0) {
					if (sharedsettings.Performance.Cursor) {
						MoveWindow(cursorwnd, pt.x-20, pt.y-20, 41, 41, TRUE);
						//MoveWindow(cursorwnd,(prevpt.x<pt.x?prevpt.x:pt.x)-3,(prevpt.y<pt.y?prevpt.y:pt.y)-3,(pt.x>prevpt.x?pt.x-prevpt.x:prevpt.x-pt.x)+7,(pt.y>prevpt.y?pt.y-prevpt.y:prevpt.y-pt.y)+7,FALSE);
					}
					MouseMove();
				}
			}
			//Reset double-click time
			//Unfortunately, we have to remember the previous pointer position since WM_MOUSEMOVE is sometimes sent even
			//if the mouse hasn't moved, e.g. when running Windows virtualized or when connecting to a remote desktop.
			if (pt.x != state.clickpt.x || pt.y != state.clickpt.y) {
				state.clicktime = 0;
			}
		}
		else if (wParam == WM_MOUSEWHEEL && !sharedstate.action && sharedsettings.Mouse.Scroll && !state.interrupted) {
			int delta = GET_WHEEL_DELTA_WPARAM(msg->mouseData);
			if (sharedsettings.Mouse.Scroll == ACTION_ALTTAB) {
				state.origin.monitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
				
				//Enumerate windows
				numhwnds = 0;
				EnumWindows(EnumAltTabWindows, 0);
				if (numhwnds < 2) {
					return CallNextHookEx(NULL, nCode, wParam, lParam);
				}
				
				//Use this to print the windows
				/*FILE *f = fopen("C:\\altdrag-log.txt", "wb");
				fprintf(f, "numhwnds: %d\n", numhwnds);
				char title[100], classname[100];
				int k;
				for (k=0; k < numhwnds; k++) {
					GetWindowTextA(hwnds[k], title, 100);
					GetClassNameA(hwnds[k], classname, 100);
					RECT wnd;
					GetWindowRect(hwnds[k], &wnd);
					fprintf(f, "wnd #%02d: %s [%s] (%dx%d @ %dx%d)\n", k, title, classname, wnd.right-wnd.left, wnd.bottom-wnd.top, wnd.left, wnd.top);
				}
				fclose(f);*/
				
				//Reorder windows
				if (delta > 0) {
					SetForegroundWindow(hwnds[numhwnds-1]);
				}
				else {
					SetWindowPos(hwnds[0], hwnds[numhwnds-1], 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);
					SetForegroundWindow(hwnds[1]);
				}
			}
			else if (sharedsettings.Mouse.Scroll == ACTION_VOLUME) {
				IMMDeviceEnumerator *pDevEnumerator = NULL;
				IMMDevice *pDev = NULL;
				IAudioEndpointVolume *pAudioEndpoint = NULL;
				
				//Get audio endpoint
				HRESULT hr = CoCreateInstance(&my_CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, &my_IID_IMMDeviceEnumerator, (void**)&pDevEnumerator);
				if (hr != S_OK) {
					Error(L"CoCreateInstance(MMDeviceEnumerator)", L"LowLevelMouseProc()", hr, TEXT(__FILE__), __LINE__);
					return CallNextHookEx(NULL, nCode, wParam, lParam);
				}
				hr = IMMDeviceEnumerator_GetDefaultAudioEndpoint(pDevEnumerator, eRender, eMultimedia, &pDev);
				IMMDeviceEnumerator_Release(pDevEnumerator);
				if (hr != S_OK) {
					Error(L"IMMDeviceEnumerator_GetDefaultAudioEndpoint(eRender, eMultimedia)", L"LowLevelMouseProc()", hr, TEXT(__FILE__), __LINE__);
					return CallNextHookEx(NULL, nCode, wParam, lParam);
				}
				hr = IMMDevice_Activate(pDev, &my_IID_IAudioEndpointVolume, CLSCTX_ALL, NULL, (void**)&pAudioEndpoint);
				IMMDevice_Release(pDev);
				if (hr != S_OK) {
					Error(L"IMMDevice_Activate(IID_IAudioEndpointVolume)", L"LowLevelMouseProc()", hr, TEXT(__FILE__), __LINE__);
					return CallNextHookEx(NULL, nCode, wParam, lParam);
				}
				
				//Function pointer so we only need one for-loop
				typedef HRESULT WINAPI (*_VolumeStep)(IAudioEndpointVolume*, LPCGUID pguidEventContext);
				_VolumeStep VolumeStep = (_VolumeStep)(pAudioEndpoint->lpVtbl->VolumeStepDown);
				if (delta > 0) {
					VolumeStep = (_VolumeStep)(pAudioEndpoint->lpVtbl->VolumeStepUp);
				}
				
				//Hold shift to make 5 steps
				int i;
				int num = (sharedstate.shift)?5:1;
				for (i=0; i < num; i++) {
					hr = VolumeStep(pAudioEndpoint, NULL);
				}
				IAudioEndpointVolume_Release(pAudioEndpoint);
				if (hr != S_OK) {
					Error(L"IAudioEndpointVolume_VolumeStepUp/Down()", L"LowLevelMouseProc()", hr, TEXT(__FILE__), __LINE__);
					return CallNextHookEx(NULL, nCode, wParam, lParam);
				}
			}
			
			//Block original scroll event
			state.blockaltup = 1;
			return 1;
		}
		
		//Return if this isn't a mouse action
		if (!action) {
			return CallNextHookEx(NULL, nCode, wParam, lParam);
		}
		
		//Get monitor info
		HMONITOR monitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
		MONITORINFO monitorinfo = { sizeof(MONITORINFO) };
		GetMonitorInfo(monitor, &monitorinfo);
		RECT mon = monitorinfo.rcWork;
		RECT fmon = monitorinfo.rcMonitor;
		
		//Toggle maximized state if move+resize is clicked
		if (buttonstate == STATE_DOWN && sharedstate.action == ACTION_MOVE && action == ACTION_RESIZE) {
			KillTimer(g_hwnd, RESTORE_TIMER);
			//Toggle maximized state
			WINDOWPLACEMENT wndpl = { sizeof(WINDOWPLACEMENT) };
			GetWindowPlacement(state.hwnd, &wndpl);
			wndpl.showCmd = (wndpl.showCmd==SW_MAXIMIZE)?SW_RESTORE:SW_MAXIMIZE;
			state.locked = (wndpl.showCmd==SW_MAXIMIZE);
			state.origin.maximized = (wndpl.showCmd==SW_MAXIMIZE);
			//If maximizing, also center window on monitor, if needed
			if (wndpl.showCmd == SW_MAXIMIZE) {
				HMONITOR wndmonitor = MonitorFromWindow(state.hwnd, MONITOR_DEFAULTTONEAREST);
				if (monitor != wndmonitor) {
					wndpl.rcNormalPosition.left = fmon.left+(mon.right-mon.left)/2-state.origin.width/2;
					wndpl.rcNormalPosition.top = fmon.top+(mon.bottom-mon.top)/2-state.origin.height/2;
					wndpl.rcNormalPosition.right = wndpl.rcNormalPosition.left+state.origin.width;
					wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top+state.origin.height;
				}
				state.origin.monitor = monitor;
			}
			else {
				//Otherwise, reset offset
				state.offset.x = (float)(pt.x-fmon.left)/(fmon.right-fmon.left)*state.origin.width;
				state.offset.y = (float)(pt.y-fmon.top)/(fmon.bottom-fmon.top)*state.origin.height;
			}
			SetWindowPlacement(state.hwnd, &wndpl);
			MouseMove();
			return 1;
		}
		
		//Block mousedown if we are busy with another action
		if (sharedstate.action && buttonstate == STATE_DOWN) {
			return 1; //Block mousedown so AltDrag.exe does not remove cursorwnd
		}
		
		if (state.alt && buttonstate == STATE_DOWN) {
			//Double check if any of the hotkeys are being pressed
			int i;
			for (i=0; i < sharedsettings.Hotkeys.length; i++) {
				if (GetAsyncKeyState(sharedsettings.Hotkeys.keys[i])&0x8000) {
					break;
				}
				else if (i+1 == sharedsettings.Hotkeys.length) {
					state.alt = 0;
					UnhookMouse();
					return CallNextHookEx(NULL, nCode, wParam, lParam);
				}
			}
			
			//Okay, at least one trigger key is being pressed
			HCURSOR cursor = NULL;
			
			//Make sure cursorwnd isn't in the way
			if (sharedsettings.Performance.Cursor) {
				ShowWindow(cursorwnd, SW_HIDE);
			}
			
			//Get window
			state.hwnd = WindowFromPoint(pt);
			if (state.hwnd == NULL) {
				return CallNextHookEx(NULL, nCode, wParam, lParam);
			}
			state.hwnd = GetAncestor(state.hwnd, GA_ROOT);
			
			//Use this to print info about the window which is about to be dragged
			/*{
				FILE *f = fopen("C:\\altdrag-log.txt", "wb");
				char title[100], classname[100];
				GetWindowTextA(state.hwnd, title, 100);
				GetClassNameA(state.hwnd, classname, 100);
				RECT wnd;
				GetWindowRect(state.hwnd, &wnd);
				fprintf(f, "hwnd     : %d\nTitle    : %s\nClassname: %s\nSize     : %dx%d\nPosition : %dx%d\nStyle    : 0x%08X\n", state.hwnd, title, classname, wnd.right-wnd.left, wnd.bottom-wnd.top, wnd.left, wnd.top, GetWindowLongPtr(state.hwnd,GWL_STYLE));
				fprintf(f, "Is fullscreen? %d == %d && %d == %d && %d == %d && %d == %d: %d\n", wnd.left, fmon.left, wnd.top, fmon.top, wnd.right, fmon.right, wnd.bottom, fmon.bottom, wnd.left==fmon.left && wnd.top==fmon.top && wnd.right==fmon.right && wnd.bottom==fmon.bottom);
				fclose(f);
			}*/
			
			//Return if window is blacklisted
			if (blacklisted(state.hwnd,&settings.ProcessBlacklist)
			 || blacklisted(state.hwnd,&settings.Blacklist)) {
				return CallNextHookEx(NULL, nCode, wParam, lParam);
			}
			
			//Get window placement
			WINDOWPLACEMENT wndpl = { sizeof(WINDOWPLACEMENT) };
			GetWindowPlacement(state.hwnd, &wndpl);
			RECT wnd;
			if (GetWindowRect(state.hwnd,&wnd) == 0) {
				return CallNextHookEx(NULL, nCode, wParam, lParam);
			}
			
			//Return if the window is a fullscreen window (and has no border)
			if (!(GetWindowLongPtr(state.hwnd,GWL_STYLE)&WS_CAPTION)
			 && wnd.left == fmon.left && wnd.top == fmon.top && wnd.right == fmon.right && wnd.bottom == fmon.bottom) {
				return CallNextHookEx(NULL, nCode, wParam, lParam);
			}
			
			//Update state
			sharedstate.action = action;
			if (!sharedstate.snap) {
				sharedstate.snap = sharedsettings.AutoSnap;
			}
			state.blockaltup = 1;
			state.locked = 0;
			state.origin.maximized = IsZoomed(state.hwnd);
			state.origin.width = wndpl.rcNormalPosition.right-wndpl.rcNormalPosition.left;
			state.origin.height = wndpl.rcNormalPosition.bottom-wndpl.rcNormalPosition.top;
			state.origin.monitor = MonitorFromWindow(state.hwnd, MONITOR_DEFAULTTONEAREST);
			
			//Check if window is in the wnddb database
			state.wndentry = NULL;
			for (i=0; i < NUMWNDDB; i++) {
				if (wnddb.items[i].hwnd == state.hwnd) {
					state.wndentry = &wnddb.items[i];
					break;
				}
			}
			
			//Find a nice place in wnddb if not already present
			if (state.wndentry == NULL) {
				int i;
				for (i=0; i < NUMWNDDB+1 && wnddb.pos->restore == 1; i++) {
					wnddb.pos = (wnddb.pos == &wnddb.items[NUMWNDDB-1])?&wnddb.items[0]:wnddb.pos+1;
				}
				state.wndentry = wnddb.pos;
				state.wndentry->hwnd = state.hwnd;
				state.wndentry->restore = 0;
			}
			
			//AutoFocus
			if (sharedsettings.AutoFocus) {
				SetForegroundWindow(state.hwnd);
			}
			
			//Get minmaxinfo
			if (action == ACTION_MOVE || action == ACTION_RESIZE) {
				MINMAXINFO mmi = {{160,28}, {GetSystemMetrics(SM_CXMAXIMIZED),GetSystemMetrics(SM_CYMAXIMIZED)}, {mon.left-8,mon.top-8}, {GetSystemMetrics(SM_CXMINTRACK),GetSystemMetrics(SM_CYMINTRACK)}, {GetSystemMetrics(SM_CXMAXTRACK),GetSystemMetrics(SM_CXMAXTRACK)}};
				SendMessage(state.hwnd, WM_GETMINMAXINFO, 0, (LPARAM)&mmi);
				state.mmi.ptMinTrackSize = mmi.ptMinTrackSize;
				state.mmi.ptMaxTrackSize = mmi.ptMaxTrackSize;
			}
			
			//Do things depending on what button was pressed
			if (action == ACTION_MOVE) {
				//Maximize window if this is a double-click
				if (GetTickCount()-state.clicktime <= GetDoubleClickTime()) {
					sharedstate.action = ACTION_NONE; //Stop move action
					state.clicktime = 0; //Reset double-click time
					state.blockmouseup = 1; //Block the mouseup, otherwise it can trigger a context menu (e.g. in explorer, or on the desktop)
					
					//Center window on monitor, if needed
					HMONITOR wndmonitor = MonitorFromWindow(state.hwnd, MONITOR_DEFAULTTONEAREST);
					if (monitor != wndmonitor) {
						wndpl.rcNormalPosition.left = fmon.left+(mon.right-mon.left)/2-state.origin.width/2;
						wndpl.rcNormalPosition.top = fmon.top+(mon.bottom-mon.top)/2-state.origin.height/2;
						wndpl.rcNormalPosition.right = wndpl.rcNormalPosition.left+state.origin.width;
						wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top+state.origin.height;
					}
					wndpl.showCmd = SW_MAXIMIZE;
					SetWindowPlacement(state.hwnd, &wndpl);
					
					//Prevent mousedown from propagating
					return 1;
				}
				
				//Restore old width/height?
				int restore = 0;
				if (state.wndentry->restore
				 && state.wndentry->last.width == state.origin.width
				 && state.wndentry->last.height == state.origin.height) {
					restore = 1;
					state.origin.width = state.wndentry->width;
					state.origin.height = state.wndentry->height;
				}
				state.wndentry->restore = 0;
				
				//Set offset
				if (state.origin.maximized) {
					state.offset.x = (float)(pt.x-wnd.left)/(wnd.right-wnd.left)*state.origin.width;
					state.offset.y = (float)(pt.y-wnd.top)/(wnd.bottom-wnd.top)*state.origin.height;
					
					//Restore the window
					wndpl.showCmd = SW_RESTORE;
					SetWindowPlacement(state.hwnd, &wndpl);
					
					//Set new position
					MouseMove();
				}
				else if (restore) {
					state.offset.x = (float)(pt.x-wnd.left)/(wnd.right-wnd.left)*state.origin.width;
					state.offset.y = (float)(pt.y-wnd.top)/(wnd.bottom-wnd.top)*state.origin.height;
					MoveWindow(state.hwnd, pt.x-state.offset.x, pt.y-state.offset.y, state.origin.width, state.origin.height, TRUE);
				}
				else {
					state.offset.x = pt.x-wnd.left;
					state.offset.y = pt.y-wnd.top;
				}
				
				cursor = cursors[HAND];
			}
			else if (action == ACTION_RESIZE) {
				//Restore the window (to monitor size) if it's maximized
				if (state.origin.maximized) {
					wndpl.rcNormalPosition = fmon; //Set size to full monitor to prevent flickering
					wndpl.showCmd = SW_RESTORE;
					SetWindowPlacement(state.hwnd, &wndpl);
					//Update origin width/height
					wnd = monitorinfo.rcWork;
					state.origin.width = wnd.right-wnd.left;
					state.origin.height = wnd.bottom-wnd.top;
					//Move window
					MoveWindow(state.hwnd, wnd.left, wnd.top, state.origin.width, state.origin.height, TRUE);
				}
				
				//Set edge and offset
				//Think of the window as nine boxes
				//Does not use state.origin.width/height since that is based on wndpl.rcNormalPosition which is not what you see when resizing a window that Windows Aero resized
				int wndwidth = wnd.right-wnd.left;
				int wndheight = wnd.bottom-wnd.top;
				if (pt.y-wnd.top < wndheight/3) {
					state.resize.y = RESIZE_TOP;
					state.offset.y = pt.y-wnd.top;
				}
				else if (pt.y-wnd.top < wndheight*2/3) {
					state.resize.y = RESIZE_CENTER;
					state.offset.y = pt.y; //Used only if both x and y are CENTER
				}
				else {
					state.resize.y = RESIZE_BOTTOM;
					state.offset.y = wnd.bottom-pt.y;
				}
				if (pt.x-wnd.left < wndwidth/3) {
					state.resize.x = RESIZE_LEFT;
					state.offset.x = pt.x-wnd.left;
				}
				else if (pt.x-wnd.left < wndwidth*2/3) {
					state.resize.x = RESIZE_CENTER;
					state.offset.x = pt.x; //Used only if both x and y are CENTER
				}
				else {
					state.resize.x = RESIZE_RIGHT;
					state.offset.x = wnd.right-pt.x;
				}
				
				//Set window right/bottom origin
				state.origin.right = wnd.right;
				state.origin.bottom = wnd.bottom;
				
				//Aero-move this window if this is a double-click
				if (GetTickCount()-state.clicktime <= GetDoubleClickTime()) {
					sharedstate.action = ACTION_NONE; //Stop resize action
					state.clicktime = 0; //Reset double-click time
					state.blockmouseup = 1; //Block the mouseup, otherwise it can trigger a context menu (e.g. in explorer, or on the desktop)
					
					//Get and set new position
					int posx, posy, wndwidth, wndheight;
					wndwidth = max(min((mon.right-mon.left)/2, state.mmi.ptMaxTrackSize.x), state.mmi.ptMinTrackSize.x);
					wndheight = max(min((mon.bottom-mon.top)/2, state.mmi.ptMaxTrackSize.y), state.mmi.ptMinTrackSize.y);
					posx = mon.left;
					posy = mon.top;
					if (state.resize.y == RESIZE_CENTER) {
						wndheight = max(min((mon.bottom-mon.top), state.mmi.ptMaxTrackSize.y), state.mmi.ptMinTrackSize.y);
						posy = (mon.bottom-mon.top)/2-wndheight/2;
					}
					else if (state.resize.y == RESIZE_BOTTOM) {
						posy = mon.bottom-wndheight;
					}
					if (state.resize.x == RESIZE_CENTER) {
						posx = wnd.left;
						wndwidth = wnd.right-wnd.left;
						if (state.resize.y != RESIZE_CENTER) {
							wndwidth = max(min((mon.right-mon.left), state.mmi.ptMaxTrackSize.x), state.mmi.ptMinTrackSize.x);
							posx = (mon.right-mon.left)/2-wndwidth/2;
						}
					}
					else if (state.resize.x == RESIZE_RIGHT) {
						posx = mon.right-wndwidth;
					}
					MoveWindow(state.hwnd, posx, posy, wndwidth, wndheight, TRUE);
					
					//Get new size after move
					//Doing this since wndwidth and wndheight might be wrong if the window is resized in chunks
					GetWindowRect(state.hwnd, &wnd);
					//Update wndentry
					state.wndentry->last.width = wnd.right-wnd.left;
					state.wndentry->last.height = wnd.bottom-wnd.top;
					if (!state.wndentry->restore) {
						state.wndentry->width = state.origin.width;
						state.wndentry->height = state.origin.height;
						state.wndentry->restore = 1;
					}
					
					//Prevent mousedown from propagating
					return 1;
				}
				
				//Set cursor
				if (sharedsettings.Performance.Cursor) {
					if ((state.resize.y == RESIZE_TOP && state.resize.x == RESIZE_LEFT)
					 || (state.resize.y == RESIZE_BOTTOM && state.resize.x == RESIZE_RIGHT)) {
						cursor = cursors[SIZENWSE];
					}
					else if ((state.resize.y == RESIZE_TOP && state.resize.x == RESIZE_RIGHT)
					 || (state.resize.y == RESIZE_BOTTOM && state.resize.x == RESIZE_LEFT)) {
						cursor = cursors[SIZENESW];
					}
					else if ((state.resize.y == RESIZE_TOP && state.resize.x == RESIZE_CENTER)
					 || (state.resize.y == RESIZE_BOTTOM && state.resize.x == RESIZE_CENTER)) {
						cursor = cursors[SIZENS];
					}
					else if ((state.resize.y == RESIZE_CENTER && state.resize.x == RESIZE_LEFT)
					 || (state.resize.y == RESIZE_CENTER && state.resize.x == RESIZE_RIGHT)) {
						cursor = cursors[SIZEWE];
					}
					else {
						cursor = cursors[SIZEALL];
					}
				}
			}
			else if (action == ACTION_MINIMIZE) {
				SendMessage(state.hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
			}
			else if (action == ACTION_CENTER) {
				MoveWindow(state.hwnd, mon.left+(mon.right-mon.left)/2-state.origin.width/2, mon.top+(mon.bottom-mon.top)/2-state.origin.height/2, state.origin.width, state.origin.height, TRUE);
			}
			else if (action == ACTION_ALWAYSONTOP) {
				int topmost = GetWindowLongPtr(state.hwnd,GWL_EXSTYLE)&WS_EX_TOPMOST;
				SetWindowPos(state.hwnd, (topmost?HWND_NOTOPMOST:HWND_TOPMOST), 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
			}
			else if (action == ACTION_CLOSE) {
				SendMessage(state.hwnd, WM_CLOSE, 0, 0);
			}
			else if (action == ACTION_LOWER) {
				SetWindowPos(state.hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);
			}
			
			//Send WM_ENTERSIZEMOVE and prepare update timer
			if (action == ACTION_MOVE || action == ACTION_RESIZE) {
				//Don't send WM_ENTERSIZEMOVE if the window is iTunes
				wchar_t classname[30] = L"";
				GetClassName(state.hwnd, classname, sizeof(classname)/sizeof(wchar_t));
				if (wcscmp(classname,L"iTunes")) {
					SendMessage(state.hwnd, WM_ENTERSIZEMOVE, 0, 0);
				}
				//Prepare update timer
				state.updaterate = 0;
				if ((action == ACTION_MOVE   && sharedsettings.Performance.MoveRate > 1)
				 || (action == ACTION_RESIZE && sharedsettings.Performance.ResizeRate > 1)) {
					SetTimer(g_hwnd, MOVE_TIMER, 100, NULL);
				}
			}
			
			//Remember time of this click so we can check for double-click
			state.clicktime = GetTickCount();
			state.clickpt = pt;
			
			//Update cursor
			if (sharedsettings.Performance.Cursor && cursor != NULL) {
				MoveWindow(cursorwnd, pt.x-20, pt.y-20, 41, 41, FALSE);
				SetClassLongPtr(cursorwnd, GCLP_HCURSOR, (LONG_PTR)cursor);
				ShowWindowAsync(cursorwnd, SW_SHOWNA);
			}
			
			//Prevent mousedown from propagating
			return 1;
		}
		else if (buttonstate == STATE_UP && state.blockmouseup) {
			state.blockmouseup = 0;
			return 1;
		}
		else if (buttonstate == STATE_UP && sharedstate.action == action) {
			sharedstate.action = ACTION_NONE;
			
			//Send WM_EXITSIZEMOVE
			if (action == ACTION_MOVE || action == ACTION_RESIZE) {
				//Don't send WM_EXITSIZEMOVE if the window is iTunes
				wchar_t classname[30] = L"";
				GetClassName(state.hwnd, classname, sizeof(classname)/sizeof(wchar_t));
				if (wcscmp(classname,L"iTunes")) {
					SendMessage(state.hwnd, WM_EXITSIZEMOVE, 0, 0);
				}
			}
			
			//Unhook mouse?
			if (!state.alt) {
				UnhookMouse();
			}
			
			//Hide cursorwnd
			if (sharedsettings.Performance.Cursor) {
				ShowWindowAsync(cursorwnd, SW_HIDE);
			}
			
			//Prevent mouseup from propagating
			return 1;
		}
	}
	
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

__declspec(dllexport) LRESULT CALLBACK ScrollHook(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		PMSLLHOOKSTRUCT msg = (PMSLLHOOKSTRUCT)lParam;
		POINT pt = msg->pt;
		
		if ((wParam == WM_MOUSEWHEEL || (wParam == WM_MOUSEHWHEEL && sharedsettings.InactiveScroll != 2)) && !state.alt) {
			//Get window and foreground window
			HWND window = WindowFromPoint(pt);
			HWND foreground = GetForegroundWindow();
			int style = GetWindowLongPtr(window, GWL_STYLE);
			
			//Return if no window, if same window (only if child has no scrollbars), or if foreground window is blacklisted
			if (window == NULL
			 || (!(style&WS_VSCROLL || style&WS_HSCROLL) && GetAncestor(window,GA_ROOT) == foreground)
			 || (foreground != NULL && blacklisted(foreground,&settings.Blacklist))) {
				return CallNextHookEx(NULL, nCode, wParam, lParam);
			}
			
			//Get wheel info
			WPARAM wp = GET_WHEEL_DELTA_WPARAM(msg->mouseData) << 16;
			LPARAM lp = (pt.y << 16) | (pt.x & 0xFFFF);
			//Add button information since we don't get it with the hook
			if (GetAsyncKeyState(VK_CONTROL)&0x8000)  wp |= MK_CONTROL;
			if (GetAsyncKeyState(VK_LBUTTON)&0x8000)  wp |= MK_LBUTTON;
			if (GetAsyncKeyState(VK_MBUTTON)&0x8000)  wp |= MK_MBUTTON;
			if (GetAsyncKeyState(VK_RBUTTON)&0x8000)  wp |= MK_RBUTTON;
			if (GetAsyncKeyState(VK_SHIFT)&0x8000)    wp |= MK_SHIFT;
			if (GetAsyncKeyState(VK_XBUTTON1)&0x8000) wp |= MK_XBUTTON1;
			if (GetAsyncKeyState(VK_XBUTTON2)&0x8000) wp |= MK_XBUTTON2;
			
			//Change WM_MOUSEWHEEL to WM_MOUSEHWHEEL if shift is being depressed
			//Note that this does not work on all windows, the message was introduced in Vista and far from all programs have implemented it
			if (wParam == WM_MOUSEWHEEL
			 && sharedsettings.InactiveScroll != 2
			 && sharedstate.shift
			 && GetAsyncKeyState(VK_SHIFT)&0x8000) {
				wParam = WM_MOUSEHWHEEL;
				wp = -wp; //Up is left, down is right
			}
			
			//Forward scroll message
			SendMessage(window, wParam, wp, lp);
			
			//Block original scroll event
			return 1;
		}
		else if (wParam == WM_MOUSEMOVE) {
			//Store prevpt so we can check if the hook goes stale
			state.prevpt = pt;
		}
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int HookMouse() {
	if (mousehook) {
		//Mouse already hooked
		return 1;
	}
	
	//Check if scrollhook has become stale
	if (sharedsettings.InactiveScroll && sharedsettings.InactiveScroll != 3) {
		SendMessage(g_hwnd, WM_TIMER, REHOOK_TIMER, 0);
	}
	
	//Set up the mouse hook
	mousehook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, hinstDLL, 0);
	if (mousehook == NULL) {
		Error(L"SetWindowsHookEx(WH_MOUSE_LL)", L"HookMouse()", GetLastError(), TEXT(__FILE__), __LINE__);
		return 1;
	}
	
	//Success
	return 0;
}

int UnhookMouse() {
	if (!mousehook) {
		//Mouse not hooked
		return 1;
	}
	
	//Stop action
	sharedstate.action = ACTION_NONE;
	if (sharedsettings.Performance.Cursor) {
		ShowWindowAsync(cursorwnd, SW_HIDE);
	}
	
	//Remove mouse hook
	if (UnhookWindowsHookEx(mousehook) == 0) {
		Error(L"UnhookWindowsHookEx(mousehook)", L"", GetLastError(), TEXT(__FILE__), __LINE__);
		mousehook = NULL;
		return 1;
	}
	
	//Success
	mousehook = NULL;
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_TIMER) {
		if (wParam == RESTORE_TIMER) {
			KillTimer(g_hwnd, wParam);
			state.locked = 0;
			
			if (sharedstate.action == ACTION_MOVE) {
				//Reset offset
				POINT pt;
				GetCursorPos(&pt);
				HMONITOR monitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
				MONITORINFO monitorinfo = { sizeof(MONITORINFO) };
				GetMonitorInfo(monitor, &monitorinfo);
				RECT fmon = monitorinfo.rcMonitor;
				state.offset.x = (float)(pt.x-fmon.left)/(fmon.right-fmon.left)*state.origin.width;
				state.offset.y = (float)(pt.y-fmon.top)/(fmon.bottom-fmon.top)*state.origin.height;
				
				//Restore window
				WINDOWPLACEMENT wndpl = { sizeof(WINDOWPLACEMENT) };
				GetWindowPlacement(state.hwnd, &wndpl);
				wndpl.showCmd = SW_RESTORE;
				SetWindowPlacement(state.hwnd, &wndpl);
				
				//Move
				MouseMove();
			}
		}
		else if (wParam == MOVE_TIMER) {
			if (!sharedstate.action) {
				KillTimer(g_hwnd, wParam);
			}
			MouseMove();
		}
		else if (wParam == REHOOK_TIMER) {
			//Silently rehook scroll hook if it becomes stale (Windows 7 is very aggressive about hooks)
			//This can often happen when locking the screen or sleeping the computer a lot
			//HACK: Set InactiveScroll=3 to disable this behavior
			POINT pt;
			GetCursorPos(&pt);
			if (pt.x != state.prevpt.x || pt.y != state.prevpt.y) {
				UnhookWindowsHookEx(scrollhook);
				scrollhook = SetWindowsHookEx(WH_MOUSE_LL, ScrollHook, hinstDLL, 0);
			}
		}
		else if (wParam == FOCUS_TIMER) {
			KillTimer(g_hwnd, wParam);
			SystemParametersInfo(SPI_SETACTIVEWINDOWTRACKING, 0, (PVOID)FALSE, 0);
		}
		else if (wParam == INIT_TIMER) {
			KillTimer(g_hwnd, wParam);
			//Needed for IAudioEndpointVolume, and maybe some future stuff
			HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
			if (hr != S_OK && hr != S_FALSE) {
				Error(L"CoInitializeEx()", L"LowLevelMouseProc()", hr, TEXT(__FILE__), __LINE__);
			}
			
			/*
			BOOL wndtracking, snaptobutton;
			DWORD trktimeout, trkzorder;
			SystemParametersInfo(SPI_GETACTIVEWINDOWTRACKING, 0, &wndtracking, 0);
			SystemParametersInfo(SPI_GETACTIVEWNDTRKTIMEOUT, 0, &trktimeout, 0);
			SystemParametersInfo(SPI_GETACTIVEWNDTRKZORDER, 0, &trkzorder, 0);
			SystemParametersInfo(SPI_GETSNAPTODEFBUTTON, 0, &snaptobutton, 0);
			DBG("SPI_GETACTIVEWINDOWTRACKING: %d\nSPI_GETACTIVEWNDTRKTIMEOUT: %d\nSPI_GETACTIVEWNDTRKZORDER: %d\nSPI_GETSNAPTODEFBUTTON: %d", wndtracking, trktimeout, trkzorder, snaptobutton);
			*/
		}
	}
	else if (msg == WM_DESTROY) {
		KillTimer(g_hwnd, RESTORE_TIMER);
		KillTimer(g_hwnd, MOVE_TIMER);
		KillTimer(g_hwnd, REHOOK_TIMER);
		if (sharedsettings.FocusOnTyping > 1) {
			SystemParametersInfo(SPI_SETACTIVEWINDOWTRACKING, 0, (PVOID)FALSE, 0);
		}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

#endif

//Msghook
__declspec(dllexport) LRESULT CALLBACK CustomWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
	if (msg == WM_WINDOWPOSCHANGING && sharedstate.snap) {
		WINDOWPOS *wndpos = (WINDOWPOS*)lParam;
		if (msgaction == ACTION_MOVE && !(wndpos->flags&SWP_NOMOVE)) {
			MoveSnap(&wndpos->x, &wndpos->y, wndpos->cx, wndpos->cy);
		}
		else if (msgaction == ACTION_RESIZE && !(wndpos->flags&SWP_NOSIZE)) {
			ResizeSnap(&wndpos->x, &wndpos->y, &wndpos->cx, &wndpos->cy);
		}
	}
	else if (msg == WM_EXITSIZEMOVE || msg == WM_DESTROY) {
		subclassed = !RemoveWindowSubclass(hwnd, CustomWndProc, 0);
		if (subclassed) {
			Error(L"RemoveWindowSubclass(hwnd, CustomWndProc, 0)", L"Failed to remove window subclassing.", -1, TEXT(__FILE__), __LINE__);
			return DefSubclassProc(hwnd, msg, wParam, lParam);
		}
		hwnd = NULL;
		msgaction = ACTION_NONE;
	}
	
	/*
	//Fun code to trap window on screen
	if (msg == WM_WINDOWPOSCHANGING) {
		WINDOWPOS *wndpos = (WINDOWPOS*)lParam;
		if (wndpos->x < 0) {
			wndpos->x = 0;
		}
		if (wndpos->y < 0) {
			wndpos->y = 0;
		}
		if (wndpos->x+wndpos->cx > 1920) {
			wndpos->x = 1920-wndpos->cx;
		}
		if (wndpos->y+wndpos->cy > 1140) {
			wndpos->y = 1140-wndpos->cy;
		}
	}
	*/
	
	return DefSubclassProc(hwnd, msg, wParam, lParam);
}

//CallWndProc is called in the context of the thread that calls SendMessage, not the thread that receives the message.
//Thus we have to explicitly share the memory we want CallWndProc to be able to access (e.g. sharedstate)
//Variables that are not shared, e.g. the blacklist, are loaded individually for each process.
__declspec(dllexport) LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION && !sharedstate.action) {
		CWPSTRUCT *msg = (CWPSTRUCT*)lParam;
		
		if (msg->message == WM_ENTERSIZEMOVE
		 && (!subclassed || state.hwnd != msg->hwnd)
		 && IsWindowVisible(msg->hwnd)
		 && GetWindowLongPtr(msg->hwnd,GWL_STYLE)&WS_CAPTION // || !(GetWindowLongPtr(msg->hwnd,GWL_EXSTYLE)&WS_EX_TOOLWINDOW))
		 && !IsIconic(msg->hwnd) && !IsZoomed(msg->hwnd)
		 && msg->hwnd == GetAncestor(msg->hwnd,GA_ROOT)
		) {
			//Return if window is blacklisted
			if (blacklisted(msg->hwnd,&settings.Blacklist)) {
				return CallNextHookEx(NULL, nCode, wParam, lParam);
			}
			
			//Remove old subclassing if another window is currently subclassed
			if (subclassed && IsWindow(state.hwnd)) {
				subclassed = !RemoveWindowSubclass(state.hwnd, CustomWndProc, 0);
				if (subclassed) {
					Error(L"RemoveWindowSubclass(hwnd, CustomWndProc, 0)", L"Failed to remove window subclassing.", -1, TEXT(__FILE__), __LINE__);
				}
			}
			
			//Set hwnd
			state.hwnd = msg->hwnd;
			
			//Double check if a shift key is still being pressed
			if (sharedstate.shift && !(GetAsyncKeyState(VK_SHIFT)&0x8000)) {
				sharedstate.shift = 0;
				sharedstate.snap = sharedsettings.AutoSnap;
			}
			if (!sharedstate.snap) {
				sharedstate.snap = sharedsettings.AutoSnap;
				if (!sharedstate.snap) {
					return CallNextHookEx(NULL, nCode, wParam, lParam);
				}
			}
			
			//Subclass window
			subclassed = SetWindowSubclass(state.hwnd, CustomWndProc, 0, 0);
			if (!subclassed) {
				Error(L"SetWindowSubclass(state.hwnd, CustomWndProc, 0, 0)", L"Failed to subclass window.", -1, TEXT(__FILE__), __LINE__);
			}
		}
		else if (msg->message == WM_WINDOWPOSCHANGING
		 && !subclassed && state.hwnd == msg->hwnd && msgaction != ACTION_NONE
		 && sharedstate.snap) {
			
			//Subclass window
			subclassed = SetWindowSubclass(state.hwnd, CustomWndProc, 0, 0);
			if (!subclassed) {
				Error(L"SetWindowSubclass(state.hwnd, CustomWndProc, 0, 0)", L"Failed to subclass window.", -1, TEXT(__FILE__), __LINE__);
			}
		}
		else if (msg->message == WM_SYSCOMMAND) {
			WPARAM action = msg->wParam&0xFFF0;
			if (action == SC_MOVE) {
				msgaction = ACTION_MOVE;
			}
			else if (action == SC_SIZE) {
				msgaction = ACTION_RESIZE;
				int edge = msg->wParam&0x000F; //These are the undocumented bits (compatible with WMSZ_*)
				//resize.x
				if (edge == WMSZ_TOP || edge == WMSZ_BOTTOM) {
					state.resize.x = RESIZE_CENTER;
				}
				if (edge == WMSZ_LEFT || edge == WMSZ_TOPLEFT || edge == WMSZ_BOTTOMLEFT) {
					state.resize.x = RESIZE_LEFT;
				}
				else if (edge == WMSZ_RIGHT || edge == WMSZ_TOPRIGHT || edge == WMSZ_BOTTOMRIGHT) {
					state.resize.x = RESIZE_RIGHT;
				}
				//resize.y
				if (edge == WMSZ_LEFT || edge == WMSZ_RIGHT) {
					state.resize.y = RESIZE_CENTER;
				}
				if (edge == WMSZ_TOP || edge == WMSZ_TOPLEFT || edge == WMSZ_TOPRIGHT) {
					state.resize.y = RESIZE_TOP;
				}
				else if (edge == WMSZ_BOTTOM || edge == WMSZ_BOTTOMLEFT || edge == WMSZ_BOTTOMRIGHT) {
					state.resize.y = RESIZE_BOTTOM;
				}
				//Set offset
				state.offset.x = 0;
				state.offset.y = 0;
			}
		}
		else if (msg->message == WM_EXITSIZEMOVE) {
			msgaction = ACTION_NONE;
		}
	}
	
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

__declspec(dllexport) void Unload() {
	sharedsettings_loaded = 0;
	#ifndef _WIN64
	DestroyWindow(g_hwnd);
	if (scrollhook) {
		if (UnhookWindowsHookEx(scrollhook) == 0) {
			Error(L"UnhookWindowsHookEx(scrollhook)", L"Could not unhook mouse. Try restarting "APP_NAME".", GetLastError(), TEXT(__FILE__), __LINE__);
		}
		scrollhook = NULL;
	}
	#endif
}

BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		hinstDLL = hInst;
		//Load settings
		wchar_t txt[1000];
		//Settings shared with CallWndProc hooks
		if (!sharedsettings_loaded) {
			sharedsettings_loaded = 1;
			sharedsettings.Hotkeys.length = 0;
			
			//Store path to ini file at initial load so CallWndProc hooks can find it
			GetModuleFileName(NULL, inipath, sizeof(inipath)/sizeof(wchar_t));
			PathRemoveFileSpec(inipath);
			wcscat(inipath, L"\\"APP_NAME".ini");
			
			//[General]
			GetPrivateProfileString(L"General", L"AutoFocus", L"0", txt, sizeof(txt)/sizeof(wchar_t), inipath);
			sharedsettings.AutoFocus = _wtoi(txt);
			GetPrivateProfileString(L"General", L"AutoSnap", L"0", txt, sizeof(txt)/sizeof(wchar_t), inipath);
			sharedsettings.AutoSnap = sharedstate.snap = _wtoi(txt);
			GetPrivateProfileString(L"General", L"Aero", L"2", txt, sizeof(txt)/sizeof(wchar_t), inipath);
			sharedsettings.Aero = _wtoi(txt);
			GetPrivateProfileString(L"General", L"InactiveScroll", L"0", txt, sizeof(txt)/sizeof(wchar_t), inipath);
			sharedsettings.InactiveScroll = _wtoi(txt);
			
			//[Advanced]
			GetPrivateProfileString(L"Advanced", L"AutoRemaximize", L"0", txt, sizeof(txt)/sizeof(wchar_t), inipath);
			sharedsettings.AutoRemaximize = _wtoi(txt);
			GetPrivateProfileString(L"Advanced", L"SnapThreshold", L"20", txt, sizeof(txt)/sizeof(wchar_t), inipath);
			sharedsettings.SnapThreshold = _wtoi(txt);
			GetPrivateProfileString(L"Advanced", L"FocusOnTyping", L"0", txt, sizeof(txt)/sizeof(wchar_t), inipath);
			sharedsettings.FocusOnTyping = _wtoi(txt);
			
			//Detect if Aero Snap is enabled
			if (sharedsettings.Aero == 2) {
				/*
				BOOL enabled;
				SystemParametersInfo(SPI_GETWINARRANGING, 0, &enabled, 0);
				DBG("SPI_GETWINARRANGING: %d", enabled);
				SystemParametersInfo(SPI_GETDOCKMOVING, 0, &enabled, 0);
				DBG("SPI_GETDOCKMOVING: %d", enabled);
				sharedsettings.Aero = !!enabled;
				*/
				HKEY key;
				DWORD len = sizeof(txt);
				RegOpenKeyEx(HKEY_CURRENT_USER, L"Control Panel\\Desktop", 0, KEY_QUERY_VALUE, &key);
				int error = RegQueryValueEx(key, L"WindowArrangementActive", NULL, NULL, (LPBYTE)txt, &len);
				RegCloseKey(key);
				if (error == ERROR_SUCCESS) {
					sharedsettings.Aero = _wtoi(txt);
				}
			}
			
			//[Performance]
			GetPrivateProfileString(L"Performance", L"Cursor", L"1", txt, sizeof(txt)/sizeof(wchar_t), inipath);
			sharedsettings.Performance.Cursor = _wtoi(txt);
			if (sharedsettings.Performance.Cursor) {
				cursorwnd = FindWindow(APP_NAME, NULL);
				#ifndef _WIN64
				cursors[HAND]     = LoadImage(NULL, IDC_HAND,     IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
				cursors[SIZENWSE] = LoadImage(NULL, IDC_SIZENWSE, IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
				cursors[SIZENESW] = LoadImage(NULL, IDC_SIZENESW, IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
				cursors[SIZENS]   = LoadImage(NULL, IDC_SIZENS,   IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
				cursors[SIZEWE]   = LoadImage(NULL, IDC_SIZEWE,   IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
				cursors[SIZEALL]  = LoadImage(NULL, IDC_SIZEALL,  IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
				#endif
			}
			GetPrivateProfileString(L"Performance", L"MoveRate", L"1", txt, sizeof(txt)/sizeof(wchar_t), inipath);
			sharedsettings.Performance.MoveRate = _wtoi(txt);
			if (sharedsettings.Performance.MoveRate < 1) sharedsettings.Performance.MoveRate = 1;
			GetPrivateProfileString(L"Performance", L"ResizeRate", L"1", txt, sizeof(txt)/sizeof(wchar_t), inipath);
			sharedsettings.Performance.ResizeRate = _wtoi(txt);
			if (sharedsettings.Performance.ResizeRate < 1) sharedsettings.Performance.ResizeRate = 1;
			
			//[Input]
			struct {
				wchar_t *key;
				wchar_t *def;
				enum action *ptr;
			} buttons[] = {
				{L"LMB",    L"Move",    &sharedsettings.Mouse.LMB},
				{L"MMB",    L"Resize",  &sharedsettings.Mouse.MMB},
				{L"RMB",    L"Resize",  &sharedsettings.Mouse.RMB},
				{L"MB4",    L"Nothing", &sharedsettings.Mouse.MB4},
				{L"MB5",    L"Nothing", &sharedsettings.Mouse.MB5},
				{L"Scroll", L"Nothing", &sharedsettings.Mouse.Scroll},
				{NULL}
			};
			int i;
			for (i=0; buttons[i].key != NULL; i++) {
				GetPrivateProfileString(L"Input", buttons[i].key, buttons[i].def, txt, sizeof(txt)/sizeof(wchar_t), inipath);
				if      (!wcsicmp(txt,L"Move"))        *buttons[i].ptr = ACTION_MOVE;
				else if (!wcsicmp(txt,L"Resize"))      *buttons[i].ptr = ACTION_RESIZE;
				else if (!wcsicmp(txt,L"Minimize"))    *buttons[i].ptr = ACTION_MINIMIZE;
				else if (!wcsicmp(txt,L"Center"))      *buttons[i].ptr = ACTION_CENTER;
				else if (!wcsicmp(txt,L"AlwaysOnTop")) *buttons[i].ptr = ACTION_ALWAYSONTOP;
				else if (!wcsicmp(txt,L"Close"))       *buttons[i].ptr = ACTION_CLOSE;
				else if (!wcsicmp(txt,L"Lower"))       *buttons[i].ptr = ACTION_LOWER;
				else if (!wcsicmp(txt,L"AltTab"))      *buttons[i].ptr = ACTION_ALTTAB;
				else if (!wcsicmp(txt,L"Volume"))      *buttons[i].ptr = ACTION_VOLUME;
				else                                   *buttons[i].ptr = ACTION_NONE;
			}
			
			unsigned int temp;
			int numread;
			GetPrivateProfileString(L"Input", L"Hotkeys", L"A4 A5", txt, sizeof(txt)/sizeof(wchar_t), inipath);
			wchar_t *pos = txt;
			while (*pos != '\0' && swscanf(pos,L"%02X%n",&temp,&numread) != EOF) {
				//Bail if we are out of space
				if (sharedsettings.Hotkeys.length == MAXKEYS) {
					break;
				}
				//Store key
				sharedsettings.Hotkeys.keys[sharedsettings.Hotkeys.length++] = temp;
				pos += numread;
			}
			
			//Zero-out wnddb hwnds
			for (i=0; i < NUMWNDDB; i++) {
				wnddb.items[i].hwnd = NULL;
			}
			wnddb.pos = &wnddb.items[0];
			
			//Create window for timers
			#ifndef _WIN64
			WNDCLASSEX wnd = {sizeof(WNDCLASSEX), 0, WindowProc, 0, 0, hInst, NULL, NULL, NULL, NULL, APP_NAME"-hooks", NULL};
			RegisterClassEx(&wnd);
			g_hwnd = CreateWindowEx(0, wnd.lpszClassName, wnd.lpszClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, HWND_MESSAGE, NULL, hInst, NULL);
			
			//InactiveScroll
			if (sharedsettings.InactiveScroll) {
				scrollhook = SetWindowsHookEx(WH_MOUSE_LL, ScrollHook, hinstDLL, 0);
				if (scrollhook == NULL) {
					Error(L"SetWindowsHookEx(WH_MOUSE_LL)", L"Could not hook mouse. Another program might be interfering.", GetLastError(), TEXT(__FILE__), __LINE__);
				}
				if (sharedsettings.InactiveScroll != 3) {
					SetTimer(g_hwnd, REHOOK_TIMER, 5000, NULL);
				}
			}
			#endif
		}
		
		//[Blacklist]
		int blacklist_alloc = 0;
		struct blacklist *blacklist = &settings.ProcessBlacklist;
		//Process ProcessBlacklist first
		GetPrivateProfileString(L"Blacklist", L"ProcessBlacklist", L"", txt, sizeof(txt)/sizeof(wchar_t), inipath);
		blacklist->data = malloc((wcslen(txt)+1)*sizeof(wchar_t));
		wcscpy(blacklist->data, txt);
		wchar_t *pos = blacklist->data;
		while (pos != NULL) {
			wchar_t *title = pos;
			wchar_t *classname = wcsstr(pos, L"|");
			//Move pos to next item (if any)
			pos = wcsstr(pos, L",");
			if (pos != NULL) {
				*pos = '\0';
				pos++;
			}
			//Split the item with NULLs
			if (blacklist == &settings.ProcessBlacklist) {
				//ProcessBlacklist does not use classname or wildcards
				classname = NULL;
			}
			else {
				if (classname != NULL) {
					*classname = '\0';
					classname++;
				}
				//Check if title or classname is wildcard
				if (!wcscmp(title,L"*")) {
					title = NULL;
				}
				if (classname != NULL && !wcscmp(classname,L"*")) {
					classname = NULL;
				}
			}
			//Check for invalid entry
			if (classname == NULL && title != NULL && title[0] == '\0') {
				title = NULL;
			}
			//Do not store item if it's empty
			if (title != NULL || classname != NULL) {
				//Make sure we have enough space
				if (blacklist->length == blacklist_alloc) {
					blacklist_alloc += 15;
					blacklist->items = realloc(blacklist->items, blacklist_alloc*sizeof(struct blacklistitem));
				}
				//Store item
				blacklist->items[blacklist->length].title = title;
				blacklist->items[blacklist->length].classname = classname;
				blacklist->length++;
			}
			//Switch gears?
			if (pos == NULL && blacklist != &settings.Snaplist) {
				blacklist_alloc = 0;
				if (blacklist == &settings.ProcessBlacklist) {
					blacklist = &settings.Blacklist;
					GetPrivateProfileString(L"Blacklist", L"Blacklist", L"", txt, sizeof(txt)/sizeof(wchar_t), inipath);
				}
				else if (blacklist == &settings.Blacklist) {
					blacklist = &settings.Snaplist;
					GetPrivateProfileString(L"Blacklist", L"Snaplist", L"", txt, sizeof(txt)/sizeof(wchar_t), inipath);
				}
				blacklist->data = malloc((wcslen(txt)+1)*sizeof(wchar_t));
				wcscpy(blacklist->data, txt);
				pos = blacklist->data;
			}
		}
		
		//Allocate space for wnds
		wnds_alloc += 20;
		wnds = realloc(wnds, wnds_alloc*sizeof(RECT));
		
		//Create a timer to do more initialization
		SetTimer(g_hwnd, INIT_TIMER, 10, NULL);
	}
	else if (reason == DLL_PROCESS_DETACH) {
		//Remove subclassing if a window is currently subclassed
		if (subclassed && IsWindow(state.hwnd)) {
			subclassed = !RemoveWindowSubclass(state.hwnd, CustomWndProc, 0);
			if (subclassed) {
				Error(L"RemoveWindowSubclass(hwnd, CustomWndProc, 0)", L"Failed to remove window subclassing.", -1, TEXT(__FILE__), __LINE__);
			}
		}
	}
	return TRUE;
}
