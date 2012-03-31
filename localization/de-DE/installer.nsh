;AltDrag - de-DE localization by Stefan Rüster
;
;This program is free software: you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.

!insertmacro MUI_LANGUAGE "German"
!define LANG ${LANG_GERMAN}

LangString L10N_UPGRADE_TITLE     ${LANG} "Bereits installiert"
LangString L10N_UPGRADE_SUBTITLE  ${LANG} "Wie soll ${APP_NAME} installiert werden?"
LangString L10N_UPGRADE_HEADER    ${LANG} "${APP_NAME} ist auf diesem System bereits installiert. Bitte Aktion auswählen und auf Weiter klicken um fortzufahren."
LangString L10N_UPGRADE_UPGRADE   ${LANG} "&Upgrade von ${APP_NAME} auf ${APP_VERSION}."
LangString L10N_UPGRADE_INI       ${LANG} "Vorhandene Einstellungen werden nach ${APP_NAME}-old.ini kopiert."
LangString L10N_UPGRADE_INSTALL   ${LANG} "In einen anderen Ordner &installieren."
LangString L10N_UPGRADE_UNINSTALL ${LANG} "&Deinstallieren ${APP_NAME}."
LangString L10N_UPDATE_SECTION    ${LANG} "Vor der Installation nach Updates suchen"
LangString L10N_UPDATE_DIALOG     ${LANG} "Eine neue Version ist verfügbar.$\nInstallation abbrechen und zur Website wechseln?"
LangString L10N_SHORTCUT          ${LANG} "Verknüpfung im Startmenü"
LangString L10N_AUTOSTART         ${LANG} "Autostart"
LangString L10N_AUTOSTART_HIDE    ${LANG} "Taskleistensymbol ausblenden"

LangString L10N_ALTSHIFT_TITLE    ${LANG} "Tastatur Shortcut"
LangString L10N_ALTSHIFT_SUBTITLE ${LANG} "Der Shortcut Alt + Shift steht im Konflikt mit ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   ${LANG} "Das Installationsprogramm hat festgestellt, dass der Windows Shortcut zum Wechseln des Tastaturlayouts Alt + Shift ist.$\n$\nMit ${APP_NAME} kannst Du Fenster aneinander einrasten lassen, indem Du Shift gedrückt hälst, während Du ein Fenster bewegst. Daher kann es vorkommen, dass Du damit aus Versehen das Tastaturlayout änderst. ${APP_NAME} versucht intern zu verhindern, dass das Tastaturlayout gewechselt wird, aber das funktioniert nicht, wenn Du Shift drückst, bevor Du ein Fenster bewegst.$\n$\nDu kannst die Tastenkombination zum Ändern des Tastaturlayouts wechseln, indem Du hier klickst. Klicke auf Weiter um fortzufahren."
LangString L10N_ALTSHIFT_BUTTON   ${LANG} "&Tastatureinstellungen öffnen"

!undef LANG