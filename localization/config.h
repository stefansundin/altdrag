
struct strings {
	wchar_t *code;
	wchar_t *title;
	wchar_t *tab_general;
	wchar_t *tab_input;
	wchar_t *tab_blacklist;
	wchar_t *tab_advanced;
	wchar_t *tab_about;
	wchar_t *general_box;
	wchar_t *general_autofocus;
	wchar_t *general_aero;
	wchar_t *general_inactivescroll;
	wchar_t *general_autosnap;
	wchar_t *general_autosnap0;
	wchar_t *general_autosnap1;
	wchar_t *general_autosnap2;
	wchar_t *general_autosnap3;
	wchar_t *general_language;
	wchar_t *general_helptranslate;
	wchar_t *general_autostart_box;
	wchar_t *general_autostart;
	wchar_t *general_autostart_hide;
	wchar_t *general_autosave;
	wchar_t *input_mouse_box;
	wchar_t *input_mouse_lmb;
	wchar_t *input_mouse_mmb;
	wchar_t *input_mouse_rmb;
	wchar_t *input_mouse_mb4;
	wchar_t *input_mouse_mb5;
	wchar_t *input_mouse_more;
	wchar_t *input_hotkeys_box;
	wchar_t *input_hotkeys_leftalt;
	wchar_t *input_hotkeys_rightalt;
	wchar_t *input_hotkeys_leftwinkey;
	wchar_t *input_hotkeys_rightwinkey;
	wchar_t *input_hotkeys_leftctrl;
	wchar_t *input_hotkeys_rightctrl;
	wchar_t *input_hotkeys_more;
	wchar_t *blacklist_box;
	wchar_t *blacklist_processblacklist;
	wchar_t *blacklist_blacklist;
	wchar_t *blacklist_snaplist;
	wchar_t *blacklist_explanation;
	wchar_t *blacklist_findwindow_box;
	wchar_t *blacklist_findwindow_explanation;
	wchar_t *advanced_box;
	wchar_t *advanced_hookwindows;
	wchar_t *advanced_checkonstartup;
	wchar_t *advanced_beta;
	wchar_t *advanced_ini;
	wchar_t *advanced_openini;
	wchar_t *about_box;
	wchar_t *about_version;
	wchar_t *about_author;
	wchar_t *about_license;
	wchar_t *about_donations_box;
	wchar_t *about_donations_plea;
	wchar_t *about_donate;
};

#include "../localization/en-US/config.h"
#include "../localization/es-ES/config.h"
#include "../localization/gl-ES/config.h"
#include "../localization/zh-CN/config.h"

struct {
	wchar_t *code;
	struct strings *strings;
	wchar_t *language;
} languages[] = {
	{L"en-US", &en_US, L"English"},
	{L"es-ES", &es_ES, L"Spanish"},
	{L"gl-ES", &gl_ES, L"Galician"},
	{L"zh-CN", &zh_CN, L"Chinese"},
	{NULL}
};

struct strings *l10n = &en_US;
