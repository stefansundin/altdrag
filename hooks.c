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

#define _WIN32_WINNT 0x0403
#include <windows.h>

static int alt=0;
static int move=0;
static HWND hwnd=NULL;
static POINT offset;

static HINSTANCE hinstDLL;
static HHOOK mousehook;
static int hook_installed=0;

static char msg[100];

_declspec(dllexport) LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		int vkey=((PKBDLLHOOKSTRUCT)lParam)->vkCode;
		
		if (vkey == VK_MENU || vkey == VK_LMENU || vkey == VK_RMENU) {
			if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
				alt=1;
				InstallHook();
			}
			else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
				alt=0;
				if (!move) {
					RemoveHook();
				}
			}
		}
	}
	
    return CallNextHookEx(NULL, nCode, wParam, lParam); 
}

_declspec(dllexport) LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		POINT pt=((PMSLLHOOKSTRUCT)lParam)->pt;
		if (!move && wParam == WM_LBUTTONDOWN && alt) {
			//Double check that Alt is pressed
			if (!(GetAsyncKeyState(VK_MENU)&0x8000)) {
				alt=0;
				RemoveHook();
			}
			
			//Alt is still being pressed
			if (alt) {
				//Get window
				if ((hwnd=WindowFromPoint(pt)) == NULL) {
					sprintf(msg,"WindowFromPoint() failed in file %s, line %d.",__FILE__,__LINE__);
					MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
				}
				hwnd=GetAncestor(hwnd,GA_ROOT);
				
				//Get window and desktop size
				RECT window;
				if (GetWindowRect(hwnd,&window) == 0) {
					sprintf(msg,"GetClientRect() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
					MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
				}
				RECT desktop;
				if (GetWindowRect(GetDesktopWindow(),&desktop) == 0) {
					sprintf(msg,"GetClientRect() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
					MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
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
							sprintf(msg,"GetClientRect() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
							MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
						}
						
						//Set offset
						offset.x=(float)(pt.x-window.left)/(window.right-window.left)*(newwindow.right-newwindow.left);
						offset.y=(float)(pt.y-window.top)/(window.bottom-window.top)*(newwindow.bottom-newwindow.top);
						
						//Move
						if (MoveWindow(hwnd,pt.x-offset.x,pt.y-offset.y,newwindow.right-newwindow.left,newwindow.bottom-newwindow.top,TRUE) == 0) {
							sprintf(msg,"MoveWindow() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
							MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
						}
					}
					else {
						//Set offset
						offset.x=pt.x-window.left;
						offset.y=pt.y-window.top;
					}
					//Ready to move window
					move=1;
					//Prevent mousedown from propagating
					return 1;
				}
			}
		}
		else if (wParam == WM_LBUTTONUP && move) {
			move=0;
			hwnd=NULL;
			RemoveHook();
			//Prevent mouseup from propagating
			return 1;
		}
		
		//Move window
		if (wParam == WM_MOUSEMOVE && move) {
			if (IsWindow(hwnd)) {
				//Get window size
				RECT window;
				if (GetWindowRect(hwnd,&window) == 0) {
					sprintf(msg,"GetClientRect() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
					MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
				}
				
				//Move
				if (MoveWindow(hwnd,pt.x-offset.x,pt.y-offset.y,window.right-window.left,window.bottom-window.top,TRUE) == 0) {
					sprintf(msg,"MoveWindow() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
					MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
				}
			}
			else {
				move=0;
				hwnd=NULL;
				RemoveHook();
			}
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
		sprintf(msg,"SetWindowsHookEx() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
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
		sprintf(msg,"UnhookWindowsHookEx() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
		MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
		return 1;
	}
	
	//Success
	hook_installed=0;
	return 0;
}

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		hinstDLL=hInstance;
	}
	return TRUE;
}
