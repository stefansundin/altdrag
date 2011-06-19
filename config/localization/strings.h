
struct strings {
	wchar_t *code;
	wchar_t *title;
	wchar_t *tab_general;
	wchar_t *tab_input;
	wchar_t *tab_blacklist;
	wchar_t *tab_advanced;
	wchar_t *tab_about;
	wchar_t *general_autofocus;
};

#include "en-US/strings.h"
#include "es-ES/strings.h"
#include "gl-ES/strings.h"

struct {
	wchar_t *code;
	struct strings *strings;
	wchar_t *language;
} languages[] = {
	{L"en-US", &en_US, L"English"},
	{L"es-ES", &es_ES, L"Spanish"},
	{L"gl-ES", &gl_ES, L"Galician"},
	{NULL}
};

struct strings *l10n = &en_US;
