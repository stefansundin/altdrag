;AltDrag - fr-FR localization by Samy Mechiri (samy.mechiri@kxen.com)
;
;This program is free software: you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.

!insertmacro MUI_LANGUAGE "French"
!define LANG ${LANG_FRENCH}

LangString L10N_UPGRADE_TITLE     ${LANG} "Déjà installé"
LangString L10N_UPGRADE_SUBTITLE  ${LANG} "Choisissez comment vous désirez installer ${APP_NAME}."
LangString L10N_UPGRADE_HEADER    ${LANG} "${APP_NAME} est déjà installé sur ce système. Choisissez une opération et clickez sur Suivant pour continuer."
LangString L10N_UPGRADE_UPGRADE   ${LANG} "&Mettre à jour ${APP_NAME} vers ${APP_VERSION}."
LangString L10N_UPGRADE_INI       ${LANG} "Vos paramètres précédents seront copiés dans ${APP_NAME}-old.ini."
LangString L10N_UPGRADE_INSTALL   ${LANG} "&Installer dans un nouveau dossier."
LangString L10N_UPGRADE_UNINSTALL ${LANG} "&Désinstaller ${APP_NAME}."
LangString L10N_UPDATE_SECTION    ${LANG} "Chercher les mises-à-jours avant d'installer"
LangString L10N_UPDATE_DIALOG     ${LANG} "Une nouvelle version est disponible.$\nAnnuler l'installation et aller sur le site web?"
LangString L10N_SHORTCUT          ${LANG} "Raccourci dans le menu Démarrer"
LangString L10N_AUTOSTART         ${LANG} "Démarrage automatique"
LangString L10N_AUTOSTART_HIDE    ${LANG} "Cacher l'icone dans la barre d'outils"

LangString L10N_ALTSHIFT_TITLE    ${LANG} "Raccourcis clavier"
LangString L10N_ALTSHIFT_SUBTITLE ${LANG} "Le raccourci Alt + Shift est en conflit avec ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   ${LANG} "L'installeur à détécté que le raccourci de Windows pour changer la configuration du clavier est Alt + Shift.$\n$\nEn utilisant ${APP_NAME}, vous pouvez maintenir Shift en faisant glisser des fenêtres pour aimanter les fenêtres. Celà signifie que vous pourrez être amenés à tapper Alt + Shift, la même combinaison de touche qui change la configuration du clavier. ${APP_NAME} tente d'empecher le changement accidentel de configuration du clavier, cependant il ne pourra le faire si vous maintenez Shift avant de faire glisser une fenêtre.$\n$\nVous pouvez désactiver ou modifier le raccourcis qui change la configuration du clavier en cliquant sur le bouton ci-dessous. Cliquez sur Suivant pour continuer."
LangString L10N_ALTSHIFT_BUTTON   ${LANG} "&Paramètres clavier"

!undef LANG
