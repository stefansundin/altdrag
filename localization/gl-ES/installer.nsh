;AltDrag - gl-ES localization by Alexander De Sousa (xandy.lua@gmail.com)
;
;This program is free software: you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.

!insertmacro MUI_LANGUAGE "Galician"
LangString L10N_LANG ${LANG_GALICIAN} ""

LangString L10N_UPGRADE_TITLE     0 "Xa está instalado."
LangString L10N_UPGRADE_SUBTITLE  0 "Escolle como queres instalar ${APP_NAME}."
LangString L10N_UPGRADE_HEADER    0 "${APP_NAME} xa está instalado no teu sistema. Selecciona a operación que queres realizar e preme Seguinte para continuar."
LangString L10N_UPGRADE_UPGRADE   0 "&Actualizar ${APP_NAME} á versión ${APP_VERSION}."
LangString L10N_UPGRADE_INI       0 "A configuración actual copiarase a ${APP_NAME}-old.ini."
LangString L10N_UPGRADE_INSTALL   0 "&Instalar de novo."
LangString L10N_UPGRADE_UNINSTALL 0 "&Desinstalar ${APP_NAME}."
LangString L10N_UPDATE_SECTION    0 "Comprobar actualizacións antes de instalar"
LangString L10N_UPDATE_DIALOG     0 "Unha nova versión está dispoñible.$\nQueres abortar a instalación e ir á páxina web?"
LangString L10N_SHORTCUT          0 "Atallo no menú de inicio"
LangString L10N_AUTOSTART         0 "Arrancar ó iniciar o sistema"
LangString L10N_AUTOSTART_HIDE    0 "Esconder a icona na bandexa do sistema"

LangString L10N_ALTSHIFT_TITLE    0 "Atallo no teclado"
LangString L10N_ALTSHIFT_SUBTITLE 0 "O atallo Alt + Shift interfire con ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   0 "O instalador detectou que o atallo para cambiar de esquema de teclado é Alt + Shift.$\n$\nCando usas ${APP_NAME}, podes premer Shift mentras moves ventás para facer que se peguen a outras ventás. Isto significa que é probable que premas Alt + Shift, a mesma combinación que activa o cambio de distribución do teclado. ${APP_NAME} trata de bloquear o cambio accidental de teclado internamente, non será posible se premes Shift antes de comezar a mover a ventá.$\n$\nPodes desactivar ou cambiar o atallo que activa o cambio de teclado premendo este botón. Preme Seguinte para continuar."
LangString L10N_ALTSHIFT_BUTTON   0 "&Config. do teclado"

LangString L10N_HOOKTIMEOUT_TITLE          0 "Registry tweak"
LangString L10N_HOOKTIMEOUT_SUBTITLE       0 "Optional tweak to keep ${APP_NAME} from stopping unexpectedly."
LangString L10N_HOOKTIMEOUT_HEADER         0 "In Windows 7, Microsoft implemented a feature that stops keyboard and mouse hooks if they take too long to respond. Unfortunately, this check can erroneously misbehave, especially if you hibernate, sleep, or lock the computer a lot.$\n$\nIf this happens, you will find that ${APP_NAME} stop functioning without warning, and you have to manually disable and enable ${APP_NAME} to make it work again.$\n$\nThere is a registry tweak to make Windows wait longer before stopping hooks, which you can enable or disable by using the buttons below. Please note that this registry tweak is completely optional."
LangString L10N_HOOKTIMEOUT_APPLYBUTTON    0 "&Enable registry tweak"
LangString L10N_HOOKTIMEOUT_REVERTBUTTON   0 "&Disable registry tweak"
LangString L10N_HOOKTIMEOUT_ALREADYAPPLIED 0 "The registry tweak has already been applied."
LangString L10N_HOOKTIMEOUT_FOOTER         0 "The change will take effect on your next login."
