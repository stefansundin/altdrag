
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
		wchar_t *mdi;
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

struct strings l10n_ini;

struct {
	wchar_t **str;
	wchar_t *name;
} l10n_mapping[] = {
	{ &l10n_ini.lang,                                    L"Lang" },
	{ &l10n_ini.lang_english,                            L"LangEnglish" },
	{ &l10n_ini.author,                                  L"Author" },

	{ &l10n_ini.tray_enabled,                            L"TrayEnabled" },
	{ &l10n_ini.tray_disabled,                           L"TrayDisabled" },
	{ &l10n_ini.menu.enable,                             L"MenuEnable" },
	{ &l10n_ini.menu.disable,                            L"MenuDisable" },
	{ &l10n_ini.menu.hide,                               L"MenuHideTray" },
	{ &l10n_ini.menu.update,                             L"MenuUpdateAvailable" },
	{ &l10n_ini.menu.config,                             L"MenuConfigure" },
	{ &l10n_ini.menu.about,                              L"MenuAbout" },
	{ &l10n_ini.menu.exit,                               L"MenuExit" },
	{ &l10n_ini.update.balloon,                          L"UpdateBalloon" },
	{ &l10n_ini.update.dialog,                           L"UpdateDialog" },
	{ &l10n_ini.update.nonew,                            L"UpdateLatest" },

	{ &l10n_ini.tray_disabled,                           L"MenuEnable" },
	{ &l10n_ini.menu.enable,                             L"MenuEnable" },
	{ &l10n_ini.menu.disable,                            L"MenuDisable" },
	{ &l10n_ini.menu.hide,                               L"MenuHideTray" },
	{ &l10n_ini.menu.update,                             L"MenuUpdateAvailable" },
	{ &l10n_ini.menu.config,                             L"MenuConfigure" },
	{ &l10n_ini.menu.about,                              L"MenuAbout" },
	{ &l10n_ini.menu.exit,                               L"MenuExit" },
	{ &l10n_ini.update.balloon,                          L"UpdateBalloon" },
	{ &l10n_ini.update.dialog,                           L"UpdateDialog" },
	{ &l10n_ini.update.nonew,                            L"UpdateLatest" },

	{ &l10n_ini.title,                                   L"ConfigTitle" },
	{ &l10n_ini.tabs.general,                            L"ConfigTabGeneral" },
	{ &l10n_ini.tabs.input,                              L"ConfigTabInput" },
	{ &l10n_ini.tabs.blacklist,                          L"ConfigTabBlacklist" },
	{ &l10n_ini.tabs.advanced,                           L"ConfigTabAdvanced" },
	{ &l10n_ini.tabs.about,                              L"ConfigTabAbout" },

	{ &l10n_ini.general.box,                             L"GeneralBox" },
	{ &l10n_ini.general.autofocus,                       L"GeneralAutoFocus" },
	{ &l10n_ini.general.aero,                            L"GeneralAero" },
	{ &l10n_ini.general.inactivescroll,                  L"GeneralInactiveScroll" },
	{ &l10n_ini.general.mdi,                             L"GeneralMDI" },
	{ &l10n_ini.general.autosnap,                        L"GeneralAutoSnap" },
	{ &l10n_ini.general.autosnap0,                       L"GeneralAutoSnap0" },
	{ &l10n_ini.general.autosnap1,                       L"GeneralAutoSnap1" },
	{ &l10n_ini.general.autosnap2,                       L"GeneralAutoSnap2" },
	{ &l10n_ini.general.autosnap3,                       L"GeneralAutoSnap3" },
	{ &l10n_ini.general.language,                        L"GeneralLanguage" },
	{ &l10n_ini.general.autostart_box,                   L"GeneralAutostartBox" },
	{ &l10n_ini.general.autostart,                       L"GeneralAutostart" },
	{ &l10n_ini.general.autostart_hide,                  L"GeneralAutostartHide" },
	{ &l10n_ini.general.autostart_elevate,               L"GeneralAutostartElevate" },
	{ &l10n_ini.general.autostart_elevate_tip,           L"GeneralAutostartElevateTip" },
	{ &l10n_ini.general.elevate,                         L"GeneralElevate" },
	{ &l10n_ini.general.elevated,                        L"GeneralElevated" },
	{ &l10n_ini.general.elevate_tip,                     L"GeneralElevateTip" },
	{ &l10n_ini.general.elevation_aborted,               L"GeneralElevationAborted" },
	{ &l10n_ini.general.autosave,                        L"GeneralAutosave" },

	{ &l10n_ini.input.mouse.box,                         L"InputMouseBox" },
	{ &l10n_ini.input.mouse.lmb,                         L"InputMouseLMB" },
	{ &l10n_ini.input.mouse.mmb,                         L"InputMouseMMB" },
	{ &l10n_ini.input.mouse.rmb,                         L"InputMouseRMB" },
	{ &l10n_ini.input.mouse.mb4,                         L"InputMouseMB4" },
	{ &l10n_ini.input.mouse.mb5,                         L"InputMouseMB5" },
	{ &l10n_ini.input.mouse.scroll,                      L"InputMouseScroll" },
	{ &l10n_ini.input.mouse.more,                        L"InputMouseMore" },

	{ &l10n_ini.input.actions.move,                      L"InputActionMove" },
	{ &l10n_ini.input.actions.resize,                    L"InputActionResize" },
	{ &l10n_ini.input.actions.close,                     L"InputActionClose" },
	{ &l10n_ini.input.actions.minimize,                  L"InputActionMinimize" },
	{ &l10n_ini.input.actions.lower,                     L"InputActionLower" },
	{ &l10n_ini.input.actions.alwaysontop,               L"InputActionAlwaysOnTop" },
	{ &l10n_ini.input.actions.center,                    L"InputActionCenter" },
	{ &l10n_ini.input.actions.nothing,                   L"InputActionNothing" },
	{ &l10n_ini.input.actions.alttab,                    L"InputActionAltTab" },
	{ &l10n_ini.input.actions.volume,                    L"InputActionVolume" },
	{ &l10n_ini.input.actions.transparency,              L"InputActionTransparency" },

	{ &l10n_ini.input.hotkeys.box,                       L"InputHotkeysBox" },
	{ &l10n_ini.input.hotkeys.leftalt,                   L"InputHotkeysLeftAlt" },
	{ &l10n_ini.input.hotkeys.rightalt,                  L"InputHotkeysRightAlt" },
	{ &l10n_ini.input.hotkeys.leftwinkey,                L"InputHotkeysLeftWinkey" },
	{ &l10n_ini.input.hotkeys.rightwinkey,               L"InputHotkeysRightWinkey" },
	{ &l10n_ini.input.hotkeys.leftctrl,                  L"InputHotkeysLeftCtrl" },
	{ &l10n_ini.input.hotkeys.rightctrl,                 L"InputHotkeysRightCtrl" },
	{ &l10n_ini.input.hotkeys.more,                      L"InputHotkeysMore" },

	{ &l10n_ini.blacklist.box,                           L"BlacklistBox" },
	{ &l10n_ini.blacklist.processblacklist,              L"BlacklistProcessBlacklist" },
	{ &l10n_ini.blacklist.blacklist,                     L"BlacklistBlacklist" },
	{ &l10n_ini.blacklist.snaplist,                      L"BlacklistSnaplist" },
	{ &l10n_ini.blacklist.explanation,                   L"BlacklistExplanation" },
	{ &l10n_ini.blacklist.findwindow_box,                L"BlacklistFindWindowBox" },
	{ &l10n_ini.blacklist.findwindow_explanation,        L"BlacklistFindWindowExplanation" },

	{ &l10n_ini.advanced.box,                            L"AdvancedBox" },
	{ &l10n_ini.advanced.hookwindows,                    L"AdvancedHookWindows" },
	{ &l10n_ini.advanced.hookwindows_warn,               L"AdvancedHookWindowsWarn" },
	{ &l10n_ini.advanced.checkonstartup,                 L"AdvancedUpdateCheckOnStartup" },
	{ &l10n_ini.advanced.beta,                           L"AdvancedUpdateBeta" },
	{ &l10n_ini.advanced.checknow,                       L"AdvancedUpdateCheckNow" },
	{ &l10n_ini.advanced.ini,                            L"AdvancedIni" },
	{ &l10n_ini.advanced.openini,                        L"AdvancedOpenIni" },

	{ &l10n_ini.about.box,                               L"AboutBox" },
	{ &l10n_ini.about.version,                           L"AboutVersion" },
	{ &l10n_ini.about.author,                            L"AboutAuthor" },
	{ &l10n_ini.about.license,                           L"AboutLicense" },
	{ &l10n_ini.about.donate,                            L"AboutDonate" },
	{ &l10n_ini.about.translation_credit,                L"AboutTranslationCredit" },
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
