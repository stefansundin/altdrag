/*
	AltDrag - Drag windows with the mouse when pressing the alt key
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
#include <time.h>
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <shlwapi.h>

#define DEBUG

static wchar_t txt[1000];
struct {
	int Cursor;
} settings={0};

//shift, move, resize and hwnd must be shared since CallWndProc is called in the context of another thread
static int alt=0;
static int shift __attribute__((section ("shared"), shared)) = 0;
static int move __attribute__((section ("shared"), shared)) = 0;
static int resize __attribute__((section ("shared"), shared)) = 0;
static HWND hwnd __attribute__((section ("shared"), shared)) = NULL;
static time_t clicktime=0;
static POINT offset;
static POINT resize_offset;
enum {TOP, RIGHT, BOTTOM, LEFT, CENTER} static resize_edge_x, resize_edge_y;
static HWND *wnds=NULL;
static int numwnds=0;
static int winxp=0;
struct blacklistitem {
	wchar_t *title;
	wchar_t *classname;
} *blacklist=NULL, *blacklist_sticky=NULL;
static int numblacklist=0;
static int numblacklist_sticky=0;

//Cursor data
static HWND cursorwnd=NULL;
enum cursornames {HAND, SIZENWSE, SIZENESW, SIZENS, SIZEWE, SIZEALL};
static HCURSOR cursor[6];

//Mousehook data
static HINSTANCE hinstDLL=NULL;
static HHOOK mousehook=NULL;

//Error message handling
LRESULT CALLBACK ErrorMsgProc(INT nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HCBT_ACTIVATE) {
		//Edit the caption of the buttons
		SetDlgItemText((HWND)wParam,IDYES,L"Copy error");
		SetDlgItemText((HWND)wParam,IDNO,L"OK");
	}
	return 0;
}

void Error(wchar_t *func, wchar_t *info, int errorcode, int line) {
	#ifdef DEBUG
	//Format message
	wchar_t errormsg[100];
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL,errorcode,0,errormsg,sizeof(errormsg)/sizeof(wchar_t),NULL);
	errormsg[wcslen(errormsg)-2]='\0'; //Remove that damn newline at the end of the formatted error message
	swprintf(txt,L"%s failed in file %s, line %d.\nError: %s (%d)\n\n%s", func, TEXT(__FILE__), line, errormsg, errorcode, info);
	//Display message
	HHOOK hhk=SetWindowsHookEx(WH_CBT, &ErrorMsgProc, 0, GetCurrentThreadId());
	int response=MessageBox(NULL, txt, L"AltDrag hooks Error", MB_ICONERROR|MB_YESNO|MB_DEFBUTTON2);
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
	#endif
}

static int wnds_alloc=0;
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	//Make sure we have enough space allocated
	if (numwnds == wnds_alloc) {
		wnds_alloc+=100;
		if ((wnds=realloc(wnds,wnds_alloc*sizeof(HWND))) == NULL) {
			Error(L"realloc(wnds)",L"Out of memory?",0,__LINE__);
			return FALSE;
		}
	}
	//Only store window if it's visible, not minimized to taskbar, not maximized and not the window we are dragging
	if (IsWindowVisible(hwnd) && !IsIconic(hwnd) && !IsZoomed(hwnd) && hwnd != (HWND)lParam) {
		//Check if window is blacklisted
		wchar_t title[256];
		wchar_t classname[256];
		GetWindowText(hwnd,title,sizeof(title)/sizeof(wchar_t));
		GetClassName(hwnd,classname,sizeof(classname)/sizeof(wchar_t));
		int i,blacklisted=0;
		for (i=0; i < numblacklist_sticky; i++) {
			if ((blacklist_sticky[i].title == NULL && !wcscmp(classname,blacklist_sticky[i].classname))
			 || (blacklist_sticky[i].classname == NULL && !wcscmp(title,blacklist_sticky[i].title))
			 || (blacklist_sticky[i].title != NULL && blacklist_sticky[i].classname != NULL && !wcscmp(title,blacklist_sticky[i].title) && !wcscmp(classname,blacklist[i].classname))) {
				blacklisted=1;
			}
		}
		//Add window to wnds
		if (!blacklisted) {
			wnds[numwnds++]=hwnd;
		}
	}
	//If this window is maximized we don't want to stick to any windows that might be under it
	if (IsZoomed(hwnd)) {
		wnds[numwnds++]=GetDesktopWindow(); //Since desktop is the last hwnd to be added, we need to add it now before returning
		return FALSE;
	}
	return TRUE;
}

void MoveWnd() {
	//Check if window still exists
	if (!IsWindow(hwnd)) {
		move=0;
		UnhookMouse();
		return;
	}
	
	/*//Use this to print all visible windows
	numwnds=0;
	EnumWindows(EnumWindowsProc,0);
	FILE *f=fopen("C:\\altdrag-log.txt","wb");
	fprintf(f,"numwnds: %d\n",numwnds);
	char title[100];
	char classname[100];
	int j;
	for (j=0; j < numwnds; j++) {
		GetWindowText(wnds[j],title,100);
		GetClassName(wnds[j],classname,100);
		RECT wndsize;
		GetWindowRect(wnds[j],&wndsize);
		fprintf(f,"wnd #%03d:%s %s [%s] (%dx%d@%dx%d)\n",j,(hwnd==wnds[j]?"***":""),title,classname,wndsize.right-wndsize.left,wndsize.bottom-wndsize.top,wndsize.left,wndsize.top);
	}
	fclose(f);*/
	
	//Reset double-click time
	clicktime=0;
	
	//Get window size
	RECT wnd;
	if (GetWindowRect(hwnd,&wnd) == 0) {
		Error(L"GetWindowRect()",L"MoveWnd()",GetLastError(),__LINE__);
	}
	
	//Get new position for window
	POINT pt;
	GetCursorPos(&pt);
	int posx=pt.x-offset.x;
	int posy=pt.y-offset.y;
	int wndwidth=wnd.right-wnd.left;
	int wndheight=wnd.bottom-wnd.top;
	
	//Double check if any of the shift keys are being pressed
	if (shift && !(GetAsyncKeyState(VK_SHIFT)&0x8000)) {
		shift=0;
	}
	
	//Check if window will stick anywhere
	if (shift) {
		numwnds=0;
		EnumWindows(EnumWindowsProc,(LPARAM)hwnd);
		
		/*//Use this to print the windows in wnds
		FILE *f=fopen("C:\\altdrag-log.txt","wb");
		fprintf(f,"numwnds: %d\n",numwnds);
		char title[100];
		char classname[100];
		int j;
		for (j=0; j < numwnds; j++) {
			GetWindowText(wnds[j],title,100);
			GetClassName(wnds[j],classname,100);
			RECT wndsize;
			GetWindowRect(wnds[j],&wndsize);
			fprintf(f,"wnd #%03d: %s [%s] (%dx%d@%dx%d)\n",j,title,classname,wndsize.right-wndsize.left,wndsize.bottom-wndsize.top,wndsize.left,wndsize.top);
		}
		fclose(f);*/
		
		//thresholdx and thresholdy will shrink to make sure the dragged window will stick to the closest windows
		int i, thresholdx=20, thresholdy=20, stuckx=0, stucky=0, stickx=0, sticky=0;
		//Loop windows
		for (i=0; i < numwnds; i++) {
			RECT stickywnd;
			if (GetWindowRect(wnds[i],&stickywnd) == 0) {
				Error(L"GetWindowRect()",L"MoveWnd()",GetLastError(),__LINE__);
			}
			
			//Check if posx sticks
			if ((stickywnd.top-thresholdx < posy && posy < stickywnd.bottom+thresholdx)
			 || (posy-thresholdx < stickywnd.top && stickywnd.top < posy+wndheight+thresholdx)) {
				if (posx-thresholdx < stickywnd.right && stickywnd.right < posx+thresholdx) {
					//The left edge of the dragged window will stick to this window's right edge
					stuckx=1;
					stickx=stickywnd.right;
					thresholdx=stickywnd.right-posx;
				}
				else if (posx+wndwidth-thresholdx < stickywnd.right && stickywnd.right < posx+wndwidth+thresholdx) {
					//The right edge of the dragged window will stick to this window's right edge
					stuckx=1;
					stickx=stickywnd.right-wndwidth;
					thresholdx=stickywnd.right-(posx+wndwidth);
				}
				else if (posx-thresholdx < stickywnd.left && stickywnd.left < posx+thresholdx) {
					//The left edge of the dragged window will stick to this window's left edge
					stuckx=1;
					stickx=stickywnd.left;
					thresholdx=stickywnd.left-posx;
				}
				else if (posx+wndwidth-thresholdx < stickywnd.left && stickywnd.left < posx+wndwidth+thresholdx) {
					//The right edge of the dragged window will stick to this window's left edge
					stuckx=1;
					stickx=stickywnd.left-wndwidth;
					thresholdx=stickywnd.left-(posx+wndwidth);
				}
			}
			
			//Check if posy sticks
			if ((stickywnd.left-thresholdy < posx && posx < stickywnd.right+thresholdy)
			 || (posx-thresholdy < stickywnd.left && stickywnd.left < posx+wndwidth+thresholdy)) {
				if (posy-thresholdy < stickywnd.bottom && stickywnd.bottom < posy+thresholdy) {
					//The top edge of the dragged window will stick to this window's bottom edge
					stucky=1;
					sticky=stickywnd.bottom;
					thresholdy=stickywnd.bottom-posy;
				}
				else if (posy+wndheight-thresholdy < stickywnd.bottom && stickywnd.bottom < posy+wndheight+thresholdy) {
					//The bottom edge of the dragged window will stick to this window's bottom edge
					stucky=1;
					sticky=stickywnd.bottom-wndheight;
					thresholdy=stickywnd.bottom-(posy+wndheight);
				}
				else if (posy-thresholdy < stickywnd.top && stickywnd.top < posy+thresholdy) {
					//The top edge of the dragged window will stick to this window's top edge
					stucky=1;
					sticky=stickywnd.top;
					thresholdy=stickywnd.top-posy;
				}
				else if (posy+wndheight-thresholdy < stickywnd.top && stickywnd.top < posy+wndheight+thresholdy) {
					//The bottom edge of the dragged window will stick to this window's top edge
					stucky=1;
					sticky=stickywnd.top-wndheight;
					thresholdy=stickywnd.top-(posy+wndheight);
				}
			}
		}
		
		//Did we stick somewhere?
		if (stuckx) {
			posx=stickx;
		}
		if (stucky) {
			posy=sticky;
		}
	}
	
	//Move
	if (MoveWindow(hwnd,posx,posy,wndwidth,wndheight,TRUE) == 0) {
		Error(L"MoveWindow()",L"MoveWnd()",GetLastError(),__LINE__);
	}
	
	/*FILE *f=fopen("C:\\altdrag-log.txt","ab");
	char title[100], classname[100];
	GetWindowText(hwnd,title,100);
	GetClassName(hwnd,classname,100);
	fprintf(f,"Moving window %s [%s] to (%d,%d)\n",title,classname,posx,posy);
	fclose(f);*/
}

void ResizeWnd() {
	//Check if window still exists
	if (!IsWindow(hwnd)) {
		resize=0;
		UnhookMouse();
		return;
	}
	
	//Get window size
	RECT wnd;
	if (GetWindowRect(hwnd,&wnd) == 0) {
		Error(L"GetWindowRect()",L"ResizeWnd()",GetLastError(),__LINE__);
	}
	
	//Get new pos and size for window
	POINT pt;
	GetCursorPos(&pt);
	int posx,posy,wndwidth,wndheight;
	if (resize_edge_x == CENTER && resize_edge_y == CENTER) {
		posx=wnd.left-(pt.x-resize_offset.x);
		posy=wnd.top-(pt.y-resize_offset.y);
		wndwidth=wnd.right-wnd.left+2*(pt.x-resize_offset.x);
		wndheight=wnd.bottom-wnd.top+2*(pt.y-resize_offset.y);
		resize_offset.x=pt.x;
		resize_offset.y=pt.y;
	}
	else {
		if (resize_edge_y == TOP) {
			posy=pt.y-resize_offset.y;
			wndheight=wnd.bottom-pt.y+resize_offset.y;
		}
		else if (resize_edge_y == CENTER) {
			posy=wnd.top;
			wndheight=wnd.bottom-wnd.top;
		}
		else if (resize_edge_y == BOTTOM) {
			posy=wnd.top;
			wndheight=pt.y-wnd.top+resize_offset.y;
		}
		if (resize_edge_x == LEFT) {
			posx=pt.x-resize_offset.x;
			wndwidth=wnd.right-pt.x+resize_offset.x;
		}
		else if (resize_edge_x == CENTER) {
			posx=wnd.left;
			wndwidth=wnd.right-wnd.left;
		}
		else if (resize_edge_x == RIGHT) {
			posx=wnd.left;
			wndwidth=pt.x-wnd.left+resize_offset.x;
		}
	}
	
	//Resize
	if (MoveWindow(hwnd,posx,posy,wndwidth,wndheight,TRUE) == 0) {
		Error(L"MoveWindow()",L"ResizeWnd()",GetLastError(),__LINE__);
	}
}

_declspec(dllexport) LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		int vkey=((PKBDLLHOOKSTRUCT)lParam)->vkCode;
		
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
			if (!alt && vkey == VK_LMENU) {
				alt=1;
				HookMouse();
			}
			else if (!shift && (vkey == VK_LSHIFT || vkey == VK_RSHIFT)) {
				shift=1;
				if (move) {
					MoveWnd();
					if (winxp) {
						//Block key to prevent XP from changing keyboard layout
						return 1;
					}
				}
			}
			else if (move && (vkey == VK_LCONTROL || vkey == VK_RCONTROL)) {
				//This doesn't always work since the menu is activated by the alt keypress, read msdn
				SetForegroundWindow(hwnd);
			}
		}
		else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
			if (vkey == VK_LMENU) {
				alt=0;
				if (!move && !resize) {
					UnhookMouse();
				}
			}
			else if (vkey == VK_LSHIFT || vkey == VK_RSHIFT) {
				shift=0;
				if (move) {
					MoveWnd();
				}
			}
		}
	}
	
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

_declspec(dllexport) LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		if (wParam == WM_LBUTTONDOWN && alt && !move) {
			//Double check if the left alt key is being pressed
			if (!(GetAsyncKeyState(VK_LMENU)&0x8000)) {
				alt=0;
				UnhookMouse();
			}
			else {
				//Alt key is still being pressed
				POINT pt=((PMSLLHOOKSTRUCT)lParam)->pt;
				
				//Make sure cursorwnd isn't in the way
				if (settings.Cursor) {
					ShowWindow(cursorwnd,SW_HIDE);
				}
				
				//Get window
				if ((hwnd=WindowFromPoint(pt)) == NULL) {
					Error(L"WindowFromPoint()",L"LowLevelMouseProc()",GetLastError(),__LINE__);
				}
				
				/*//Use this to print the title and classname of the child you press
				FILE *f=fopen("C:\\altdrag-log.txt","wb");
				char title[100], classname[100];
				GetWindowText(hwnd,title,100);
				GetClassName(hwnd,classname,100);
				fprintf(f,"Pressed %s [%s]\n",title,classname);
				fclose(f);*/
				
				hwnd=GetAncestor(hwnd,GA_ROOT);
				
				//Check if window is blacklisted
				wchar_t title[256];
				wchar_t classname[256];
				GetWindowText(hwnd,title,sizeof(title)/sizeof(wchar_t));
				GetClassName(hwnd,classname,sizeof(classname)/sizeof(wchar_t));
				int i,blacklisted=0;
				for (i=0; i < numblacklist; i++) {
					//swprintf(txt,L"if (%s == NULL && %s == %s)\n || (%s == NULL && %s == %s)\n || (%s == %s && %s == %s)",blacklist[i].title,classname,blacklist[i].classname,blacklist[i].classname,title,blacklist[i].title,title,blacklist[i].title,classname,blacklist[i].classname);
					//MessageBox(NULL, txt, L"Debug message", MB_ICONINFORMATION|MB_OK);
					if ((blacklist[i].title == NULL && !wcscmp(classname,blacklist[i].classname))
					 || (blacklist[i].classname == NULL && !wcscmp(title,blacklist[i].title))
					 || (blacklist[i].title != NULL && blacklist[i].classname != NULL && !wcscmp(title,blacklist[i].title) && !wcscmp(classname,blacklist[i].classname))) {
						blacklisted=1;
					}
				}
				
				//Only continue if this window is not blacklisted
				if (!blacklisted) {
					//Get window and desktop size
					RECT window;
					if (GetWindowRect(hwnd,&window) == 0) {
						Error(L"GetWindowRect()",L"LowLevelMouseProc()",GetLastError(),__LINE__);
					}
					RECT desktop;
					if (GetWindowRect(GetDesktopWindow(),&desktop) == 0) {
						Error(L"GetWindowRect()",L"LowLevelMouseProc()",GetLastError(),__LINE__);
					}
					
					//Only move if the window is not fullscreen
					if (window.left != desktop.left || window.top != desktop.top || window.right != desktop.right || window.bottom != desktop.bottom) {
						//Check if this is a double-click
						if (difftime(time(NULL),clicktime) <= GetDoubleClickTime()/1000) {
							//Alt+double-clicking a window maxmizes it
							//Reset double-click time
							clicktime=0;
							//Maximize window
							WINDOWPLACEMENT wndpl;
							wndpl.length=sizeof(WINDOWPLACEMENT);
							GetWindowPlacement(hwnd,&wndpl);
							wndpl.showCmd=SW_MAXIMIZE;
							SetWindowPlacement(hwnd,&wndpl);
							//Stop move action
							move=0;
							if (!alt) {
								UnhookMouse();
							}
							//Hide cursorwnd
							if (settings.Cursor) {
								ShowWindow(cursorwnd,SW_HIDE);
								SetWindowLongPtr(cursorwnd,GWL_EXSTYLE,WS_EX_TOOLWINDOW); //Workaround for http://support.microsoft.com/kb/270624/
								//Maybe show IDC_SIZEALL cursor here really quick somehow?
							}
							//Prevent mousedown from propagating
							return 1;
						}
						else {
							//Restore the window if it's maximized
							if (IsZoomed(hwnd)) {
								//Restore window
								WINDOWPLACEMENT wndpl;
								wndpl.length=sizeof(WINDOWPLACEMENT);
								GetWindowPlacement(hwnd,&wndpl);
								wndpl.showCmd=SW_RESTORE;
								SetWindowPlacement(hwnd,&wndpl);
								
								//Get new pos and size
								RECT newwindow;
								if (GetWindowRect(hwnd,&newwindow) == 0) {
									Error(L"GetWindowRect()",L"LowLevelMouseProc()",GetLastError(),__LINE__);
								}
								
								//Set offset
								offset.x=(float)(pt.x-window.left)/(window.right-window.left)*(newwindow.right-newwindow.left);
								offset.y=(float)(pt.y-window.top)/(window.bottom-window.top)*(newwindow.bottom-newwindow.top);
								
								//Move
								MoveWnd();
							}
							else {
								//Remember time of this click so we can check for double-click
								clicktime=time(NULL);
								
								//Set offset
								offset.x=pt.x-window.left;
								offset.y=pt.y-window.top;
							}
							//Show cursorwnd
							if (settings.Cursor) {
								SetClassLongPtr(cursorwnd,GCLP_HCURSOR,(LONG_PTR)cursor[HAND]);
								if (!resize) {
									MoveWindow(cursorwnd,pt.x-20,pt.y-20,41,41,FALSE);
									SetWindowLongPtr(cursorwnd,GWL_EXSTYLE,WS_EX_LAYERED|WS_EX_TOOLWINDOW); //Workaround for http://support.microsoft.com/kb/270624/
									if (winxp) {
										SetLayeredWindowAttributes(cursorwnd,0,1,LWA_ALPHA); //Almost transparent (XP fix)
									}
								}
								ShowWindowAsync(cursorwnd,SW_SHOWNA);
							}
							//Ready to move window
							move=1;
							//Prevent mousedown from propagating
							return 1;
						}
					}
				}
			}
		}
		else if (wParam == WM_LBUTTONUP && move) {
			move=0;
			if (!alt) {
				UnhookMouse();
			}
			//Hide cursorwnd
			if (settings.Cursor) {
				if (resize) {
					SetClassLongPtr(cursorwnd,GCLP_HCURSOR,(LONG_PTR)cursor[SIZEALL]);
				}
				else {
					ShowWindow(cursorwnd,SW_HIDE);
					SetWindowLongPtr(cursorwnd,GWL_EXSTYLE,WS_EX_TOOLWINDOW); //Workaround for http://support.microsoft.com/kb/270624/
				}
			}
			//Prevent mouseup from propagating
			return 1;
		}
		else if ((wParam == WM_MBUTTONDOWN || wParam == WM_RBUTTONDOWN) && alt && !resize) {
			//Double check if the left alt key is being pressed
			if (!(GetAsyncKeyState(VK_LMENU)&0x8000)) {
				alt=0;
				UnhookMouse();
			}
			else {
				//Alt key is still being pressed
				POINT pt=((PMSLLHOOKSTRUCT)lParam)->pt;
				
				//Make sure cursorwnd isn't in the way
				if (settings.Cursor) {
					ShowWindow(cursorwnd,SW_HIDE);
				}
				
				//Get window
				if ((hwnd=WindowFromPoint(pt)) == NULL) {
					Error(L"WindowFromPoint()",L"LowLevelMouseProc()",GetLastError(),__LINE__);
				}
				hwnd=GetAncestor(hwnd,GA_ROOT);
				
				//Get window and desktop size
				RECT window;
				if (GetWindowRect(hwnd,&window) == 0) {
					Error(L"GetWindowRect()",L"LowLevelMouseProc()",GetLastError(),__LINE__);
				}
				RECT desktop;
				if (GetWindowRect(GetDesktopWindow(),&desktop) == 0) {
					Error(L"GetWindowRect()",L"LowLevelMouseProc()",GetLastError(),__LINE__);
				}
				
				//Only resize if the window is not fullscreen
				if (window.left != desktop.left || window.top != desktop.top || window.right != desktop.right || window.bottom != desktop.bottom) {
					//Restore the window if it's maximized
					if (IsZoomed(hwnd)) {
						//Restore window
						WINDOWPLACEMENT wndpl;
						wndpl.length=sizeof(WINDOWPLACEMENT);
						GetWindowPlacement(hwnd,&wndpl);
						wndpl.showCmd=SW_RESTORE;
						//New size
						RECT normalpos={desktop.left,desktop.top,desktop.right,desktop.bottom};
						//Compensate for taskbar
						RECT taskbar;
						if (GetWindowRect(FindWindow(L"Shell_TrayWnd",NULL),&taskbar)) {
							if (taskbar.left == desktop.left && taskbar.right == desktop.right) {
								//Taskbar is on the bottom or top position, adjust height
								normalpos.bottom-=taskbar.bottom-taskbar.top;
							}
							else if (taskbar.top == desktop.top && taskbar.bottom == desktop.bottom) {
								//Taskbar is on the left or right position, adjust width
								normalpos.right-=taskbar.right-taskbar.left;
							}
						}
						wndpl.rcNormalPosition=normalpos;
						SetWindowPlacement(hwnd,&wndpl);
						//Update window size
						GetWindowRect(hwnd,&window);
					}
					//Set edge and offset
					if (pt.y-window.top < (window.bottom-window.top)/3) {
						resize_edge_y=TOP;
						resize_offset.y=pt.y-window.top;
					}
					else if (pt.y-window.top < (window.bottom-window.top)*2/3) {
						resize_edge_y=CENTER;
						resize_offset.y=pt.y; //Used only if both x and y are CENTER
					}
					else {
						resize_edge_y=BOTTOM;
						resize_offset.y=window.bottom-pt.y;
					}
					if (pt.x-window.left < (window.right-window.left)/3) {
						resize_edge_x=LEFT;
						resize_offset.x=pt.x-window.left;
					}
					else if (pt.x-window.left < (window.right-window.left)*2/3) {
						resize_edge_x=CENTER;
						resize_offset.x=pt.x; //Used only if both x and y are CENTER
					}
					else {
						resize_edge_x=RIGHT;
						resize_offset.x=window.right-pt.x;
					}
					//Show cursorwnd
					if (settings.Cursor) {
						if (!move) {
							MoveWindow(cursorwnd,pt.x-20,pt.y-20,41,41,FALSE);
							if ((resize_edge_y == TOP && resize_edge_x == LEFT)
							 || (resize_edge_y == BOTTOM && resize_edge_x == RIGHT)) {
								SetClassLongPtr(cursorwnd,GCLP_HCURSOR,(LONG_PTR)cursor[SIZENWSE]);
							}
							else if ((resize_edge_y == TOP && resize_edge_x == RIGHT)
							 || (resize_edge_y == BOTTOM && resize_edge_x == LEFT)) {
								SetClassLongPtr(cursorwnd,GCLP_HCURSOR,(LONG_PTR)cursor[SIZENESW]);
							}
							else if ((resize_edge_y == TOP && resize_edge_x == CENTER)
							 || (resize_edge_y == BOTTOM && resize_edge_x == CENTER)) {
								SetClassLongPtr(cursorwnd,GCLP_HCURSOR,(LONG_PTR)cursor[SIZENS]);
							}
							else if ((resize_edge_y == CENTER && resize_edge_x == LEFT)
							 || (resize_edge_y == CENTER && resize_edge_x == RIGHT)) {
								SetClassLongPtr(cursorwnd,GCLP_HCURSOR,(LONG_PTR)cursor[SIZEWE]);
							}
							else {
								SetClassLongPtr(cursorwnd,GCLP_HCURSOR,(LONG_PTR)cursor[SIZEALL]);
							}
							SetWindowLongPtr(cursorwnd,GWL_EXSTYLE,WS_EX_LAYERED|WS_EX_TOOLWINDOW); //Workaround for http://support.microsoft.com/kb/270624/
							if (winxp) {
								SetLayeredWindowAttributes(cursorwnd,0,1,LWA_ALPHA); //Almost transparent (XP fix)
							}
						}
						ShowWindowAsync(cursorwnd,SW_SHOWNA);
					}
					//Ready to resize window
					resize=1;
					//Prevent mousedown from propagating
					return 1;
				}
			}
		}
		else if ((wParam == WM_MBUTTONUP || wParam == WM_RBUTTONUP) && resize) {
			resize=0;
			if (!alt) {
				UnhookMouse();
			}
			//Hide cursorwnd
			if (settings.Cursor && !move) {
				ShowWindow(cursorwnd,SW_HIDE);
				SetWindowLongPtr(cursorwnd,GWL_EXSTYLE,WS_EX_TOOLWINDOW); //Workaround for http://support.microsoft.com/kb/270624/
			}
			//Prevent mouseup from propagating
			return 1;
		}
		else if (wParam == WM_MOUSEMOVE && (move || resize)) {
			//Move cursorwnd
			if (settings.Cursor) {
				POINT pt=((PMSLLHOOKSTRUCT)lParam)->pt;
				MoveWindow(cursorwnd,pt.x-20,pt.y-20,41,41,TRUE);
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
	
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

//CallWndProc is called in the context of the thread that calls SendMessage, not the thread that receives the message.
//Thus we have to explicitly share the memory we want CallWndProc to be able to access (shift, move and hwnd)
_declspec(dllexport) LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION && !move) {
		CWPSTRUCT *msg=(CWPSTRUCT*)lParam;
		
		/*if (shift) {
			FILE *f=fopen("C:\\callwndproc.log","ab"); //Important to specify the full path here since CallWndProc is called in the context of another thread.
			fprintf(f,"message: %d\n",msg->message);
			fclose(f);
		}*/
		
		if (msg->message == WM_WINDOWPOSCHANGED && shift && IsWindowVisible(msg->hwnd) && !IsIconic(hwnd) && !IsZoomed(msg->hwnd)) {
			//Set offset
			POINT pt;
			GetCursorPos(&pt);
			WINDOWPOS *wndpos=(WINDOWPOS*)msg->lParam;
			offset.x=pt.x-wndpos->x;
			offset.y=pt.y-wndpos->y;
			//Set hwnd
			hwnd=msg->hwnd;
			//Move window
			MoveWnd();
		}
	}
	
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int HookMouse() {
	if (mousehook) {
		//Mouse already hooked
		return 1;
	}
	
	//Set up the mouse hook
	if ((mousehook=SetWindowsHookEx(WH_MOUSE_LL,LowLevelMouseProc,hinstDLL,0)) == NULL) {
		Error(L"SetWindowsHookEx(WH_MOUSE_LL)",L"",GetLastError(),__LINE__);
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
	
	//Remove mouse hook
	if (UnhookWindowsHookEx(mousehook) == 0) {
		Error(L"UnhookWindowsHookEx(mousehook)",L"",GetLastError(),__LINE__);
		return 1;
	}
	
	//Success
	mousehook=NULL;
	return 0;
}

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		hinstDLL=hInstance;
		//Load settings
		wchar_t path[MAX_PATH];
		GetModuleFileName(NULL,path,sizeof(path)/sizeof(wchar_t));
		PathRenameExtension(path,L".ini");
		GetPrivateProfileString(L"AltDrag",L"Cursor",L"0",txt,sizeof(txt)/sizeof(wchar_t),path);
		swscanf(txt,L"%d",&settings.Cursor);
		//Blacklist
		GetPrivateProfileString(L"AltDrag",L"Blacklist",L"",txt,sizeof(txt)/sizeof(wchar_t),path);
		int *add_numblacklist=&numblacklist, blacklist_alloc=0;
		wchar_t *pos=txt, title[256], classname[256];
		struct blacklistitem **add_blacklist=&blacklist;
		while (pos != NULL) {
			wchar_t *title=pos;
			wchar_t *classname=wcsstr(pos,L"|");
			pos=wcsstr(pos,L",");
			if (pos != NULL) {
				*pos='\0';
				pos++;
			}
			if (classname != NULL) {
				*classname='\0';
				classname++;
			}
			//Make sure we have enough space
			if (*add_numblacklist == blacklist_alloc) {
				blacklist_alloc+=10;
				*add_blacklist=realloc(*add_blacklist,blacklist_alloc*sizeof(struct blacklistitem));
			}
			//Allocate memory for title and classname
			wchar_t *item_title, *item_classname;
			if (wcslen(title) > 0) {
				item_title=malloc((wcslen(title)+1)*sizeof(wchar_t));
				wcscpy(item_title,title);
			}
			else {
				item_title=NULL;
			}
			if (classname != NULL && wcslen(classname) > 0) {
				item_classname=malloc((wcslen(classname)+1)*sizeof(wchar_t));
				wcscpy(item_classname,classname);
			}
			else {
				item_classname=NULL;
			}
			//Only store item if it's not empty
			if (item_title != NULL || item_classname != NULL) {
				(*add_blacklist)[*add_numblacklist].title=item_title;
				(*add_blacklist)[*add_numblacklist].classname=item_classname;
				(*add_numblacklist)++;
			}
			//Switch gears to blacklist_sticky?
			if (pos == NULL && *add_blacklist == blacklist) {
				add_blacklist=&blacklist_sticky;
				add_numblacklist=&numblacklist_sticky;
				blacklist_alloc=0;
				GetPrivateProfileString(L"AltDrag",L"Blacklist_Sticky",L"",txt,sizeof(txt)/sizeof(wchar_t),path);
				pos=txt;
			}
		}
		//Check if OS is WinXP/Server 2003
		OSVERSIONINFO vi;
		vi.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
		GetVersionEx(&vi);
		if (vi.dwMajorVersion == 5 && vi.dwMinorVersion >= 1) {
			//Now we will block the shift key if it is pressed while moving a window, this is to prevent XP from switching keyboard layout
			//We will also set cursorwnd to 99% transparent to make it work in XP
			winxp=1;
		}
		//Cursor
		if (settings.Cursor) {
			cursorwnd=FindWindow(L"AltDrag",NULL);
			cursor[HAND]=LoadImage(NULL, IDC_HAND, IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
			cursor[SIZENWSE]=LoadImage(NULL, IDC_SIZENWSE, IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
			cursor[SIZENESW]=LoadImage(NULL, IDC_SIZENESW, IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
			cursor[SIZENS]=LoadImage(NULL, IDC_SIZENS, IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
			cursor[SIZEWE]=LoadImage(NULL, IDC_SIZEWE, IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
			cursor[SIZEALL]=LoadImage(NULL, IDC_SIZEALL, IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
		}
	}
	else if (reason == DLL_PROCESS_DETACH) {
		int i;
		for (i=0; i < numblacklist; i++) {
			free(blacklist[i].title);
			free(blacklist[i].classname);
		}
		numblacklist=0;
		free(blacklist);
		free(wnds);
	}
	return TRUE;
}
