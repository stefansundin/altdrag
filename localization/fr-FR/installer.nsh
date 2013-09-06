; AltDrag - fr-FR localization by Samy Mechiri (samy.mechiri@kxen.com)
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.

!insertmacro MUI_LANGUAGE "French"
LangString L10N_LANG ${LANG_FRENCH} ""

LangString L10N_UPGRADE_TITLE     0 "Déjà installé"
LangString L10N_UPGRADE_SUBTITLE  0 "Choisissez comment vous désirez installer ${APP_NAME}."
LangString L10N_UPGRADE_HEADER    0 "${APP_NAME} est déjà installé sur ce système. Choisissez une opération et clickez sur Suivant pour continuer."
LangString L10N_UPGRADE_UPGRADE   0 "&Mettre à jour ${APP_NAME} vers ${APP_VERSION}."
LangString L10N_UPGRADE_INI       0 "Vos paramètres précédents seront copiés dans ${APP_NAME}-old.ini."
LangString L10N_UPGRADE_INSTALL   0 "&Installer dans un nouveau dossier."
LangString L10N_UPGRADE_UNINSTALL 0 "&Désinstaller ${APP_NAME}."
LangString L10N_UPDATE_DIALOG     0 "Une nouvelle version est disponible.$\nAnnuler l'installation et aller sur le site web?"

LangString L10N_ALTSHIFT_TITLE    0 "Raccourcis clavier"
LangString L10N_ALTSHIFT_SUBTITLE 0 "Le raccourci Alt + Shift est en conflit avec ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   0 "L'installeur à détécté que le raccourci de Windows pour changer la configuration du clavier est Alt + Shift.$\n$\nEn utilisant ${APP_NAME}, vous pouvez maintenir Shift en faisant glisser des fenêtres pour aimanter les fenêtres. Celà signifie que vous pourrez être amenés à tapper Alt + Shift, la même combinaison de touche qui change la configuration du clavier. ${APP_NAME} tente d'empecher le changement accidentel de configuration du clavier, cependant il ne pourra le faire si vous maintenez Shift avant de faire glisser une fenêtre.$\n$\nVous pouvez désactiver ou modifier le raccourcis qui change la configuration du clavier en cliquant sur le bouton ci-dessous. Cliquez sur Suivant pour continuer."
LangString L10N_ALTSHIFT_BUTTON   0 "&Paramètres clavier"

LangString L10N_HOOKTIMEOUT_TITLE          0 "Registry tweak"
LangString L10N_HOOKTIMEOUT_SUBTITLE       0 "Optional tweak to keep ${APP_NAME} from stopping unexpectedly."
LangString L10N_HOOKTIMEOUT_HEADER         0 "In Windows 7, Microsoft implemented a feature that stops keyboard and mouse hooks if they take too long to respond. Unfortunately, this check can erroneously misbehave, especially if you hibernate, sleep, or lock the computer a lot.$\n$\nIf this happens, you will find that ${APP_NAME} stop functioning without warning, and you have to manually disable and enable ${APP_NAME} to make it work again.$\n$\nThere is a registry tweak to make Windows wait longer before stopping hooks, which you can enable or disable by using the buttons below. Please note that this registry tweak is completely optional."
LangString L10N_HOOKTIMEOUT_APPLYBUTTON    0 "&Enable registry tweak"
LangString L10N_HOOKTIMEOUT_REVERTBUTTON   0 "&Disable registry tweak"
LangString L10N_HOOKTIMEOUT_ALREADYAPPLIED 0 "The registry tweak has already been applied."
LangString L10N_HOOKTIMEOUT_FOOTER         0 "The change will take effect on your next login."
