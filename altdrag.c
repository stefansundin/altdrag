/*
  Copyright (C) 2015  Stefan Sundin

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
*/

#define UNICODE
#define _UNICODE
#define _WIN32_WINNT 0x0600
#define _WIN32_IE 0x0600

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <shlwapi.h>

// App
#define APP_NAME            L"AltDrag"
#define APP_VERSION         "1.1"
#define APP_URL             L"https://stefansundin.github.io/altdrag/"

// Messages
#define WM_TRAY                WM_USER+1
#define SWM_TOGGLE             WM_APP+1
#define SWM_HIDE               WM_APP+2
#define SWM_UPDATE             WM_APP+3
#define SWM_CONFIG             WM_APP+4
#define SWM_ABOUT              WM_APP+5
#define SWM_EXIT               WM_APP+6

// Boring stuff
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define ENABLED() (keyhook || msghook)
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hinst = NULL;
HWND g_hwnd = NULL;
UINT WM_TASKBARCREATED = 0;
UINT WM_UPDATESETTINGS = 0;
UINT WM_ADDTRAY = 0;
UINT WM_HIDETRAY = 0;
UINT WM_OPENCONFIG = 0;
UINT WM_CLOSECONFIG = 0;
wchar_t inipath[MAX_PATH];
int HookSystem();
int HookMouse();

// Cool stuff
HINSTANCE hinstDLL = NULL;
HHOOK keyhook = NULL;
HHOOK msghook = NULL;
BOOL x64 = FALSE;
int vista = 0;
int elevated = 0;

// Include stuff
#include "localization/strings.h"
#include "localization/languages.h"
#include "include/error.c"
#include "include/localization.c"
#include "include/languages.c"
#include "include/tray.c"
#include "include/update.c"
#include "config/config.c"

// Entry point
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, char *szCmdLine, int iCmdShow) {
  g_hinst = hInst;
  IsWow64Process(GetCurrentProcess(), &x64);

  // Get ini path
  GetModuleFileName(NULL, inipath, ARRAY_SIZE(inipath));
  PathRemoveFileSpec(inipath);
  wcscat(inipath, L"\\"APP_NAME".ini");
  wchar_t txt[10];

  // Convert szCmdLine to argv and argc (max 10 arguments)
  char *argv[10];
  int argc = 1;
  argv[0] = szCmdLine;
  while ((argv[argc]=strchr(argv[argc-1],' ')) != NULL) {
    *argv[argc] = '\0';
    if (argc == ARRAY_SIZE(argv)) break;
    argv[argc++]++;
  }

  // Check arguments
  int i;
  int elevate=0, quiet=0, config=-1, multi=0;
  for (i=0; i < argc; i++) {
    if (!strcmp(argv[i],"-hide") || !strcmp(argv[i],"-h")) {
      // -hide = do not add tray icon, hide it if already running
      hide = 1;
    }
    else if (!strcmp(argv[i],"-quiet") || !strcmp(argv[i],"-q")) {
      // -quiet = do nothing if already running
      quiet = 1;
    }
    else if (!strcmp(argv[i],"-elevate") || !strcmp(argv[i],"-e")) {
      // -elevate = create a new instance with administrator privileges
      elevate = 1;
    }
    else if (!strcmp(argv[i],"-config") || !strcmp(argv[i],"-c")) {
      // -config = open config (with requested page)
      config = (i+1 < argc)?atoi(argv[i+1]):0;
    }
    else if (!strcmp(argv[i],"-multi")) {
      // -multi = allow multiple instances, used internally when elevating via config window
      multi = 1;
    }
  }

  // Check if elevated if in >= Vista
  OSVERSIONINFO vi = { sizeof(OSVERSIONINFO) };
  GetVersionEx(&vi);
  vista = (vi.dwMajorVersion >= 6);
  if (vista) {
    HANDLE token;
    TOKEN_ELEVATION elevation;
    DWORD len;
    if (OpenProcessToken(GetCurrentProcess(),TOKEN_READ,&token) && GetTokenInformation(token,TokenElevation,&elevation,sizeof(elevation),&len)) {
      elevated = elevation.TokenIsElevated;
    }
  }

  // Register some messages
  WM_UPDATESETTINGS = RegisterWindowMessage(L"UpdateSettings");
  WM_OPENCONFIG = RegisterWindowMessage(L"OpenConfig");
  WM_CLOSECONFIG = RegisterWindowMessage(L"CloseConfig");
  WM_ADDTRAY = RegisterWindowMessage(L"AddTray");
  WM_HIDETRAY = RegisterWindowMessage(L"HideTray");

  // Look for previous instance
  GetPrivateProfileString(L"Advanced", L"MultipleInstances", L"0", txt, ARRAY_SIZE(txt), inipath);
  if (!_wtoi(txt) && !multi) {
    HWND previnst = FindWindow(APP_NAME, NULL);
    if (previnst != NULL) {
      if (quiet) {
        return 0;
      }
      PostMessage(previnst, WM_UPDATESETTINGS, 0, 0);
      PostMessage(previnst, (hide && !config?WM_CLOSECONFIG:WM_OPENCONFIG), config, 0);
      PostMessage(previnst, (hide?WM_HIDETRAY:WM_ADDTRAY), 0, 0);
      return 0;
    }
  }

  // Check AlwaysElevate
  if (!elevated) {
    GetPrivateProfileString(L"Advanced", L"AlwaysElevate", L"0", txt, ARRAY_SIZE(txt), inipath);
    if (_wtoi(txt)) {
      elevate = 1;
    }

    // Handle request to elevate to administrator privileges
    if (elevate) {
      wchar_t path[MAX_PATH];
      GetModuleFileName(NULL, path, ARRAY_SIZE(path));
      int ret = (INT_PTR) ShellExecute(NULL, L"runas", path, (hide?L"-hide":NULL), NULL, SW_SHOWNORMAL);
      if (ret > 32) {
        return 0;
      }
    }
  }

  // Language
  memset(&l10n_ini, 0, sizeof(l10n_ini));
  UpdateLanguage();

  // Create window
  WNDCLASSEX wnd = { sizeof(WNDCLASSEX), 0, WindowProc, 0, 0, hInst, NULL, NULL, (HBRUSH)(COLOR_WINDOW+1), NULL, APP_NAME, NULL };
  RegisterClassEx(&wnd);
  g_hwnd = CreateWindowEx(WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_TRANSPARENT, wnd.lpszClassName, NULL, WS_POPUP, 0, 0, 0, 0, NULL, NULL, hInst, NULL);

  // Tray icon
  InitTray();
  UpdateTray();

  // Hook system
  HookSystem();

  // Add tray if hook failed, even though -hide was supplied
  if (hide && !keyhook) {
    hide = 0;
    UpdateTray();
  }

  // Check for update
  GetPrivateProfileString(L"Update", L"CheckOnStartup", L"0", txt, ARRAY_SIZE(txt), inipath);
  if (_wtoi(txt)) {
    CheckForUpdate(0);
  }

  // Open config if -config was supplied
  if (config != -1) {
    PostMessage(g_hwnd, WM_OPENCONFIG, config, 0);
  }

  // Message loop
  MSG msg;
  while (GetMessage(&msg,NULL,0,0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}

int HookSystem() {
  if (keyhook && msghook) {
    // System already hooked
    return 1;
  }

  // Load library
  if (!hinstDLL) {
    wchar_t path[MAX_PATH];
    GetModuleFileName(NULL, path, ARRAY_SIZE(path));
    PathRemoveFileSpec(path);
    wcscat(path, L"\\hooks.dll");
    hinstDLL = LoadLibrary(path);
    if (hinstDLL == NULL) {
      Error(L"LoadLibrary('hooks.dll')", L"This probably means that the file hooks.dll is missing. You can try reinstalling "APP_NAME".", GetLastError());
      return 1;
    }
  }

  // Load keyboard hook
  HOOKPROC procaddr;
  if (!keyhook) {
    // Get address to keyboard hook (beware name mangling)
    procaddr = (HOOKPROC) GetProcAddress(hinstDLL, "LowLevelKeyboardProc@12");
    if (procaddr == NULL) {
      Error(L"GetProcAddress('LowLevelKeyboardProc@12')", L"This probably means that the file hooks.dll is from an old version or corrupt. You can try reinstalling "APP_NAME".", GetLastError());
      return 1;
    }
    // Set up the keyboard hook
    keyhook = SetWindowsHookEx(WH_KEYBOARD_LL, procaddr, hinstDLL, 0);
    if (keyhook == NULL) {
      Error(L"SetWindowsHookEx(WH_KEYBOARD_LL)", L"Could not hook keyboard. Another program might be interfering.", GetLastError());
      return 1;
    }
  }

  // HookWindows
  wchar_t txt[10];
  GetPrivateProfileString(L"Advanced", L"HookWindows", L"0", txt, ARRAY_SIZE(txt), inipath);
  if (!msghook && _wtoi(txt)) {
    // Get address to message hook (beware name mangling)
    procaddr = (HOOKPROC) GetProcAddress(hinstDLL, "CallWndProc@12");
    if (procaddr == NULL) {
      Error(L"GetProcAddress('CallWndProc@12')", L"This probably means that the file hooks.dll is from an old version or corrupt. You can try reinstalling "APP_NAME".", GetLastError());
      return 1;
    }
    // Set up the message hook
    msghook = SetWindowsHookEx(WH_CALLWNDPROC, procaddr, hinstDLL, 0);
    if (msghook == NULL) {
      Error(L"SetWindowsHookEx(WH_CALLWNDPROC)", L"Could not hook message hook. Another program might be interfering.", GetLastError());
      return 1;
    }

    // x64
    if (x64) {
      wchar_t path[MAX_PATH];
      GetModuleFileName(NULL, path, ARRAY_SIZE(path));
      PathRemoveFileSpec(path);
      wcscat(path, L"\\HookWindows_x64.exe");
      ShellExecute(NULL, L"open", path, L"nowarning", NULL, SW_SHOWNORMAL);
    }
  }

  // Success
  UpdateTray();
  return 0;
}

// Force processes to unload hooks.dll by sending them a dummy message (HookWindows)
// To be honest I don't really know if this makes a difference anymore
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
  PostMessage(hwnd, WM_NULL, 0, 0);
  return TRUE;
}

int UnhookSystem() {
  if (!keyhook && !msghook) {
    // System not hooked
    return 1;
  }

  // Remove keyboard hook
  if (keyhook) {
    if (UnhookWindowsHookEx(keyhook) == 0) {
      #ifdef DEBUG
      Error(L"UnhookWindowsHookEx(keyhook)", L"Could not unhook keyboard. Try restarting "APP_NAME".", GetLastError());
      #else
      if (showerror) {
        MessageBox(NULL, l10n->unhook_error, APP_NAME, MB_ICONINFORMATION|MB_OK|MB_TOPMOST|MB_SETFOREGROUND);
      }
      #endif
    }
    keyhook = NULL;
  }

  // Remove message hook
  if (msghook) {
    if (UnhookWindowsHookEx(msghook) == 0) {
      #ifdef DEBUG
      Error(L"UnhookWindowsHookEx(msghook)", L"Could not unhook message hook. Try restarting "APP_NAME".", GetLastError());
      #endif
    }
    msghook = NULL;

    // Close HookWindows_x64.exe
    if (x64) {
      HWND window = FindWindow(L"AltDrag-x64", NULL);
      if (window != NULL) {
        PostMessage(window, WM_CLOSE, 0, 0);
      }
    }

    // Send dummy messages to all processes to make them unload hooks.dll
    EnumWindows(EnumWindowsProc, 0);
  }

  // Tell dll file that we are unloading
  void (*Unload)() = (void*) GetProcAddress(hinstDLL, "Unload");
  if (Unload == NULL) {
    Error(L"GetProcAddress('Unload')", L"This probably means that the file hooks.dll is from an old version or corrupt. You can try reinstalling "APP_NAME".", GetLastError());
  }
  else {
    Unload();
  }

  // Unload library
  if (hinstDLL) {
    if (FreeLibrary(hinstDLL) == 0) {
      Error(L"FreeLibrary()", L"Could not free hooks.dll. Try restarting "APP_NAME".", GetLastError());
    }
    hinstDLL = NULL;
  }

  // Success
  UpdateTray();
  return 0;
}

void ToggleState() {
  if (ENABLED()) {
    UnhookSystem();
  }
  else {
    SendMessage(g_hwnd, WM_UPDATESETTINGS, 0, 0);
    HookSystem();
  }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  if (msg == WM_TRAY) {
    if (lParam == WM_LBUTTONDOWN || lParam == WM_LBUTTONDBLCLK) {
      ToggleState();
      if (lParam == WM_LBUTTONDBLCLK && !(GetAsyncKeyState(VK_SHIFT)&0x8000)) {
        SendMessage(hwnd, WM_OPENCONFIG, 0, 0);
      }
    }
    else if (lParam == WM_MBUTTONDOWN) {
      ShellExecute(NULL, L"open", inipath, NULL, NULL, SW_SHOWNORMAL);
    }
    else if (lParam == WM_RBUTTONUP) {
      ShowContextMenu(hwnd);
    }
    else if (lParam == NIN_BALLOONUSERCLICK) {
      hide = 0;
      SendMessage(hwnd, WM_COMMAND, SWM_UPDATE, 0);
    }
    else if (lParam == NIN_BALLOONTIMEOUT) {
      if (hide) {
        RemoveTray();
      }
    }
  }
  else if (msg == WM_UPDATESETTINGS) {
    UpdateLanguage();
    // Reload hooks
    if (ENABLED()) {
      UnhookSystem();
      HookSystem();
    }
    // Reload config language
    if (!wParam && IsWindow(g_cfgwnd)) {
      SendMessage(g_cfgwnd, WM_UPDATESETTINGS, 0, 0);
    }
  }
  else if (msg == WM_ADDTRAY) {
    hide = 0;
    UpdateTray();
  }
  else if (msg == WM_HIDETRAY) {
    hide = 1;
    RemoveTray();
  }
  else if (msg == WM_OPENCONFIG && (lParam || !hide)) {
    OpenConfig(wParam);
  }
  else if (msg == WM_CLOSECONFIG) {
    CloseConfig();
  }
  else if (msg == WM_TASKBARCREATED) {
    tray_added = 0;
    UpdateTray();
  }
  else if (msg == WM_COMMAND) {
    int wmId=LOWORD(wParam), wmEvent=HIWORD(wParam);
    if (wmId == SWM_TOGGLE) {
      ToggleState();
    }
    else if (wmId == SWM_HIDE) {
      hide = 1;
      RemoveTray();
    }
    else if (wmId == SWM_UPDATE) {
      if (MessageBox(NULL,l10n->update_dialog,APP_NAME,MB_ICONINFORMATION|MB_YESNO|MB_TOPMOST|MB_SETFOREGROUND) == IDYES) {
        OpenUrl(APP_URL);
      }
    }
    else if (wmId == SWM_CONFIG) {
      SendMessage(hwnd, WM_OPENCONFIG, 0, 0);
    }
    else if (wmId == SWM_ABOUT) {
      SendMessage(hwnd, WM_OPENCONFIG, 4, 0);
    }
    else if (wmId == SWM_EXIT) {
      DestroyWindow(hwnd);
    }
  }
  else if (msg == WM_QUERYENDSESSION && msghook) {
    showerror = 0;
    UnhookSystem();
  }
  else if (msg == WM_DESTROY) {
    showerror = 0;
    UnhookSystem();
    RemoveTray();
    PostQuitMessage(0);
  }
  else if (msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN) {
    // Hide cursorwnd if clicked on, this might happen if it wasn't hidden by hooks.c for some reason
    ShowWindow(hwnd, SW_HIDE);
  }
  else if (wParam && (msg == WM_PAINT || msg == WM_ERASEBKGND)){
      return 0; // Do nothing when asked to paint!
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}
