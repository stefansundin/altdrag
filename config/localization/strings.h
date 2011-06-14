
struct strings {
	wchar_t *code;
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
