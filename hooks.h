/*
	AltDrag - Drag windows with the mouse when pressing the alt key
	Copyright (C) 2009  Stefan Sundin (recover89@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

//Blacklist
int blacklisted(HWND hwnd, struct blacklist *list) {
	wchar_t title[256];
	wchar_t classname[256];
	GetWindowText(hwnd, title, sizeof(title)/sizeof(wchar_t));
	GetClassName(hwnd, classname, sizeof(classname)/sizeof(wchar_t));
	int i;
	for (i=0; i < list->numitems; i++) {
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
		monitors = realloc(monitors,monitors_alloc*sizeof(RECT));
		if (monitors == NULL) {
			Error(L"realloc(monitors)", L"Out of memory?", 0, TEXT(__FILE__), __LINE__);
			return FALSE;
		}
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
		wnds = realloc(wnds,wnds_alloc*sizeof(RECT));
		if (wnds == NULL) {
			Error(L"realloc(wnds)", L"Out of memory?", 0, TEXT(__FILE__), __LINE__);
			return FALSE;
		}
	}
	//Only store window if it's visible, not minimized to taskbar, not the window we are dragging and not blacklisted
	RECT wnd;
	if (window != hwnd && window != progman
	 && IsWindowVisible(window) && !IsIconic(window)
	 && !blacklisted(window,&settings.Blacklist_Sticky)
	 && GetWindowRect(window,&wnd) != 0
	) {
		//Return if the window is in the roll-up database (I want to get used to the roll-ups before deciding if I want this)
		/*for (i=0; i < NUMROLLUP; i++) {
			if (rollup[i].hwnd == window) {
				return TRUE;
			}
		}*/
		//If maximized, then this window covers the whole monitor
		//I do this since the window has an extra, invisible, border when maximized (a border that stretches onto other monitors)
		if (IsZoomed(window)) {
			HMONITOR monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
			MONITORINFO monitorinfo;
			monitorinfo.cbSize = sizeof(MONITORINFO);
			GetMonitorInfo(monitor, &monitorinfo);
			wnd=monitorinfo.rcMonitor;
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
	}
	return TRUE;
}

void Enum() {
	//Update handle to progman
	if (!IsWindow(progman)) {
		progman = FindWindow(L"Progman",L"Program Manager");
	}
	
	//Enumerate monitors
	nummonitors = 0;
	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);
	
	//Enumerate windows
	numwnds = 0;
	if (shift || sharedsettings.AutoStick > 0) {
		HWND taskbar = FindWindow(L"Shell_TrayWnd",NULL);
		RECT wnd;
		if (taskbar != NULL && GetWindowRect(taskbar,&wnd) != 0) {
			wnds[numwnds++] = wnd;
		}
	}
	if (shift || sharedsettings.AutoStick >= 2) {
		EnumWindows(EnumWindowsProc, 0);
	}
	
	//Use this to print the monitors and windows
	/*FILE *f = fopen("C:\\altdrag-log.txt","wb");
	fprintf(f, "nummonitors: %d\n", nummonitors);
	int k;
	for (k=0; k < nummonitors; k++) {
		fprintf(f, "mon #%02d: left %d, top %d, right %d, bottom %d\n", k, monitors[k].left, monitors[k].top, monitors[k].right, monitors[k].bottom);
	}
	fprintf(f, "\n");
	fprintf(f, "numwnds: %d\n", numwnds);
	char title[100];
	char classname[100];
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

int MoveStick(int *posx, int *posy, int wndwidth, int wndheight) {
	//Enumerate monitors and windows
	Enum();
	
	//thresholdx and thresholdy will shrink to make sure the dragged window will stick to the closest windows
	int i, j, thresholdx, thresholdy, stuckx=0, stucky=0, stickx=0, sticky=0;
	thresholdx = thresholdy = STICKY_THRESHOLD;
	//Loop monitors and windows
	for (i=0, j=0; i < nummonitors || j < numwnds; ) {
		RECT stickywnd;
		int stickinside;
		
		//Get stickywnd
		if (i < nummonitors) {
			stickywnd = monitors[i];
			stickinside = 1;
			i++;
		}
		else if (j < numwnds) {
			stickywnd = wnds[j];
			stickinside = (shift || sharedsettings.AutoStick != 2);
			j++;
		}
		
		//Check if posx sticks
		if ((stickywnd.top-thresholdx < *posy && *posy < stickywnd.bottom+thresholdx)
		 || (*posy-thresholdx < stickywnd.top && stickywnd.top < *posy+wndheight+thresholdx)) {
			int stickinsidecond = (stickinside || *posy+wndheight-thresholdx < stickywnd.top || stickywnd.bottom < *posy+thresholdx);
			if (*posx-thresholdx < stickywnd.right && stickywnd.right < *posx+thresholdx) {
				//The left edge of the dragged window will stick to this window's right edge
				stuckx = 1;
				stickx = stickywnd.right;
				thresholdx = stickywnd.right-*posx;
			}
			else if (stickinsidecond && *posx+wndwidth-thresholdx < stickywnd.right && stickywnd.right < *posx+wndwidth+thresholdx) {
				//The right edge of the dragged window will stick to this window's right edge
				stuckx = 1;
				stickx = stickywnd.right-wndwidth;
				thresholdx = stickywnd.right-(*posx+wndwidth);
			}
			else if (stickinsidecond && *posx-thresholdx < stickywnd.left && stickywnd.left < *posx+thresholdx) {
				//The left edge of the dragged window will stick to this window's left edge
				stuckx = 1;
				stickx = stickywnd.left;
				thresholdx = stickywnd.left-*posx;
			}
			else if (*posx+wndwidth-thresholdx < stickywnd.left && stickywnd.left < *posx+wndwidth+thresholdx) {
				//The right edge of the dragged window will stick to this window's left edge
				stuckx = 1;
				stickx = stickywnd.left-wndwidth;
				thresholdx = stickywnd.left-(*posx+wndwidth);
			}
		}
		
		//Check if posy sticks
		if ((stickywnd.left-thresholdy < *posx && *posx < stickywnd.right+thresholdy)
		 || (*posx-thresholdy < stickywnd.left && stickywnd.left < *posx+wndwidth+thresholdy)) {
			int stickinsidecond = (stickinside || *posx+wndwidth-thresholdy < stickywnd.left || stickywnd.right < *posx+thresholdy);
			if (*posy-thresholdy < stickywnd.bottom && stickywnd.bottom < *posy+thresholdy) {
				//The top edge of the dragged window will stick to this window's bottom edge
				stucky = 1;
				sticky = stickywnd.bottom;
				thresholdy = stickywnd.bottom-*posy;
			}
			else if (stickinsidecond && *posy+wndheight-thresholdy < stickywnd.bottom && stickywnd.bottom < *posy+wndheight+thresholdy) {
				//The bottom edge of the dragged window will stick to this window's bottom edge
				stucky = 1;
				sticky = stickywnd.bottom-wndheight;
				thresholdy = stickywnd.bottom-(*posy+wndheight);
			}
			else if (stickinsidecond && *posy-thresholdy < stickywnd.top && stickywnd.top < *posy+thresholdy) {
				//The top edge of the dragged window will stick to this window's top edge
				stucky = 1;
				sticky = stickywnd.top;
				thresholdy = stickywnd.top-*posy;
			}
			else if (*posy+wndheight-thresholdy < stickywnd.top && stickywnd.top < *posy+wndheight+thresholdy) {
				//The bottom edge of the dragged window will stick to this window's top edge
				stucky = 1;
				sticky = stickywnd.top-wndheight;
				thresholdy = stickywnd.top-(*posy+wndheight);
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
	
	//Return
	return (stuckx || stucky);
}

int ResizeStick(int *posx, int *posy, int *wndwidth, int *wndheight) {
	//Enumerate monitors and windows
	Enum();
	
	//thresholdx and thresholdy will shrink to make sure the dragged window will stick to the closest windows
	int i, j, thresholdx, thresholdy, stuckleft=0, stucktop=0, stuckright=0, stuckbottom=0, stickleft=0, sticktop=0, stickright=0, stickbottom=0;
	thresholdx = thresholdy = STICKY_THRESHOLD;
	//Loop monitors and windows
	for (i=0, j=0; i < nummonitors || j < numwnds; ) {
		RECT stickywnd;
		int stickinside;
		
		//Get stickywnd
		if (i < nummonitors) {
			stickywnd = monitors[i];
			stickinside = 1;
			i++;
		}
		else if (j < numwnds) {
			stickywnd = wnds[j];
			stickinside = (shift || sharedsettings.AutoStick != 2);
			j++;
		}
		
		//Check if posx sticks
		if ((stickywnd.top-thresholdx < *posy && *posy < stickywnd.bottom+thresholdx)
		 || (*posy-thresholdx < stickywnd.top && stickywnd.top < *posy+*wndheight+thresholdx)) {
			int stickinsidecond = (stickinside || *posy+*wndheight-thresholdx < stickywnd.top || stickywnd.bottom < *posy+thresholdx);
			if (resize_x == LEFT && *posx-thresholdx < stickywnd.right && stickywnd.right < *posx+thresholdx) {
				//The left edge of the dragged window will stick to this window's right edge
				stuckleft = 1;
				stickleft = stickywnd.right;
				thresholdx = stickywnd.right-*posx;
			}
			else if (stickinsidecond && resize_x == RIGHT && *posx+*wndwidth-thresholdx < stickywnd.right && stickywnd.right < *posx+*wndwidth+thresholdx) {
				//The right edge of the dragged window will stick to this window's right edge
				stuckright = 1;
				stickright = stickywnd.right;
				thresholdx = stickywnd.right-(*posx+*wndwidth);
			}
			else if (stickinsidecond && resize_x == LEFT && *posx-thresholdx < stickywnd.left && stickywnd.left < *posx+thresholdx) {
				//The left edge of the dragged window will stick to this window's left edge
				stuckleft = 1;
				stickleft = stickywnd.left;
				thresholdx = stickywnd.left-*posx;
			}
			else if (resize_x == RIGHT && *posx+*wndwidth-thresholdx < stickywnd.left && stickywnd.left < *posx+*wndwidth+thresholdx) {
				//The right edge of the dragged window will stick to this window's left edge
				stuckright = 1;
				stickright = stickywnd.left;
				thresholdx = stickywnd.left-(*posx+*wndwidth);
			}
		}
		
		//Check if posy sticks
		if ((stickywnd.left-thresholdy < *posx && *posx < stickywnd.right+thresholdy)
		 || (*posx-thresholdy < stickywnd.left && stickywnd.left < *posx+*wndwidth+thresholdy)) {
			int stickinsidecond = (stickinside || *posx+*wndwidth-thresholdy < stickywnd.left || stickywnd.right < *posx+thresholdy);
			if (resize_y == TOP && *posy-thresholdy < stickywnd.bottom && stickywnd.bottom < *posy+thresholdy) {
				//The top edge of the dragged window will stick to this window's bottom edge
				stucktop = 1;
				sticktop = stickywnd.bottom;
				thresholdy = stickywnd.bottom-*posy;
			}
			else if (stickinsidecond && resize_y == BOTTOM && *posy+*wndheight-thresholdy < stickywnd.bottom && stickywnd.bottom < *posy+*wndheight+thresholdy) {
				//The bottom edge of the dragged window will stick to this window's bottom edge
				stuckbottom = 1;
				stickbottom = stickywnd.bottom;
				thresholdy = stickywnd.bottom-(*posy+*wndheight);
			}
			else if (stickinsidecond && resize_y == TOP && *posy-thresholdy < stickywnd.top && stickywnd.top < *posy+thresholdy) {
				//The top edge of the dragged window will stick to this window's top edge
				stucktop = 1;
				sticktop = stickywnd.top;
				thresholdy = stickywnd.top-*posy;
			}
			else if (resize_y == BOTTOM && *posy+*wndheight-thresholdy < stickywnd.top && stickywnd.top < *posy+*wndheight+thresholdy) {
				//The bottom edge of the dragged window will stick to this window's top edge
				stuckbottom = 1;
				stickbottom = stickywnd.top;
				thresholdy = stickywnd.top-(*posy+*wndheight);
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
	
	//Return
	return (stuckleft || stucktop || stuckright || stuckbottom);
}

int HookMouse() {
	if (mousehook) {
		//Mouse already hooked
		return 1;
	}
	
	//Set up the mouse hook
	mousehook = SetWindowsHookEx(WH_MOUSE_LL,LowLevelMouseProc,hinstDLL,0);
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
	
	//Remove mouse hook
	if (UnhookWindowsHookEx(mousehook) == 0) {
		Error(L"UnhookWindowsHookEx(mousehook)", L"", GetLastError(), TEXT(__FILE__), __LINE__);
		mousehook = NULL;
		return 1;
	}
	
	//Success
	mousehook = NULL;
	clicktime = 0;
	return 0;
}

//Msghook
__cdecl __declspec(dllexport) LRESULT CALLBACK CustomWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_WINDOWPOSCHANGING && (shift || sharedsettings.AutoStick)) {
		WINDOWPOS *wndpos = (WINDOWPOS*)lParam;
		if (msgaction == MOVE && !(wndpos->flags&SWP_NOMOVE)) {
			MoveStick(&wndpos->x, &wndpos->y, wndpos->cx, wndpos->cy);
		}
		else if (msgaction == RESIZE && !(wndpos->flags&SWP_NOSIZE)) {
			ResizeStick(&wndpos->x, &wndpos->y, &wndpos->cx, &wndpos->cy);
		}
	}
	
	/* //Fun code to trap window on screen
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
	
	return CallWindowProc(oldwndproc, hwnd, msg, wParam, lParam);
}

//CallWndProc is called in the context of the thread that calls SendMessage, not the thread that receives the message.
//Thus we have to explicitly share the memory we want CallWndProc to be able to access (shift, move, resize and hwnd)
__cdecl __declspec(dllexport) LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION && !move && !resize) {
		CWPSTRUCT *msg = (CWPSTRUCT*)lParam;
		
		if (hwnd != msg->hwnd
		 && !move && !resize
		 && (msg->message == WM_ENTERSIZEMOVE || msg->message == WM_WINDOWPOSCHANGING)
		 && (shift || sharedsettings.AutoStick)
		 && IsWindowVisible(msg->hwnd)
		 && !(GetWindowLongPtr(msg->hwnd,GWL_EXSTYLE)&WS_EX_TOOLWINDOW)
		 && !IsIconic(msg->hwnd) && !IsZoomed(msg->hwnd)
		 && msg->hwnd == GetAncestor(msg->hwnd,GA_ROOT)) {
			//Double check if any of the shift keys are being pressed
			if (shift && !(GetAsyncKeyState(VK_SHIFT)&0x8000)) {
				shift = 0;
				if (!sharedsettings.AutoStick) {
					return CallNextHookEx(NULL, nCode, wParam, lParam);
				}
			}
			
			//Return if window is blacklisted
			if (blacklisted(msg->hwnd,&settings.Blacklist)) {
				return CallNextHookEx(NULL, nCode, wParam, lParam);
			}
			
			//Restore old WndProc if another window has already been subclassed
			if (oldwndproc != NULL && IsWindow(hwnd)) {
				if (SetWindowLongPtr(hwnd,GWLP_WNDPROC,(LONG_PTR)oldwndproc) == 0) {
					Error(L"SetWindowLongPtr(hwnd, GWLP_WNDPROC, oldwndproc)", L"Failed to restore subclassed window to its old wndproc.", GetLastError(), TEXT(__FILE__), __LINE__);
				}
				oldwndproc = NULL;
			}
			
			//Set hwnd
			hwnd = msg->hwnd;
			//Subclass window
			oldwndproc = (WNDPROC)SetWindowLongPtr(hwnd,GWLP_WNDPROC,(LONG_PTR)CustomWndProc);
			if (oldwndproc == 0) {
				//This might fail if the window isn't responding, which happens from now and then, so there's no need to nag about it
				//Error(L"SetWindowLongPtr(hwnd, GWLP_WNDPROC, CustomWndProc)", L"Failed to subclass window.", GetLastError(), TEXT(__FILE__), __LINE__);
			}
		}
		
		if (msg->message == WM_SYSCOMMAND) {
			WPARAM action = msg->wParam&0xFFF0;
			if (action == SC_MOVE) {
				msgaction = MOVE;
			}
			else if (action == SC_SIZE) {
				msgaction = RESIZE;
				int edge = msg->wParam&0x000F; //These are the undocumented bits (compatible with WMSZ_*)
				//Set offset
				//resize_x
				if (edge == WMSZ_TOP || edge == WMSZ_BOTTOM) {
					resize_x = CENTER;
				}
				if (edge == WMSZ_LEFT || edge == WMSZ_TOPLEFT || edge == WMSZ_BOTTOMLEFT) {
					resize_x = LEFT;
				}
				else if (edge == WMSZ_RIGHT || edge == WMSZ_TOPRIGHT || edge == WMSZ_BOTTOMRIGHT) {
					resize_x = RIGHT;
				}
				//resize_y
				if (edge == WMSZ_LEFT || edge == WMSZ_RIGHT) {
					resize_y = CENTER;
				}
				if (edge == WMSZ_TOP || edge == WMSZ_TOPLEFT || edge == WMSZ_TOPRIGHT) {
					resize_y = TOP;
				}
				else if (edge == WMSZ_BOTTOM || edge == WMSZ_BOTTOMLEFT || edge == WMSZ_BOTTOMRIGHT) {
					resize_y = BOTTOM;
				}
				resize_offset.x = 0;
				resize_offset.y = 0;
			}
		}
		
		if ((msg->message == WM_EXITSIZEMOVE || msg->message == WM_DESTROY)
		 && msg->hwnd == hwnd && oldwndproc != NULL) {
			//Restore old WndProc
			if (SetWindowLongPtr(hwnd,GWLP_WNDPROC,(LONG_PTR)oldwndproc) == 0) {
				Error(L"SetWindowLongPtr(hwnd, GWLP_WNDPROC, oldwndproc)", L"Failed to restore subclassed window to its old wndproc.", GetLastError(), TEXT(__FILE__), __LINE__);
			}
			oldwndproc = NULL;
			hwnd = NULL;
		}
	}
	
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

__cdecl __declspec(dllexport) void ClearSettings() {
	sharedsettings_loaded=0;
}
