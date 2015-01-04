/*
  Copyright (C) 2015  Stefan Sundin

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
*/

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
