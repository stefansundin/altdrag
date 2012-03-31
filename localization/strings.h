
struct strings {
	/* tray */
	wchar_t *tray_enabled;
	wchar_t *tray_disabled;
	wchar_t *menu_enable;
	wchar_t *menu_disable;
	wchar_t *menu_hide;
	wchar_t *menu_update;
	wchar_t *menu_config;
	wchar_t *menu_about;
	wchar_t *menu_exit;
	
	/* update */
	wchar_t *update_balloon;
	wchar_t *update_dialog;
	wchar_t *update_nonew;
	
	/* config */
	wchar_t *lang;
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
	wchar_t *advanced_checknow;
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

#include "en-US/strings.h"
#include "es-ES/strings.h"
#include "gl-ES/strings.h"
#include "zh-CN/strings.h"
#include "sk-SK/strings.h"
#include "ru-RU/strings.h"
#include "fr-FR/strings.h"
#include "pl-PL/strings.h"
//#include "de-DE/strings.h"
#include "pt-BR/strings.h"

struct {
	wchar_t *code;
	struct strings *strings;
} languages[] = {
	{ L"en-US", &en_US },
	{ L"es-ES", &es_ES },
	{ L"gl-ES", &gl_ES },
	{ L"zh-CN", &zh_CN },
	{ L"sk-SK", &sk_SK },
	{ L"ru-RU", &ru_RU },
	{ L"fr-FR", &fr_FR },
	{ L"pl-PL", &pl_PL },
	//{ L"de-DE", &de_DE },
	{ L"pt-BR", &pt_BR },
	{ NULL }
};

struct strings *l10n = &en_US;
