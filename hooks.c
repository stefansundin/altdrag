/*
	AltDrag - Drag windows with the mouse when pressing the alt key
	Copyright (C) 2009  Stefan Sundin (recover89@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

#define UNICODE
#define _UNICODE
#define _WIN32_WINNT 0x0500

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <shlwapi.h>

//App
#define APP_NAME L"AltDrag"
#define STICKY_THRESHOLD 20

//Some variables must be shared so the CallWndProc hooks can access them
#define shareattr __attribute__((section ("shared"), shared))

//States
int alt = 0;
int shift shareattr = 0;
int move shareattr = 0;
int resize shareattr = 0;
HWND hwnd = NULL;
unsigned int clicktime = 0;
POINT offset;
POINT resize_offset;
enum {TOP, RIGHT, BOTTOM, LEFT, CENTER} resize_x, resize_y;
int blockshift = 0;
int blockaltup = 0;

//Sticky
RECT *monitors = NULL;
int nummonitors = 0;
RECT *wnds = NULL;
int numwnds = 0;
HWND progman = NULL;

//Settings
struct {
	int Cursor;
	int AutoStick;
	int RMBMinimize;
} sharedsettings shareattr = {0,0,0};
int sharedsettings_loaded shareattr = 0;
wchar_t inipath[MAX_PATH] shareattr;
wchar_t txt[1000] shareattr;

//Blacklist
struct blacklistitem {
	wchar_t *title;
	wchar_t *classname;
};
struct blacklist {
	struct blacklistitem *items;
	int numitems;
};
struct {
	struct blacklist Blacklist;
	struct blacklist Blacklist_Sticky;
} settings = {{NULL,0},{NULL,0}};

//Roll-up data
#define NUMROLLUP 8
struct rollupdata {
	HWND hwnd;
	int width;
	int height;
};
struct rollupdata rollup[NUMROLLUP];
int rolluppos = 0;

//Cursor data
HWND cursorwnd shareattr = NULL;
HCURSOR cursor[6] shareattr;
enum {HAND, SIZENWSE, SIZENESW, SIZENS, SIZEWE, SIZEALL} resizecursor;

//Mousehook data
HINSTANCE hinstDLL = NULL;
HHOOK mousehook = NULL;

//Msghook data
WNDPROC oldwndproc = NULL;
enum {MOVE, RESIZE, NONE} msgaction = NONE;

//Error()
#ifdef DEBUG
#include "include/error.h"
#else
#define Error(a,b,c,d,e)
#endif

__declspec(dllexport) LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
#include "include/hooks.h"

void MoveWnd() {
	//Check if window still exists
	if (!IsWindow(hwnd)) {
		move = 0;
		UnhookMouse();
		return;
	}
	
	//Get window size
	RECT wnd;
	if (GetWindowRect(hwnd,&wnd) == 0) {
		Error(L"GetWindowRect()", L"MoveWnd()", GetLastError(), TEXT(__FILE__), __LINE__);
	}
	
	//Get new position for window
	POINT pt;
	GetCursorPos(&pt);
	int posx = pt.x-offset.x;
	int posy = pt.y-offset.y;
	int wndwidth = wnd.right-wnd.left;
	int wndheight = wnd.bottom-wnd.top;
	
	//Double check if any of the shift keys are being pressed
	if (shift && !(GetAsyncKeyState(VK_SHIFT)&0x8000)) {
		shift = 0;
	}
	
	//Check if window will stick anywhere
	if (shift || sharedsettings.AutoStick) {
		MoveStick(&posx, &posy, wndwidth, wndheight);
	}
	
	//Move
	if (MoveWindow(hwnd,posx,posy,wndwidth,wndheight,TRUE) == 0) {
		Error(L"MoveWindow()", L"MoveWnd()", GetLastError(), TEXT(__FILE__), __LINE__);
	}
}

void ResizeWnd() {
	//Check if window still exists
	if (!IsWindow(hwnd)) {
		resize = 0;
		UnhookMouse();
		return;
	}
	
	//Get window size
	RECT wnd;
	if (GetWindowRect(hwnd,&wnd) == 0) {
		Error(L"GetWindowRect()", L"ResizeWnd()", GetLastError(), TEXT(__FILE__), __LINE__);
	}
	
	//Get new pos and size for window
	POINT pt;
	GetCursorPos(&pt);
	int posx, posy, wndwidth, wndheight;
	if (resize_x == CENTER && resize_y == CENTER) {
		posx = wnd.left-(pt.x-resize_offset.x);
		posy = wnd.top-(pt.y-resize_offset.y);
		wndwidth = wnd.right-wnd.left+2*(pt.x-resize_offset.x);
		wndheight = wnd.bottom-wnd.top+2*(pt.y-resize_offset.y);
		resize_offset.x = pt.x;
		resize_offset.y = pt.y;
	}
	else {
		if (resize_y == TOP) {
			posy = pt.y-resize_offset.y;
			wndheight = wnd.bottom-pt.y+resize_offset.y;
		}
		else if (resize_y == CENTER) {
			posy = wnd.top;
			wndheight = wnd.bottom-wnd.top;
		}
		else if (resize_y == BOTTOM) {
			posy = wnd.top;
			wndheight = pt.y-wnd.top+resize_offset.y;
		}
		if (resize_x == LEFT) {
			posx = pt.x-resize_offset.x;
			wndwidth = wnd.right-pt.x+resize_offset.x;
		}
		else if (resize_x == CENTER) {
			posx = wnd.left;
			wndwidth = wnd.right-wnd.left;
		}
		else if (resize_x == RIGHT) {
			posx = wnd.left;
			wndwidth = pt.x-wnd.left+resize_offset.x;
		}
	}
	
	//Double check if any of the shift keys are being pressed
	if (shift && !(GetAsyncKeyState(VK_SHIFT)&0x8000)) {
		shift = 0;
	}
	
	//Check if window will stick anywhere
	if ((shift || sharedsettings.AutoStick) && (resize_x != CENTER || resize_y != CENTER)) {
		ResizeStick(&posx, &posy, &wndwidth, &wndheight);
	}
	
	//Resize
	if (MoveWindow(hwnd,posx,posy,wndwidth,wndheight,TRUE) == 0) {
		Error(L"MoveWindow()", L"ResizeWnd()", GetLastError(), TEXT(__FILE__), __LINE__);
	}
}

__declspec(dllexport) LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		int vkey = ((PKBDLLHOOKSTRUCT)lParam)->vkCode;
		
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
			if (!alt && (vkey == VK_LMENU || vkey == VK_RMENU)) {
				alt = 1;
				blockaltup = 0;
				clicktime = 0; //Reset double-click time
				//Don't hook the mouse if the foreground window is fullscreen
				HWND window = GetForegroundWindow();
				if (!(GetWindowLongPtr(window,GWL_STYLE)&WS_CAPTION)) {
					if (!IsWindow(progman)) {
						progman = FindWindow(L"Progman",L"Program Manager");
					}
					if (window != progman) {
						//Get window size
						RECT wnd;
						if (GetWindowRect(window,&wnd) == 0) {
							//Error(L"GetWindowRect(&wnd)", L"LowLevelKeyboardProc()", GetLastError(), TEXT(__FILE__), __LINE__);
							return CallNextHookEx(NULL, nCode, wParam, lParam);
						}
						/*
						if (nummonitors == 1) {
							//Return if the window is fullscreen
							if (wnd.left == monitors[0].left && wnd.top == monitors[0].top && wnd.right == monitors[0].right && wnd.bottom == monitors[0].bottom) {
								return CallNextHookEx(NULL, nCode, wParam, lParam);
							}
						}
						*/
						//Return if the window is fullscreen
						int i;
						for (i=0; i < nummonitors; i++) {
							if (wnd.left == monitors[i].left && wnd.top == monitors[i].top && wnd.right == monitors[i].right && wnd.bottom == monitors[i].bottom) {
								return CallNextHookEx(NULL, nCode, wParam, lParam);
							}
						}
					}
				}
				//Hook mouse
				HookMouse();
			}
			else if (vkey == VK_LSHIFT || vkey == VK_RSHIFT) {
				if (!shift) {
					shift = 1;
					if (move) {
						MoveWnd();
					}
					if (resize) {
						ResizeWnd();
					}
				}
				if (alt && blockshift) {
					//Block keypress to prevent Windows from changing keyboard layout
					return 1;
				}
			}
			else if (move && (vkey == VK_LCONTROL || vkey == VK_RCONTROL)) {
				//This doesn't always work since the menu is activated by the alt keypress (read msdn)
				SetForegroundWindow(hwnd);
			}
		}
		else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
			if ((vkey == VK_LMENU || vkey == VK_RMENU) && !(GetAsyncKeyState(VK_MENU)&0x8000)) {
				alt = 0;
				
				//Block the alt keyup to prevent the menu of the foreground window to be selected
				//This actually "disguises" the alt keypress in such a way that the menu will not be triggered
				//For more information, see issue 20
				if (blockaltup) {
					//Send a ctrl keydown and keyup event to disguise the alt keyup
					KEYBDINPUT ctrl[2];
					ctrl[0].wVk = VK_CONTROL;
					ctrl[0].wScan = 0;
					ctrl[0].dwFlags = 0;
					ctrl[0].time = 0;
					ctrl[0].dwExtraInfo = GetMessageExtraInfo();
					ctrl[1].wVk = VK_CONTROL;
					ctrl[1].wScan = 0;
					ctrl[1].dwFlags = KEYEVENTF_KEYUP;
					ctrl[1].time = 0;
					ctrl[1].dwExtraInfo = ctrl[0].dwExtraInfo;
					INPUT input[2];
					input[0].type = INPUT_KEYBOARD;
					input[0].ki = ctrl[0];
					input[1].type = INPUT_KEYBOARD;
					input[1].ki = ctrl[1];
					SendInput(2, input, sizeof(INPUT));
				}
				
				if (!move && !resize) {
					UnhookMouse();
				}
			}
			else if (vkey == VK_LSHIFT || vkey == VK_RSHIFT) {
				shift = 0;
				blockshift = 0;
				if (move) {
					MoveWnd();
				}
				if (resize) {
					ResizeWnd();
				}
			}
		}
	}
	
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

__declspec(dllexport) LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		if (alt && (wParam == WM_LBUTTONDOWN || wParam == WM_MBUTTONDOWN || wParam == WM_RBUTTONDOWN)) {
			//Double check if any alt key is being pressed
			if (!(GetAsyncKeyState(VK_MENU)&0x8000)) {
				alt = 0;
				UnhookMouse();
				return CallNextHookEx(NULL, nCode, wParam, lParam);
			}
			
			//Alt key is still being pressed
			POINT pt = ((PMSLLHOOKSTRUCT)lParam)->pt;
			
			//Make sure cursorwnd isn't in the way
			if (sharedsettings.Cursor) {
				ShowWindow(cursorwnd, SW_HIDE);
			}
			
			//Get window
			if ((hwnd=WindowFromPoint(pt)) == NULL) {
				return CallNextHookEx(NULL, nCode, wParam, lParam);
			}
			hwnd = GetAncestor(hwnd,GA_ROOT);
			
			//Return if window is blacklisted
			if (blacklisted(hwnd,&settings.Blacklist)) {
				return CallNextHookEx(NULL, nCode, wParam, lParam);
			}
			
			//Get window size
			RECT wnd;
			if (GetWindowRect(hwnd,&wnd) == 0) {
				Error(L"GetWindowRect(&wnd)", L"LowLevelMouseProc()", GetLastError(), TEXT(__FILE__), __LINE__);
			}
			//Enumerate monitors
			nummonitors = 0;
			EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);
			//Return if the window is fullscreen
			int i;
			if (!(GetWindowLongPtr(hwnd,GWL_STYLE)&WS_CAPTION)) {
				for (i=0; i < nummonitors; i++) {
					if (wnd.left == monitors[i].left && wnd.top == monitors[i].top && wnd.right == monitors[i].right && wnd.bottom == monitors[i].bottom) {
						return CallNextHookEx(NULL, nCode, wParam, lParam);
					}
				}
			}
			
			//Do things depending on what button was pressed
			if (wParam == WM_LBUTTONDOWN) {
				//Maximize window if this is a double-click
				if (GetTickCount()-clicktime <= GetDoubleClickTime()) {
					//Alt+double-clicking a window maximizes it
					//Maximize window
					WINDOWPLACEMENT wndpl;
					wndpl.length = sizeof(WINDOWPLACEMENT);
					GetWindowPlacement(hwnd, &wndpl);
					wndpl.showCmd = SW_MAXIMIZE;
					//Also roll-down the window if it's in the roll-up database
					for (i=0; i < NUMROLLUP; i++) {
						if (rollup[i].hwnd == hwnd) {
							//Roll-down window
							RECT normalpos = {wnd.left, wnd.top, wnd.left+rollup[i].width, wnd.top+rollup[i].height};
							wndpl.rcNormalPosition = normalpos;
							//Remove window from database
							rollup[i].hwnd = NULL;
						}
					}
					SetWindowPlacement(hwnd, &wndpl);
					//Stop move action
					move = 0;
					//Hide cursorwnd
					if (sharedsettings.Cursor) {
						ShowWindow(cursorwnd, SW_HIDE);
						SetWindowLongPtr(cursorwnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW); //Workaround for http://support.microsoft.com/kb/270624/
						//Maybe show IDC_SIZEALL cursor here really quick somehow?
					}
					//Prevent mousedown from propagating
					return 1;
				}
				
				//Restore the window if it's maximized
				if (IsZoomed(hwnd)) {
					//Restore window
					WINDOWPLACEMENT wndpl;
					wndpl.length = sizeof(WINDOWPLACEMENT);
					GetWindowPlacement(hwnd, &wndpl);
					wndpl.showCmd = SW_RESTORE;
					SetWindowPlacement(hwnd, &wndpl);
					
					//Get new pos and size
					RECT newwnd;
					if (GetWindowRect(hwnd,&newwnd) == 0) {
						Error(L"GetWindowRect()", L"LowLevelMouseProc()", GetLastError(), TEXT(__FILE__), __LINE__);
					}
					
					//Set offset
					offset.x = (float)(pt.x-wnd.left)/(wnd.right-wnd.left)*(newwnd.right-newwnd.left);
					offset.y = (float)(pt.y-wnd.top)/(wnd.bottom-wnd.top)*(newwnd.bottom-newwnd.top);
					
					//Move
					MoveWnd();
				}
				else {
					//Remember time of this click so we can check for double-click
					clicktime = GetTickCount();
					
					//Set offset
					offset.x = pt.x-wnd.left;
					offset.y = pt.y-wnd.top;
				}
				//Show cursorwnd
				if (sharedsettings.Cursor) {
					SetClassLongPtr(cursorwnd, GCLP_HCURSOR, (LONG_PTR)cursor[HAND]);
					if (!resize) {
						MoveWindow(cursorwnd, pt.x-20, pt.y-20, 41, 41, FALSE);
						SetWindowLongPtr(cursorwnd, GWL_EXSTYLE, WS_EX_LAYERED|WS_EX_TOOLWINDOW); //Workaround for http://support.microsoft.com/kb/270624/
						SetLayeredWindowAttributes(cursorwnd, 0, 1, LWA_ALPHA); //Almost transparent
					}
					ShowWindowAsync(cursorwnd, SW_SHOWNA);
				}
				//Ready to move window
				move = 1;
				blockshift = 1;
				blockaltup = 1;
				//Prevent mousedown from propagating
				return 1;
			}
			else if (sharedsettings.RMBMinimize && wParam == WM_RBUTTONDOWN) {
				//Minimize window
				WINDOWPLACEMENT wndpl;
				wndpl.length = sizeof(WINDOWPLACEMENT);
				GetWindowPlacement(hwnd, &wndpl);
				wndpl.showCmd = SW_MINIMIZE;
				SetWindowPlacement(hwnd, &wndpl);
				
				//Prevent mousedown from propagating
				return 1;
			}
			else {
				//Roll-down the window if it's in the roll-up database
				for (i=0; i < NUMROLLUP; i++) {
					if (rollup[i].hwnd == hwnd) {
						//Roll-down window
						if (MoveWindow(hwnd, wnd.left, wnd.top, rollup[i].width, rollup[i].height, TRUE) == 0) {
							Error(L"MoveWindow()", L"When rolling down window", GetLastError(), TEXT(__FILE__), __LINE__);
						}
						//Remove window from database
						rollup[i].hwnd = NULL;
						//Prevent mousedown from propagating
						return 1;
					}
				}
				
				//Roll-up window if this is a double-click (or if both middle and right mouse button is pressed)
				if (GetTickCount()-clicktime <= GetDoubleClickTime() || resize) {
					//Alt+middle-double-clicking a window makes it roll-up
					//Store window size
					rollup[rolluppos].hwnd = hwnd;
					rollup[rolluppos].width = wnd.right-wnd.left;
					rollup[rolluppos].height = wnd.bottom-wnd.top;
					rolluppos = (rolluppos+1)%NUMROLLUP;
					//Roll-up window
					if (MoveWindow(hwnd, wnd.left, wnd.top, wnd.right-wnd.left, 30, TRUE) == 0) {
						Error(L"MoveWindow()", L"Roll-up", GetLastError(), TEXT(__FILE__), __LINE__);
					}
					//Stop resize action
					resize = 1;
					//Hide cursorwnd
					if (sharedsettings.Cursor) {
						ShowWindow(cursorwnd, SW_HIDE);
						SetWindowLongPtr(cursorwnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW); //Workaround for http://support.microsoft.com/kb/270624/
					}
					//Prevent mousedown from propagating
					return 1;
				}
				
				//Restore the window if it's maximized
				if (IsZoomed(hwnd)) {
					//Restore window
					WINDOWPLACEMENT wndpl;
					wndpl.length = sizeof(WINDOWPLACEMENT);
					GetWindowPlacement(hwnd, &wndpl);
					wndpl.showCmd = SW_RESTORE;
					//Get new size
					HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
					MONITORINFO monitorinfo;
					monitorinfo.cbSize = sizeof(MONITORINFO);
					if (GetMonitorInfo(monitor,&monitorinfo) == FALSE) {
						Error(L"GetMonitorInfo(monitor)", L"LowLevelMouseProc()", GetLastError(), TEXT(__FILE__), __LINE__);
					}
					wndpl.rcNormalPosition = monitorinfo.rcWork;
					//Update window
					SetWindowPlacement(hwnd, &wndpl);
					wnd = monitorinfo.rcWork;
				}
				//Set edge and offset
				if (wnd.bottom-wnd.top < 60) {
					//This is a very thin window
					resize_y = BOTTOM;
					resize_offset.y = wnd.bottom-pt.y;
					if (pt.x-wnd.left < (wnd.right-wnd.left)/3) {
						resize_x = LEFT;
						resize_offset.x = pt.x-wnd.left;
					}
					else {
						resize_x = RIGHT;
						resize_offset.x = wnd.right-pt.x;
					}
				}
				else {
					//Think of the window as nine boxes
					if (pt.y-wnd.top < (wnd.bottom-wnd.top)/3) {
						resize_y = TOP;
						resize_offset.y = pt.y-wnd.top;
					}
					else if (pt.y-wnd.top < (wnd.bottom-wnd.top)*2/3) {
						resize_y = CENTER;
						resize_offset.y = pt.y; //Used only if both x and y are CENTER
					}
					else {
						resize_y = BOTTOM;
						resize_offset.y = wnd.bottom-pt.y;
					}
					if (pt.x-wnd.left < (wnd.right-wnd.left)/3) {
						resize_x = LEFT;
						resize_offset.x = pt.x-wnd.left;
					}
					else if (pt.x-wnd.left < (wnd.right-wnd.left)*2/3) {
						resize_x = CENTER;
						resize_offset.x = pt.x; //Used only if both x and y are CENTER
					}
					else {
						resize_x = RIGHT;
						resize_offset.x = wnd.right-pt.x;
					}
				}
				//Show cursorwnd
				if (sharedsettings.Cursor) {
					//Determine shape of cursor
					if ((resize_y == TOP && resize_x == LEFT)
					 || (resize_y == BOTTOM && resize_x == RIGHT)) {
						resizecursor = SIZENWSE;
					}
					else if ((resize_y == TOP && resize_x == RIGHT)
					 || (resize_y == BOTTOM && resize_x == LEFT)) {
						resizecursor = SIZENESW;
					}
					else if ((resize_y == TOP && resize_x == CENTER)
					 || (resize_y == BOTTOM && resize_x == CENTER)) {
						resizecursor = SIZENS;
					}
					else if ((resize_y == CENTER && resize_x == LEFT)
					 || (resize_y == CENTER && resize_x == RIGHT)) {
						resizecursor = SIZEWE;
					}
					else {
						resizecursor = SIZEALL;
					}
					//Change cursor
					if (!move) {
						MoveWindow(cursorwnd, pt.x-20, pt.y-20, 41, 41, FALSE);
						SetClassLongPtr(cursorwnd, GCLP_HCURSOR, (LONG_PTR)cursor[resizecursor]);
						SetWindowLongPtr(cursorwnd, GWL_EXSTYLE, WS_EX_LAYERED|WS_EX_TOOLWINDOW); //Workaround for http://support.microsoft.com/kb/270624/
						SetLayeredWindowAttributes(cursorwnd, 0, 1, LWA_ALPHA); //Almost transparent
					}
					ShowWindowAsync(cursorwnd, SW_SHOWNA);
				}
				//Remember time of this click so we can check for double-click
				clicktime = GetTickCount();
				//Ready to resize window
				resize = 1;
				//Prevent mousedown from propagating
				return 1;
			}
		}
		else if (wParam == WM_LBUTTONUP && move) {
			move = 0;
			if (!alt) {
				UnhookMouse();
			}
			//Hide cursorwnd
			if (sharedsettings.Cursor) {
				if (resize) {
					SetClassLongPtr(cursorwnd, GCLP_HCURSOR, (LONG_PTR)cursor[resizecursor]);
				}
				else {
					ShowWindow(cursorwnd, SW_HIDE);
					SetWindowLongPtr(cursorwnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW); //Workaround for http://support.microsoft.com/kb/270624/
				}
			}
			//Prevent mouseup from propagating
			return 1;
		}
		else if (sharedsettings.RMBMinimize && wParam == WM_RBUTTONUP && alt) {
			//Prevent mouseup from propagating
			return 1;
		}
		else if ((wParam == WM_MBUTTONUP || wParam == WM_RBUTTONUP) && (alt || resize)) {
			resize = 0;
			if (!alt) {
				UnhookMouse();
			}
			//Hide cursorwnd
			if (sharedsettings.Cursor && !move) {
				ShowWindow(cursorwnd, SW_HIDE);
				SetWindowLongPtr(cursorwnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW); //Workaround for http://support.microsoft.com/kb/270624/
			}
			//Prevent mouseup from propagating
			return 1;
		}
		else if (wParam == WM_MOUSEMOVE) {
			//Reset double-click time
			clicktime = 0; //This prevents me from double-clicking when running Windows virtualized.
			if (move || resize) {
				//Move cursorwnd
				if (sharedsettings.Cursor) {
					POINT pt = ((PMSLLHOOKSTRUCT)lParam)->pt;
					MoveWindow(cursorwnd, pt.x-20, pt.y-20, 41, 41, TRUE);
					//MoveWindow(cursorwnd,(prevpt.x<pt.x?prevpt.x:pt.x)-3,(prevpt.y<pt.y?prevpt.y:pt.y)-3,(pt.x>prevpt.x?pt.x-prevpt.x:prevpt.x-pt.x)+7,(pt.y>prevpt.y?pt.y-prevpt.y:prevpt.y-pt.y)+7,FALSE);
				}
				//Move or resize
				if (move) {
					//Move window
					MoveWnd();
				}
				else if (resize) {
					//Resize window
					ResizeWnd();
				}
			}
		}
	}
	
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		hinstDLL = hInstance;
		//Load settings
		//Settings shared with CallWndProc hook
		if (!sharedsettings_loaded) {
			sharedsettings_loaded = 1;
			//Have to store path to ini file at initial load so CallWndProc hooks can find it
			GetModuleFileName(NULL, inipath, sizeof(inipath)/sizeof(wchar_t));
			PathRenameExtension(inipath, L".ini");
			//Cursor
			GetPrivateProfileString(APP_NAME, L"Cursor", L"0", txt, sizeof(txt)/sizeof(wchar_t), inipath);
			swscanf(txt, L"%d", &sharedsettings.Cursor);
			if (sharedsettings.Cursor) {
				cursorwnd = FindWindow(APP_NAME,NULL);
				cursor[HAND]     = LoadImage(NULL, IDC_HAND,     IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
				cursor[SIZENWSE] = LoadImage(NULL, IDC_SIZENWSE, IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
				cursor[SIZENESW] = LoadImage(NULL, IDC_SIZENESW, IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
				cursor[SIZENS]   = LoadImage(NULL, IDC_SIZENS,   IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
				cursor[SIZEWE]   = LoadImage(NULL, IDC_SIZEWE,   IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
				cursor[SIZEALL]  = LoadImage(NULL, IDC_SIZEALL,  IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
			}
			//AutoStick
			GetPrivateProfileString(APP_NAME, L"AutoStick", L"0", txt, sizeof(txt)/sizeof(wchar_t), inipath);
			swscanf(txt, L"%d", &sharedsettings.AutoStick);
			//RMBMinimize
			GetPrivateProfileString(APP_NAME, L"RMBMinimize", L"0", txt, sizeof(txt)/sizeof(wchar_t), inipath);
			swscanf(txt, L"%d", &sharedsettings.RMBMinimize);
			//Zero-out roll-up hwnds
			int i;
			for (i=0; i < NUMROLLUP; i++) {
				rollup[i].hwnd = NULL;
			}
		}
		//Blacklist
		GetPrivateProfileString(APP_NAME, L"Blacklist", L"", txt, sizeof(txt)/sizeof(wchar_t), inipath);
		int blacklist_alloc = 0;
		wchar_t *pos = txt;
		struct blacklist *add_blacklist = &settings.Blacklist;
		while (pos != NULL) {
			wchar_t *title = pos;
			wchar_t *classname = wcsstr(pos,L"|");
			//Move pos to next item (if any)
			pos = wcsstr(pos,L",");
			if (pos != NULL) {
				*pos = '\0';
				pos++;
			}
			if (classname != NULL) {
				*classname = '\0';
				classname++;
			}
			//Allocate memory and copy over text
			wchar_t *item_title, *item_classname;
			if (!wcscmp(title,L"*")) {
				item_title = NULL;
			}
			else {
				item_title = malloc((wcslen(title)+1)*sizeof(wchar_t));
				wcscpy(item_title, title);
			}
			if (classname == NULL) {
				item_classname = NULL;
			}
			else {
				if (!wcscmp(classname,L"*")) {
					item_classname = NULL;
				}
				else {
					item_classname = malloc((wcslen(classname)+1)*sizeof(wchar_t));
					wcscpy(item_classname, classname);
				}
			}
			//Store item if it's not empty
			if (item_title != NULL || item_classname != NULL) {
				//Make sure we have enough space
				if (add_blacklist->numitems == blacklist_alloc) {
					blacklist_alloc += 10;
					add_blacklist->items = realloc(add_blacklist->items,blacklist_alloc*sizeof(struct blacklistitem));
				}
				//Store item
				add_blacklist->items[add_blacklist->numitems].title = item_title;
				add_blacklist->items[add_blacklist->numitems].classname = item_classname;
				add_blacklist->numitems++;
			}
			//Switch gears to Blacklist_Sticky?
			if (pos == NULL && add_blacklist == &settings.Blacklist) {
				add_blacklist = &settings.Blacklist_Sticky;
				blacklist_alloc = 0;
				GetPrivateProfileString(APP_NAME, L"Blacklist_Sticky", L"", txt, sizeof(txt)/sizeof(wchar_t), inipath);
				pos = txt;
			}
		}
		//Allocate space for wnds
		wnds_alloc += 5;
		wnds = realloc(wnds,wnds_alloc*sizeof(RECT));
		if (wnds == NULL) {
			Error(L"realloc(wnds)", L"Out of memory?", 0, TEXT(__FILE__), __LINE__);
			return FALSE;
		}
	}
	else if (reason == DLL_PROCESS_DETACH) {
		//Restore old WndProc
		if (oldwndproc != NULL && IsWindow(hwnd)) {
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)oldwndproc);
		}
		//Free memory
		int i;
		for (i=0; i < settings.Blacklist.numitems; i++) {
			free(settings.Blacklist.items[i].title);
			free(settings.Blacklist.items[i].classname);
		}
		settings.Blacklist.numitems = 0;
		free(settings.Blacklist.items);
		for (i=0; i < settings.Blacklist_Sticky.numitems; i++) {
			free(settings.Blacklist_Sticky.items[i].title);
			free(settings.Blacklist_Sticky.items[i].classname);
		}
		settings.Blacklist_Sticky.numitems = 0;
		free(settings.Blacklist_Sticky.items);
		free(wnds);
	}
	return TRUE;
}
