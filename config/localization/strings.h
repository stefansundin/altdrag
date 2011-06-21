
struct strings {
	wchar_t *code;
	wchar_t *title;
	wchar_t *tab_general;
	wchar_t *tab_input;
	wchar_t *tab_blacklist;
	wchar_t *tab_advanced;
	wchar_t *tab_about;
	wchar_t *general_autofocus;
	wchar_t *general_aero;
	wchar_t *general_inactivescroll;
	wchar_t *general_autosnap;
	wchar_t *general_autosnap0;
	wchar_t *general_autosnap1;
	wchar_t *general_autosnap2;
	wchar_t *general_autosnap3;
	wchar_t *general_language;
	wchar_t *general_submittranslation;
	wchar_t *general_autostart_header;
	wchar_t *general_autostart;
	wchar_t *general_autostart_hide;
	wchar_t *general_autosave;
	wchar_t *input_mouse_header;
	wchar_t *input_mouse_lmb;
	wchar_t *input_mouse_mmb;
	wchar_t *input_mouse_rmb;
	wchar_t *input_mouse_mb4;
	wchar_t *input_mouse_mb5;
	wchar_t *input_mouse_idea;
	wchar_t *input_hotkeys_header;
	wchar_t *input_hotkeys_leftalt;
	wchar_t *input_hotkeys_rightalt;
	wchar_t *input_hotkeys_leftwinkey;
	wchar_t *input_hotkeys_rightwinkey;
	wchar_t *input_hotkeys_leftctrl;
	wchar_t *input_hotkeys_rightctrl;
	wchar_t *input_hotkeys_more;
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
