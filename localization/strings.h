
struct strings {
	wchar_t *code;
	wchar_t *lang_english;
	wchar_t *lang;
	wchar_t *author;

	// tray
	wchar_t *tray_enabled;
	wchar_t *tray_disabled;

	// menu
	wchar_t *menu_enable;
	wchar_t *menu_disable;
	wchar_t *menu_hide;
	wchar_t *menu_update;
	wchar_t *menu_config;
	wchar_t *menu_about;
	wchar_t *menu_exit;

	// update
	wchar_t *update_balloon;
	wchar_t *update_dialog;
	wchar_t *update_nonew;

	// config
	wchar_t *title;
	wchar_t *tab_general;
	wchar_t *tab_input;
	wchar_t *tab_blacklist;
	wchar_t *tab_advanced;
	wchar_t *tab_about;

	// general
	wchar_t *general_box;
	wchar_t *general_autofocus;
	wchar_t *general_aero;
	wchar_t *general_inactivescroll;
	wchar_t *general_mdi;
	wchar_t *general_autosnap;
	wchar_t *general_autosnap0;
	wchar_t *general_autosnap1;
	wchar_t *general_autosnap2;
	wchar_t *general_autosnap3;
	wchar_t *general_language;
	wchar_t *general_autostart_box;
	wchar_t *general_autostart;
	wchar_t *general_autostart_hide;
	wchar_t *general_autostart_elevate;
	wchar_t *general_autostart_elevate_tip;
	wchar_t *general_elevate;
	wchar_t *general_elevated;
	wchar_t *general_elevate_tip;
	wchar_t *general_elevation_aborted;
	wchar_t *general_autosave;

	// input
	// mouse
	wchar_t *input_mouse_box;
	wchar_t *input_mouse_lmb;
	wchar_t *input_mouse_mmb;
	wchar_t *input_mouse_rmb;
	wchar_t *input_mouse_mb4;
	wchar_t *input_mouse_mb5;
	wchar_t *input_mouse_scroll;
	wchar_t *input_mouse_lowerwithmmb;
	// actions
	wchar_t *input_actions_move;
	wchar_t *input_actions_resize;
	wchar_t *input_actions_close;
	wchar_t *input_actions_minimize;
	wchar_t *input_actions_lower;
	wchar_t *input_actions_alwaysontop;
	wchar_t *input_actions_center;
	wchar_t *input_actions_nothing;
	wchar_t *input_actions_alttab;
	wchar_t *input_actions_volume;
	wchar_t *input_actions_transparency;
	// hotkeys
	wchar_t *input_hotkeys_box;
	wchar_t *input_hotkeys_leftalt;
	wchar_t *input_hotkeys_rightalt;
	wchar_t *input_hotkeys_leftwinkey;
	wchar_t *input_hotkeys_rightwinkey;
	wchar_t *input_hotkeys_leftctrl;
	wchar_t *input_hotkeys_rightctrl;
	wchar_t *input_hotkeys_more;

	// blacklist
	wchar_t *blacklist_box;
	wchar_t *blacklist_processblacklist;
	wchar_t *blacklist_blacklist;
	wchar_t *blacklist_snaplist;
	wchar_t *blacklist_explanation;
	wchar_t *blacklist_findwindow_box;
	wchar_t *blacklist_findwindow_explanation;

	// advanced
	wchar_t *advanced_box;
	wchar_t *advanced_hookwindows;
	wchar_t *advanced_hookwindows_warn;
	wchar_t *advanced_checkonstartup;
	wchar_t *advanced_beta;
	wchar_t *advanced_checknow;
	wchar_t *advanced_ini;
	wchar_t *advanced_openini;

	// about
	wchar_t *about_box;
	wchar_t *about_version;
	wchar_t *about_author;
	wchar_t *about_license;
	wchar_t *about_donate;
	wchar_t *about_translation_credit;

	/* misc */
	wchar_t *unhook_error;
};

struct strings l10n_ini;

struct {
	wchar_t **str;
	wchar_t *name;
} l10n_mapping[] = {
	{ &l10n_ini.code,                                    L"Code" },
	{ &l10n_ini.lang_english,                            L"LangEnglish" },
	{ &l10n_ini.lang,                                    L"Lang" },
	{ &l10n_ini.author,                                  L"Author" },

	{ &l10n_ini.tray_enabled,                            L"TrayEnabled" },
	{ &l10n_ini.tray_disabled,                           L"TrayDisabled" },
	{ &l10n_ini.menu_enable,                             L"MenuEnable" },
	{ &l10n_ini.menu_disable,                            L"MenuDisable" },
	{ &l10n_ini.menu_hide,                               L"MenuHideTray" },
	{ &l10n_ini.menu_update,                             L"MenuUpdateAvailable" },
	{ &l10n_ini.menu_config,                             L"MenuConfigure" },
	{ &l10n_ini.menu_about,                              L"MenuAbout" },
	{ &l10n_ini.menu_exit,                               L"MenuExit" },
	{ &l10n_ini.update_balloon,                          L"UpdateBalloon" },
	{ &l10n_ini.update_dialog,                           L"UpdateDialog" },
	{ &l10n_ini.update_nonew,                            L"UpdateLatest" },

	{ &l10n_ini.title,                                   L"ConfigTitle" },
	{ &l10n_ini.tab_general,                             L"ConfigTabGeneral" },
	{ &l10n_ini.tab_input,                               L"ConfigTabInput" },
	{ &l10n_ini.tab_blacklist,                           L"ConfigTabBlacklist" },
	{ &l10n_ini.tab_advanced,                            L"ConfigTabAdvanced" },
	{ &l10n_ini.tab_about,                               L"ConfigTabAbout" },

	{ &l10n_ini.general_box,                             L"GeneralBox" },
	{ &l10n_ini.general_autofocus,                       L"GeneralAutoFocus" },
	{ &l10n_ini.general_aero,                            L"GeneralAero" },
	{ &l10n_ini.general_inactivescroll,                  L"GeneralInactiveScroll" },
	{ &l10n_ini.general_mdi,                             L"GeneralMDI" },
	{ &l10n_ini.general_autosnap,                        L"GeneralAutoSnap" },
	{ &l10n_ini.general_autosnap0,                       L"GeneralAutoSnap0" },
	{ &l10n_ini.general_autosnap1,                       L"GeneralAutoSnap1" },
	{ &l10n_ini.general_autosnap2,                       L"GeneralAutoSnap2" },
	{ &l10n_ini.general_autosnap3,                       L"GeneralAutoSnap3" },
	{ &l10n_ini.general_language,                        L"GeneralLanguage" },
	{ &l10n_ini.general_autostart_box,                   L"GeneralAutostartBox" },
	{ &l10n_ini.general_autostart,                       L"GeneralAutostart" },
	{ &l10n_ini.general_autostart_hide,                  L"GeneralAutostartHide" },
	{ &l10n_ini.general_autostart_elevate,               L"GeneralAutostartElevate" },
	{ &l10n_ini.general_autostart_elevate_tip,           L"GeneralAutostartElevateTip" },
	{ &l10n_ini.general_elevate,                         L"GeneralElevate" },
	{ &l10n_ini.general_elevated,                        L"GeneralElevated" },
	{ &l10n_ini.general_elevate_tip,                     L"GeneralElevateTip" },
	{ &l10n_ini.general_elevation_aborted,               L"GeneralElevationAborted" },
	{ &l10n_ini.general_autosave,                        L"GeneralAutosave" },

	{ &l10n_ini.input_mouse_box,                         L"InputMouseBox" },
	{ &l10n_ini.input_mouse_lmb,                         L"InputMouseLMB" },
	{ &l10n_ini.input_mouse_mmb,                         L"InputMouseMMB" },
	{ &l10n_ini.input_mouse_rmb,                         L"InputMouseRMB" },
	{ &l10n_ini.input_mouse_mb4,                         L"InputMouseMB4" },
	{ &l10n_ini.input_mouse_mb5,                         L"InputMouseMB5" },
	{ &l10n_ini.input_mouse_scroll,                      L"InputMouseScroll" },
	{ &l10n_ini.input_mouse_lowerwithmmb,                L"InputMouseLowerWithMMB" },

	{ &l10n_ini.input_actions_move,                      L"InputActionMove" },
	{ &l10n_ini.input_actions_resize,                    L"InputActionResize" },
	{ &l10n_ini.input_actions_close,                     L"InputActionClose" },
	{ &l10n_ini.input_actions_minimize,                  L"InputActionMinimize" },
	{ &l10n_ini.input_actions_lower,                     L"InputActionLower" },
	{ &l10n_ini.input_actions_alwaysontop,               L"InputActionAlwaysOnTop" },
	{ &l10n_ini.input_actions_center,                    L"InputActionCenter" },
	{ &l10n_ini.input_actions_nothing,                   L"InputActionNothing" },
	{ &l10n_ini.input_actions_alttab,                    L"InputActionAltTab" },
	{ &l10n_ini.input_actions_volume,                    L"InputActionVolume" },
	{ &l10n_ini.input_actions_transparency,              L"InputActionTransparency" },

	{ &l10n_ini.input_hotkeys_box,                       L"InputHotkeysBox" },
	{ &l10n_ini.input_hotkeys_leftalt,                   L"InputHotkeysLeftAlt" },
	{ &l10n_ini.input_hotkeys_rightalt,                  L"InputHotkeysRightAlt" },
	{ &l10n_ini.input_hotkeys_leftwinkey,                L"InputHotkeysLeftWinkey" },
	{ &l10n_ini.input_hotkeys_rightwinkey,               L"InputHotkeysRightWinkey" },
	{ &l10n_ini.input_hotkeys_leftctrl,                  L"InputHotkeysLeftCtrl" },
	{ &l10n_ini.input_hotkeys_rightctrl,                 L"InputHotkeysRightCtrl" },
	{ &l10n_ini.input_hotkeys_more,                      L"InputHotkeysMore" },

	{ &l10n_ini.blacklist_box,                           L"BlacklistBox" },
	{ &l10n_ini.blacklist_processblacklist,              L"BlacklistProcessBlacklist" },
	{ &l10n_ini.blacklist_blacklist,                     L"BlacklistBlacklist" },
	{ &l10n_ini.blacklist_snaplist,                      L"BlacklistSnaplist" },
	{ &l10n_ini.blacklist_explanation,                   L"BlacklistExplanation" },
	{ &l10n_ini.blacklist_findwindow_box,                L"BlacklistFindWindowBox" },
	{ &l10n_ini.blacklist_findwindow_explanation,        L"BlacklistFindWindowExplanation" },

	{ &l10n_ini.advanced_box,                            L"AdvancedBox" },
	{ &l10n_ini.advanced_hookwindows,                    L"AdvancedHookWindows" },
	{ &l10n_ini.advanced_hookwindows_warn,               L"AdvancedHookWindowsWarn" },
	{ &l10n_ini.advanced_checkonstartup,                 L"AdvancedUpdateCheckOnStartup" },
	{ &l10n_ini.advanced_beta,                           L"AdvancedUpdateBeta" },
	{ &l10n_ini.advanced_checknow,                       L"AdvancedUpdateCheckNow" },
	{ &l10n_ini.advanced_ini,                            L"AdvancedIni" },
	{ &l10n_ini.advanced_openini,                        L"AdvancedOpenIni" },

	{ &l10n_ini.about_box,                               L"AboutBox" },
	{ &l10n_ini.about_version,                           L"AboutVersion" },
	{ &l10n_ini.about_author,                            L"AboutAuthor" },
	{ &l10n_ini.about_license,                           L"AboutLicense" },
	{ &l10n_ini.about_donate,                            L"AboutDonate" },
	{ &l10n_ini.about_translation_credit,                L"AboutTranslationCredit" },

	{ &l10n_ini.unhook_error,                            L"MiscUnhookError" },
};
