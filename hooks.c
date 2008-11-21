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

//#define DEBUG

//shift, move and hwnd must be shared since CallWndProc is called in the context of another thread
static int alt=0;
static int shift __attribute__((section ("shared"), shared)) = 0;
static int move __attribute__((section ("shared"), shared)) = 0;
static int resize __attribute__((section ("shared"), shared)) = 0;
static HWND hwnd __attribute__((section ("shared"), shared)) = NULL;
static POINT offset;
static POINT resize_offset;
static HWND *wnds=NULL;
static int numwnds=0;
static int maxwnds=0;

//Cursor data
static HWND cursorwnd=NULL;
static HCURSOR cursorhand=NULL;
static HCURSOR cursorsize=NULL;

//Mousehook data
static HINSTANCE hinstDLL;
static HHOOK mousehook;
static int hook_installed=0;

static char txt[100];

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	//Make sure we have enough space allocated
	if (numwnds == maxwnds) {
		if ((wnds=realloc(wnds,(maxwnds+100)*sizeof(HWND))) == NULL) {
			#ifdef DEBUG
			sprintf(txt,"realloc() failed in file %s, line %d.",__FILE__,__LINE__);
			MessageBox(NULL, txt, "AltDrag Warning", MB_ICONWARNING|MB_OK);
			#endif
			return FALSE;
		}
		maxwnds+=100;
	}
	//Only store window if it's visible, not minimized to taskbar, not maximized and not the window we are dragging
	if (IsWindowVisible(hwnd) && !IsIconic(hwnd) && !IsZoomed(hwnd) && hwnd != (HWND)lParam) {
		//We don't want to add certain windows, such as the Vista start orb or cursorwnd
		char title[100];
		char classname[100]; //classname must be at least as long as the longest member in the list blacklist
		GetWindowText(hwnd,title,sizeof(title));
		GetClassName(hwnd,classname,sizeof(classname));
		if (strcmp(title,"Start") && strcmp(classname,"Button") && hwnd != cursorwnd) {
			//This window is not the Vista start orb
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
		RemoveHook();
		return;
	}
	
	//Get window size
	RECT wnd;
	if (GetWindowRect(hwnd,&wnd) == 0) {
		#ifdef DEBUG
		sprintf(txt,"GetWindowRect() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, txt, "AltDrag Warning", MB_ICONWARNING|MB_OK);
		#endif
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
				#ifdef DEBUG
				sprintf(txt,"GetWindowRect() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
				MessageBox(NULL, txt, "AltDrag Warning", MB_ICONWARNING|MB_OK);
				#endif
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
	if (MoveWindow(hwnd,posx,posy,wndwidth,wndheight,FALSE) == 0) {
		#ifdef DEBUG
		sprintf(txt,"MoveWindow() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, txt, "AltDrag Warning", MB_ICONWARNING|MB_OK);
		#endif
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
		RemoveHook();
		return;
	}
	
	//Get window size
	RECT wnd;
	if (GetWindowRect(hwnd,&wnd) == 0) {
		#ifdef DEBUG
		sprintf(txt,"GetWindowRect() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, txt, "AltDrag Warning", MB_ICONWARNING|MB_OK);
		#endif
	}
	
	//Get new size for window
	POINT pt;
	GetCursorPos(&pt);
	int posx=wnd.left;
	int posy=wnd.top;
	int wndwidth=pt.x-wnd.left+resize_offset.x;
	int wndheight=pt.y-wnd.top+resize_offset.y;
	
	//Double check if any of the shift keys are being pressed
	if (shift && !(GetAsyncKeyState(VK_SHIFT)&0x8000)) {
		shift=0;
	}
	
	//Resize
	if (MoveWindow(hwnd,posx,posy,wndwidth,wndheight,TRUE) == 0) {
		#ifdef DEBUG
		sprintf(txt,"MoveWindow() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, txt, "AltDrag Warning", MB_ICONWARNING|MB_OK);
		#endif
	}
}

_declspec(dllexport) LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		int vkey=((PKBDLLHOOKSTRUCT)lParam)->vkCode;
		
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
			if (!alt && vkey == VK_LMENU) {
				alt=1;
				InstallHook();
			}
			else if (!shift && (vkey == VK_LSHIFT || vkey == VK_RSHIFT)) {
				shift=1;
				if (move) {
					MoveWnd();
				}
			}
			else if (move && (vkey == VK_LCONTROL || vkey == VK_RCONTROL)) {
				SetForegroundWindow(hwnd);
			}
		}
		else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
			if (vkey == VK_LMENU) {
				alt=0;
				if (!move && !resize) {
					RemoveHook();
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

_declspec(dllexport) LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		if (wParam == WM_LBUTTONDOWN && alt && !move) {
			//Double check if the left alt key is being pressed
			if (!(GetAsyncKeyState(VK_LMENU)&0x8000)) {
				alt=0;
				RemoveHook();
			}
			else {
				//Alt key is still being pressed
				POINT pt=((PMSLLHOOKSTRUCT)lParam)->pt;
				
				//Make sure cursorwnd isn't in the way
				ShowWindow(cursorwnd,SW_HIDE);
				
				//Get window
				if ((hwnd=WindowFromPoint(pt)) == NULL) {
					#ifdef DEBUG
					sprintf(txt,"WindowFromPoint() failed in file %s, line %d.",__FILE__,__LINE__);
					MessageBox(NULL, txt, "AltDrag Warning", MB_ICONWARNING|MB_OK);
					#endif
				}
				hwnd=GetAncestor(hwnd,GA_ROOT);
				
				//Get the classname so we can check if this window is the alt+tab window
				char classname[100];
				GetClassName(hwnd,classname,sizeof(classname));
				
				//Only continue if this window is not the alt+tab window
				if (strcmp(classname,"TaskSwitcherWnd") && strcmp(classname,"TaskSwitcherOverlayWnd")) {
					//Get window and desktop size
					RECT window;
					if (GetWindowRect(hwnd,&window) == 0) {
						#ifdef DEBUG
						sprintf(txt,"GetWindowRect() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
						MessageBox(NULL, txt, "AltDrag Warning", MB_ICONWARNING|MB_OK);
						#endif
					}
					RECT desktop;
					if (GetWindowRect(GetDesktopWindow(),&desktop) == 0) {
						#ifdef DEBUG
						sprintf(txt,"GetWindowRect() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
						MessageBox(NULL, txt, "AltDrag Warning", MB_ICONWARNING|MB_OK);
						#endif
					}
					
					//Don't move the window if it's fullscreen
					if (window.left != desktop.left || window.top != desktop.top || window.right != desktop.right || window.bottom != desktop.bottom) {
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
								#ifdef DEBUG
								sprintf(txt,"GetWindowRect() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
								MessageBox(NULL, txt, "AltDrag Warning", MB_ICONWARNING|MB_OK);
								#endif
							}
							
							//Set offset
							offset.x=(float)(pt.x-window.left)/(window.right-window.left)*(newwindow.right-newwindow.left);
							offset.y=(float)(pt.y-window.top)/(window.bottom-window.top)*(newwindow.bottom-newwindow.top);
							
							//Move
							MoveWnd();
						}
						else {
							//Set offset
							offset.x=pt.x-window.left;
							offset.y=pt.y-window.top;
						}
						//Show cursorwnd
						SetClassLongPtr(cursorwnd,GCLP_HCURSOR,(LONG_PTR)cursorhand);
						if (!resize) {
							MoveWindow(cursorwnd,desktop.left,desktop.top,desktop.right-desktop.left,desktop.bottom-desktop.top,FALSE);
							SetWindowLongPtr(cursorwnd,GWL_EXSTYLE,WS_EX_LAYERED|WS_EX_TOOLWINDOW); //Workaround for http://support.microsoft.com/kb/270624/
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
		else if (wParam == WM_LBUTTONUP && move) {
			move=0;
			if (!alt) {
				RemoveHook();
			}
			//Hide cursorwnd
			if (resize) {
				SetClassLongPtr(cursorwnd,GCLP_HCURSOR,(LONG_PTR)cursorsize);
			}
			else {
				ShowWindowAsync(cursorwnd,SW_HIDE);
				SetWindowLongPtr(cursorwnd,GWL_EXSTYLE,WS_EX_TOOLWINDOW); //Workaround for http://support.microsoft.com/kb/270624/
			}
			//Prevent mouseup from propagating
			return 1;
		}
		else if (wParam == WM_MBUTTONDOWN && alt && !resize) {
			//Double check if the left alt key is being pressed
			if (!(GetAsyncKeyState(VK_LMENU)&0x8000)) {
				alt=0;
				RemoveHook();
			}
			else {
				//Alt key is still being pressed
				POINT pt=((PMSLLHOOKSTRUCT)lParam)->pt;
				
				//Make sure cursorwnd isn't in the way
				ShowWindow(cursorwnd,SW_HIDE);
				
				//Get window
				if ((hwnd=WindowFromPoint(pt)) == NULL) {
					#ifdef DEBUG
					sprintf(txt,"WindowFromPoint() failed in file %s, line %d.",__FILE__,__LINE__);
					MessageBox(NULL, txt, "AltDrag Warning", MB_ICONWARNING|MB_OK);
					#endif
				}
				hwnd=GetAncestor(hwnd,GA_ROOT);
				
				//Get window and desktop size
				RECT window;
				if (GetWindowRect(hwnd,&window) == 0) {
					#ifdef DEBUG
					sprintf(txt,"GetWindowRect() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
					MessageBox(NULL, txt, "AltDrag Warning", MB_ICONWARNING|MB_OK);
					#endif
				}
				RECT desktop;
				if (GetWindowRect(GetDesktopWindow(),&desktop) == 0) {
					#ifdef DEBUG
					sprintf(txt,"GetWindowRect() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
					MessageBox(NULL, txt, "AltDrag Warning", MB_ICONWARNING|MB_OK);
					#endif
				}
				
				//Don't resize the window if it's fullscreen
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
						if (GetWindowRect(FindWindow("Shell_TrayWnd",NULL),&taskbar)) {
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
					//Set offset
					resize_offset.x=window.right-pt.x;
					resize_offset.y=window.bottom-pt.y;
					//Show cursorwnd
					if (!move) {
						MoveWindow(cursorwnd,desktop.left,desktop.top,desktop.right-desktop.left,desktop.bottom-desktop.top,FALSE);
						SetWindowLongPtr(cursorwnd,GWL_EXSTYLE,WS_EX_LAYERED|WS_EX_TOOLWINDOW); //Workaround for http://support.microsoft.com/kb/270624/
						SetClassLongPtr(cursorwnd,GCLP_HCURSOR,(LONG_PTR)cursorsize);
					}
					ShowWindowAsync(cursorwnd,SW_SHOWNA);
					//Ready to resize window
					resize=1;
					//Prevent mousedown from propagating
					return 1;
				}
			}
		}
		else if (wParam == WM_MBUTTONUP && resize) {
			resize=0;
			if (!alt) {
				RemoveHook();
			}
			//Hide cursorwnd
			if (!move) {
				ShowWindowAsync(cursorwnd,SW_HIDE);
				SetWindowLongPtr(cursorwnd,GWL_EXSTYLE,WS_EX_TOOLWINDOW); //Workaround for http://support.microsoft.com/kb/270624/
			}
			//Prevent mouseup from propagating
			return 1;
		}
		else if (wParam == WM_MOUSEMOVE) {
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
		
		if (msg->message == WM_WINDOWPOSCHANGED && shift && IsWindowVisible(msg->hwnd)) {
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

int InstallHook() {
	if (hook_installed) {
		//Hook already installed
		return 1;
	}
	
	//Set up the mouse hook
	if ((mousehook=SetWindowsHookEx(WH_MOUSE_LL,MouseProc,hinstDLL,0)) == NULL) {
		#ifdef DEBUG
		sprintf(txt,"SetWindowsHookEx() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, txt, "AltDrag Warning", MB_ICONWARNING|MB_OK);
		#endif
		return 1;
	}
	
	//Success
	hook_installed=1;
	return 0;
}

int RemoveHook() {
	if (!hook_installed) {
		//Hook not installed
		return 1;
	}
	
	//Remove mouse hook
	if (UnhookWindowsHookEx(mousehook) == 0) {
		#ifdef DEBUG
		sprintf(txt,"UnhookWindowsHookEx() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, txt, "AltDrag Warning", MB_ICONWARNING|MB_OK);
		#endif
		return 1;
	}
	
	//Success
	hook_installed=0;
	return 0;
}

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		hinstDLL=hInstance;
		cursorwnd=FindWindow("AltDrag",NULL);
		cursorhand=LoadImage(NULL, IDC_HAND, IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
		cursorsize=LoadImage(NULL, IDC_SIZEALL, IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_SHARED);
	}
	else if (reason == DLL_PROCESS_DETACH) {
		free(wnds);
	}
	return TRUE;
}
