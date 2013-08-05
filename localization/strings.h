
struct strings {
	wchar_t *code;
	wchar_t *lang_english;
	wchar_t *lang;
	wchar_t *author;

	/* tray */
	wchar_t *tray_enabled;
	wchar_t *tray_disabled;
	struct {
		wchar_t *enable;
		wchar_t *disable;
		wchar_t *hide;
		wchar_t *update;
		wchar_t *config;
		wchar_t *about;
		wchar_t *exit;
	} menu;

	/* update */
	struct {
		wchar_t *balloon;
		wchar_t *dialog;
		wchar_t *nonew;
	} update;

	/* config */
	wchar_t *title;
	struct {
		wchar_t *general;
		wchar_t *input;
		wchar_t *blacklist;
		wchar_t *advanced;
		wchar_t *about;
	} tabs;
	struct {
		wchar_t *box;
		wchar_t *autofocus;
		wchar_t *aero;
		wchar_t *inactivescroll;
		wchar_t *autosnap;
		wchar_t *autosnap0;
		wchar_t *autosnap1;
		wchar_t *autosnap2;
		wchar_t *autosnap3;
		wchar_t *language;
		wchar_t *autostart_box;
		wchar_t *autostart;
		wchar_t *autostart_hide;
		wchar_t *autostart_elevate;
		wchar_t *autostart_elevate_tip;
		wchar_t *elevate;
		wchar_t *elevated;
		wchar_t *elevate_tip;
		wchar_t *elevation_aborted;
		wchar_t *autosave;
	} general;
	struct {
		struct {
			wchar_t *box;
			wchar_t *lmb;
			wchar_t *mmb;
			wchar_t *rmb;
			wchar_t *mb4;
			wchar_t *mb5;
			wchar_t *scroll;
			wchar_t *more;
		} mouse;
		struct {
			wchar_t *move;
			wchar_t *resize;
			wchar_t *close;
			wchar_t *minimize;
			wchar_t *lower;
			wchar_t *alwaysontop;
			wchar_t *center;
			wchar_t *nothing;
			wchar_t *alttab;
			wchar_t *volume;
			wchar_t *transparency;
		} actions;
		struct {
			wchar_t *box;
			wchar_t *leftalt;
			wchar_t *rightalt;
			wchar_t *leftwinkey;
			wchar_t *rightwinkey;
			wchar_t *leftctrl;
			wchar_t *rightctrl;
			wchar_t *more;
		} hotkeys;
	} input;
	struct {
		wchar_t *box;
		wchar_t *processblacklist;
		wchar_t *blacklist;
		wchar_t *snaplist;
		wchar_t *explanation;
		wchar_t *findwindow_box;
		wchar_t *findwindow_explanation;
	} blacklist;
	struct {
		wchar_t *box;
		wchar_t *hookwindows;
		wchar_t *hookwindows_warn;
		wchar_t *checkonstartup;
		wchar_t *beta;
		wchar_t *checknow;
		wchar_t *ini;
		wchar_t *openini;
	} advanced;
	struct {
		wchar_t *box;
		wchar_t *version;
		wchar_t *author;
		wchar_t *license;
		wchar_t *donate;
		wchar_t *translation_credit;
	} about;
};

#include "en-US/strings.h"
#include "fr-FR/strings.h"
#include "pl-PL/strings.h"
#include "pt-BR/strings.h"
#include "ru-RU/strings.h"
#include "sk-SK/strings.h"
#include "zh-CN/strings.h"
#include "it-IT/strings.h"
#include "de-DE/strings.h"

struct strings *languages[] = {
	&en_US,
	&fr_FR,
	&pl_PL,
	&pt_BR,
	&sk_SK,
	&ru_RU,
	&zh_CN,
	&it_IT,
	&de_DE,
};

struct strings *l10n = &en_US;
