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
#include <time.h>
#include <windows.h>

static int alt=0;
static int move=0;
static HWND hwnd=NULL;
static POINT offset;

static char msg[100];

_declspec(dllexport) LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		int vkey=((PKBDLLHOOKSTRUCT)lParam)->vkCode;
		
		if (vkey == VK_MENU || vkey == VK_LMENU || vkey == VK_RMENU) {
			if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
				alt=1;
			}
			else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
				alt=0;
				//Prevent keyup from propagating if we're moving a window
				if (move) {
					return 1;
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
			}
			
			//Alt is still being pressed
			if (alt) {
				//Get hwnd
				if ((hwnd=WindowFromPoint(pt)) == NULL) {
					sprintf(msg,"WindowFromPoint() failed in file %s, line %d.",__FILE__,__LINE__);
					MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
				}
				hwnd=GetAncestor(hwnd,GA_ROOT);
				//Restore the window if it's maximized
				if (IsZoomed(hwnd)) {
					//Restore window
					WINDOWPLACEMENT wndpl;
					wndpl.length=sizeof(WINDOWPLACEMENT);
					GetWindowPlacement(hwnd,&wndpl);
					wndpl.showCmd=SW_RESTORE;
					SetWindowPlacement(hwnd,&wndpl);
					
					//Get hwnd pos and size
					RECT rect;
					if (GetWindowRect(hwnd,&rect) == 0) {
						sprintf(msg,"GetClientRect() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
						MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
					}
					
					//Set offset
					offset.x=(rect.right-rect.left)/2;
					offset.y=(rect.bottom-rect.top)/2;
					
					//Move
					if (MoveWindow(hwnd,pt.x-offset.x,pt.y-offset.y,rect.right-rect.left,rect.bottom-rect.top,TRUE) == 0) {
						sprintf(msg,"MoveWindow() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
						MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
					}
				}
				else {
					//Get hwnd pos and size
					RECT rect;
					if (GetWindowRect(hwnd,&rect) == 0) {
						sprintf(msg,"GetClientRect() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
						MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
					}
					//Set offset
					offset.x=pt.x-rect.left;
					offset.y=pt.y-rect.top;
				}
				//Ready to move hwnd
				move=1;
				//Prevent mousepress from propagating
				return 1;
			}
		}
		else if (wParam == WM_LBUTTONUP && move) {
			move=0;
			hwnd=NULL;
		}
		
		//Move window
		if (wParam == WM_MOUSEMOVE && move) {
			//Get hwnd pos and size
			RECT rect;
			if (GetWindowRect(hwnd,&rect) == 0) {
				sprintf(msg,"GetClientRect() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
				MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
			}
			
			//Move
			if (MoveWindow(hwnd,pt.x-offset.x,pt.y-offset.y,rect.right-rect.left,rect.bottom-rect.top,TRUE) == 0) {
				sprintf(msg,"MoveWindow() failed (error code: %d) in file %s, line %d.",GetLastError(),__FILE__,__LINE__);
				MessageBox(NULL, msg, "AltDrag Warning", MB_ICONWARNING|MB_OK);
			}
		}
	}
	
	return CallNextHookEx(NULL, nCode, wParam, lParam); 
}

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD reason, LPVOID reserved) {
	return TRUE;
}
