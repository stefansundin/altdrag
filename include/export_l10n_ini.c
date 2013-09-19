/*
	Copyright (C) 2013  Stefan Sundin (recover89@gmail.com)

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
#define APP_VERSION         "1.0"
#define HELP_URL            "https://code.google.com/p/altdrag/wiki/Translate"

#include "../localization/strings.h"

void wcscpy_escape(wchar_t *dest, wchar_t *source) {
	// Copy from source to dest, escaping \n to \\n
	for (; *source != '\0'; source++,dest++) {
		if (*source == '\n') {
			*dest = '\\';
			dest++;
			*dest = 'n';
		}
		else {
			*dest = *source;
		}
	}
	*dest = '\0';
}

int main(int argc, char *argv[]) {
	char utf16le_bom[2] = { 0xFF, 0xFE };
	int i,j;
	for (i=0; i < ARRAY_SIZE(languages); i++) {
		l10n = languages[i];
		if (l10n == &en_US) {
			continue;
		}

		wchar_t ini[MAX_PATH];
		GetModuleFileName(NULL, ini, ARRAY_SIZE(ini));
		PathRemoveFileSpec(ini);
		wcscat(ini, L"\\");
		wcscat(ini, l10n->code);
		wcscat(ini, L"\\Translation.ini");

		FILE *f = _wfopen(ini, L"wb");
		fwrite(utf16le_bom, 1, sizeof(utf16le_bom), f); // Write BOM
		fwprintf(f, L"; Translation file for "APP_NAME" "APP_VERSION"\n\
; %s localization by %s\n\
; Simply put this file in the same directory as "APP_NAME", then restart "APP_NAME".\n\
; Please read the wiki for help: "HELP_URL"\n\
; Use encoding UTF-16LE with BOM to be able to use Unicode\n\
\n\
", l10n->code, l10n->author);
		fclose(f);

		for (j=0; j < ARRAY_SIZE(l10n_mapping); j++) {
			wchar_t txt[3000];
			// Get pointer to string
			wchar_t *str = *(wchar_t**) ((void*)l10n + ((void*)l10n_mapping[j].str - (void*)&l10n_ini));
			// Escape
			wcscpy_escape(txt, str);
			// Remove version number from about.version
			if (l10n_mapping[j].str == &l10n_ini.about.version) {
				txt[wcslen(txt)-strlen(APP_VERSION)] = '\0';
				if (txt[wcslen(txt)-1] == ' ') {
					txt[wcslen(txt)-1] = '\0';
				}
			}
			// Write
			WritePrivateProfileString(L"Translation", l10n_mapping[j].name, txt, ini);
		}
	}

	return 0;
}
