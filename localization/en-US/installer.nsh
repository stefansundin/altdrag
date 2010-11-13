;AltDrag - en-US localization by Stefan Sundin (recover89@gmail.com)
;
;This program is free software: you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.

!insertmacro MUI_LANGUAGE "English"
!define LANG ${LANG_ENGLISH}

LangString L10N_UPGRADE_TITLE     ${LANG} "Already Installed"
LangString L10N_UPGRADE_SUBTITLE  ${LANG} "Choose how you want to install ${APP_NAME}."
LangString L10N_UPGRADE_HEADER    ${LANG} "${APP_NAME} is already installed on this system. Select the operation you want to perform and click Next to continue."
LangString L10N_UPGRADE_UPGRADE   ${LANG} "&Upgrade ${APP_NAME} to ${APP_VERSION}."
LangString L10N_UPGRADE_INI       ${LANG} "Your existing settings will be copied to ${APP_NAME}-old.ini."
LangString L10N_UPGRADE_INSTALL   ${LANG} "&Install to a new location."
LangString L10N_UPGRADE_UNINSTALL ${LANG} "&Uninstall ${APP_NAME}."
LangString L10N_UPDATE_SECTION    ${LANG} "Check for update before installing"
LangString L10N_UPDATE_DIALOG     ${LANG} "A new version is available.$\nAbort install and go to website?"
LangString L10N_RUNNING           ${LANG} "${APP_NAME} is running. Close?"
LangString L10N_RUNNING_UNINSTALL ${LANG} "If you choose No, ${APP_NAME} will be completely removed on next reboot."
LangString L10N_SHORTCUT          ${LANG} "Start Menu Shortcut"
LangString L10N_AUTOSTART         ${LANG} "Autostart"
LangString L10N_AUTOSTART_HIDE    ${LANG} "Hide tray"

LangString L10N_ALTSHIFT_TITLE    ${LANG} "Keyboard Shortcut"
LangString L10N_ALTSHIFT_SUBTITLE ${LANG} "The shortcut Alt + Shift conflicts with ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   ${LANG} "The installer has detected that you have more than one keyboard layout installed, and that the shortcut to toggle the current keyboard layout is Alt + Shift.$\n$\nWhen using ${APP_NAME}, you can press Shift while dragging windows to make them stick to other windows. This means you are likely to press Alt + Shift, the same combination which toggles your keyboard layout. While ${APP_NAME} internally tries to block accidentally switching the keyboard layout, it might not always succeed.$\n$\nYou can disable or change the shortcut which toggles the keyboard layout by pressing this button. Click Next to continue."
LangString L10N_ALTSHIFT_BUTTON   ${LANG} "&Open keyboard settings"

!undef LANG
