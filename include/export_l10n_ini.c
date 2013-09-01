/*
	Read/write to ini files.
	Used to batch-write the Language setting when using 'build all'.

	Copyright (C) 2012  Stefan Sundin (recover89@gmail.com)

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

#define UNICODE
#define _UNICODE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <shlwapi.h>

#define APP_NAME            L"AltDrag"
#define APP_VERSION         "1.0b3"

#include "../localization/strings.h"

void wcscpy_escape(wchar_t *dest, wchar_t *source) {
	//Copy from source to dest, resolving \\n to \n
	for (; *source != '\0'; source++,dest++) {
		if (*source == '\n') {
			*dest = '\\';
			dest++;
			*dest = 'n';
			source++;
		}
		else {
			*dest = *source;
		}
	}
	*dest = '\0';
}

int main(int argc, char *argv[]) {
	int i,j;
	for (i=0; i < ARRAY_SIZE(languages); i++) {
		l10n = languages[i];

		wchar_t ini[MAX_PATH];
		GetModuleFileName(NULL, ini, ARRAY_SIZE(ini));
		PathRemoveFileSpec(ini);
		wcscat(ini, L"\\");
		wcscat(ini, l10n->code);
		wcscat(ini, L"\\Translation.ini");

		FILE *f = _wfopen(ini, L"wb");
		fwprintf(f, L"; Translation file for AltDrag 1.0\n\
; %s localization by %s\n\
; Please read the wiki for help: https://code.google.com/p/altdrag/wiki/Translate\n\
; Use encoding UTF-16LE with BOM to be able to use Unicode\n\
; Note: For some reason &A will not work as a shortcut.\n\
\n\
", l10n->code, l10n->author);
		fclose(f);

		for (j=0; j < ARRAY_SIZE(l10n_mapping); j++) {
			wchar_t txt[3000];
			//Get pointer to string
			wchar_t *str = *(wchar_t**) ((void*)l10n + ((void*)l10n_mapping[j].str - (void*)&l10n_ini));
			//Escape
			wcscpy_escape(txt, str);
			//Write
			WritePrivateProfileString(L"Translation", l10n_mapping[j].name, txt, ini);
		}
	}

	return 0;
}
