; AltDrag - de-DE localization by Markus Hentsch
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.

!insertmacro MUI_LANGUAGE "German"
LangString L10N_LANG ${LANG_GERMAN} ""

LangString L10N_UPGRADE_TITLE     0 "Bereits installiert"
LangString L10N_UPGRADE_SUBTITLE  0 "Wählen Sie aus, wie ${APP_NAME} installiert werden soll."
LangString L10N_UPGRADE_HEADER    0 "${APP_NAME} ist auf diesem System bereits installiert. Wählen Sie die gewünschte Aktion aus und klicken Sie auf Weiter um fortzufahren."
LangString L10N_UPGRADE_UPGRADE   0 "&Aktualisierung von ${APP_NAME} auf ${APP_VERSION}."
LangString L10N_UPGRADE_INI       0 "Ihre bisherigen Einstellungen werden nach ${APP_NAME}-old.ini kopiert."
LangString L10N_UPGRADE_INSTALL   0 "An einem neuen Ort &installieren."
LangString L10N_UPGRADE_UNINSTALL 0 "${APP_NAME} &deinstallieren."
LangString L10N_UPDATE_DIALOG     0 "Eine neue Version ist verfügbar.$\nSoll die Installation abgebrochen und die Webseite aufgerufen werden?"

LangString L10N_ALTSHIFT_TITLE    0 "Tastenkombination"
LangString L10N_ALTSHIFT_SUBTITLE 0 "Die Tastenkombination Alt + Shift steht mit ${APP_NAME} im Konflikt."
LangString L10N_ALTSHIFT_HEADER   0 "Das Installationsprogramm hat festgestellt, dass Alt + Shift derzeit von Windows dazu verwendet wird, das Tastaturlayout umzuschalten.$\n$\nMit ${APP_NAME} können bewegte Fenster bei gedrückter Shift-Taste aneinander eingerastet werden. Es kann also vorkommen, dass Sie damit versehentlich das Tastaturlayout ändern. ${APP_NAME} versucht zwar intern das Wechseln des Tastaturlayouts zu verhindern, scheitert aber wenn Sie die Shift-Taste drücken noch bevor Sie das Fenster bewegen.$\n$\nSie können die Tastenkombination zum Umschalten des Tastaturlayouts ändern, indem Sie auf den nachfolgenden Button klicken. Klicken Sie auf Weiter um fortzufahren."
LangString L10N_ALTSHIFT_BUTTON   0 "&Einstellungen öffnen"

LangString L10N_HOOKTIMEOUT_TITLE          0 "Registry-Tweak"
LangString L10N_HOOKTIMEOUT_SUBTITLE       0 "Optionaler Tweak um zu verhindern, dass ${APP_NAME} unerwartet beendet wird."
LangString L10N_HOOKTIMEOUT_HEADER         0 "In Windows 7 hat Microsoft eine Funktion implementiert, die Tastatur- und Maus-Hooks selbstständig beendet, sobald diese eine zu große Antwortverzögerung haben. Leider kommt es vor, dass diese Funktion fälschlicherweise ausgelöst wird, wenn der Computer oft gesperrt oder in Ruhezustand bzw. Standby versetzt wird.$\n$\nWenn das passiert, wird ${APP_NAME} ohne jegliche Warnung aufhören zu funktionieren und muss manuell deaktiviert und wieder aktiviert werden.$\n$\nEs gibt einen Registry-Tweak um Windows dazu zu bringen, länger auf Hooks zu warten bevor sie beendet werden. Sie können diesen über die nachfolgenden Buttons aktivieren oder deaktivieren. Bitte beachten Sie, dass dieser Registry-Tweak völlig optional ist."
LangString L10N_HOOKTIMEOUT_APPLYBUTTON    0 "Registry-Tweak a&ktivieren"
LangString L10N_HOOKTIMEOUT_REVERTBUTTON   0 "Registry-Tweak &deaktivieren"
LangString L10N_HOOKTIMEOUT_ALREADYAPPLIED 0 "Der Registry-Tweak wurde bereits aktiviert."
LangString L10N_HOOKTIMEOUT_FOOTER         0 "Die Änderung wird bei Ihrer nächsten Anmeldung übernommen."
