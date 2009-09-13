/*
	Error message handling
	Copyright (C) 2009  Stefan Sundin (recover89@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

int showerror = 1;

LRESULT CALLBACK ErrorMsgProc(INT nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HCBT_ACTIVATE) {
		//Edit the caption of the buttons
		SetDlgItemText((HWND)wParam, IDYES, L"Copy error");
		SetDlgItemText((HWND)wParam, IDNO,  L"OK");
	}
	return 0;
}

void Error(wchar_t *func, wchar_t *info, int errorcode, wchar_t *file, int line) {
	if (showerror) {
		//Format message
		wchar_t msg[1000], errormsg[100];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorcode, 0, errormsg, sizeof(errormsg)/sizeof(wchar_t), NULL);
		errormsg[wcslen(errormsg)-2] = '\0'; //Remove that damn newline at the end of the formatted error message
		swprintf(msg, L"%s failed in file %s, line %d.\nError: %s (%d)\n\n%s", func, file, line, errormsg, errorcode, info);
		//Display message
		HHOOK hhk = SetWindowsHookEx(WH_CBT,&ErrorMsgProc,0,GetCurrentThreadId());
		int response = MessageBox(NULL,msg,APP_NAME" Error",MB_ICONERROR|MB_YESNO|MB_DEFBUTTON2);
		UnhookWindowsHookEx(hhk);
		if (response == IDYES) {
			//Copy message to clipboard
			OpenClipboard(NULL);
			EmptyClipboard();
			wchar_t *data = LocalAlloc(LMEM_FIXED,(wcslen(msg)+1)*sizeof(wchar_t));
			memcpy(data, msg, (wcslen(msg)+1)*sizeof(wchar_t));
			SetClipboardData(CF_UNICODETEXT, data);
			CloseClipboard();
		}
	}
}
