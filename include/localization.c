/*
	Copyright (C) 2013  Stefan Sundin (recover89@gmail.com)

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

size_t wcslen_resolved(wchar_t *str) {
	// Return the length of str, having resolved escape sequences
	wchar_t *ptr;
	int num_escape_sequences = 0;
	for (ptr=str; *ptr != '\0'; ptr++) {
		if (*ptr == '\\' && *(ptr+1) != '\0') {
			ptr++;
			num_escape_sequences++;
		}
	}
	return ptr-str-num_escape_sequences;
}

void wcscpy_resolve(wchar_t *dest, wchar_t *source) {
	// Copy from source to dest, resolving \\n to \n
	for (; *source != '\0'; source++,dest++) {
		if (*source == '\\' && *(source+1) == 'n') {
			*dest = '\n';
			source++;
		}
		else {
			*dest = *source;
		}
	}
	*dest = '\0';
}

void LoadTranslation(wchar_t *ini) {
	wchar_t txt[3000];
	int i;
	for (i=0; i < ARRAY_SIZE(l10n_mapping); i++) {
		// Get pointer to default English string to be used if ini entry doesn't exist
		wchar_t *def = *(wchar_t**) ((void*)&en_US + ((void*)l10n_mapping[i].str - (void*)&l10n_ini));
		GetPrivateProfileString(L"Translation", l10n_mapping[i].name, def, txt, ARRAY_SIZE(txt), ini);
		if (l10n_mapping[i].str == &l10n_ini.about.version) {
			wcscat(txt, L" ");
			wcscat(txt, TEXT(APP_VERSION));
		}
		*l10n_mapping[i].str = realloc(*l10n_mapping[i].str, (wcslen_resolved(txt)+1)*sizeof(wchar_t));
		wcscpy_resolve(*l10n_mapping[i].str, txt);
	}
}

void UpdateLanguage() {
	wchar_t path[MAX_PATH];
	GetModuleFileName(NULL, path, ARRAY_SIZE(path));
	PathRemoveFileSpec(path);
	wcscat(path, L"\\Translation.ini");
	FILE *f = _wfopen(path, L"rb");
	if (f != NULL) {
		fclose(f);
		LoadTranslation(path);
		l10n = &l10n_ini;
		return;
	}

	wchar_t txt[10];
	GetPrivateProfileString(L"General", L"Language", L"en-US", txt, ARRAY_SIZE(txt), inipath);
	int i;
	for (i=0; i < ARRAY_SIZE(languages); i++) {
		if (!wcsicmp(txt,languages[i]->code)) {
			l10n = languages[i];
			break;
		}
	}
}
