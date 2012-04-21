;AltDrag - en-US localization by Stefan Sundin (recover89@gmail.com)
;
;This program is free software: you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.

!insertmacro MUI_LANGUAGE "English" ;English name of this language
LangString L10N_LANG ${LANG_ENGLISH} ""

LangString L10N_UPGRADE_TITLE     0 "Already Installed"
LangString L10N_UPGRADE_SUBTITLE  0 "Choose how you want to install ${APP_NAME}."
LangString L10N_UPGRADE_HEADER    0 "${APP_NAME} is already installed on this system. Select the operation you want to perform and click Next to continue."
LangString L10N_UPGRADE_UPGRADE   0 "&Upgrade ${APP_NAME} to ${APP_VERSION}."
LangString L10N_UPGRADE_INI       0 "Your existing settings will be copied to ${APP_NAME}-old.ini."
LangString L10N_UPGRADE_INSTALL   0 "&Install to a new location."
LangString L10N_UPGRADE_UNINSTALL 0 "&Uninstall ${APP_NAME}."
LangString L10N_UPDATE_DIALOG     0 "A new version is available.$\nAbort install and go to website?"

LangString L10N_ALTSHIFT_TITLE    0 "Keyboard Shortcut"
LangString L10N_ALTSHIFT_SUBTITLE 0 "The shortcut Alt + Shift conflicts with ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   0 "The installer has detected that the Windows shortcut to toggle the current keyboard layout is Alt + Shift.$\n$\nWhen using ${APP_NAME}, you can press Shift while dragging windows to make them snap to other windows. This means you are likely to press Alt + Shift, the same combination which toggles your keyboard layout. While ${APP_NAME} internally tries to block accidentally switching the keyboard layout, it will not succeed if you press Shift before you start dragging a window.$\n$\nYou can disable or change the shortcut which toggles the keyboard layout by pressing this button. Click Next to continue."
LangString L10N_ALTSHIFT_BUTTON   0 "&Open keyboard settings"
