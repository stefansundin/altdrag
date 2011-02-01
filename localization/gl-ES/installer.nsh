;AltDrag - gl-ES localization by Alexander De Sousa (xandy.lua@gmail.com)
;
;This program is free software: you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.

!insertmacro MUI_LANGUAGE "Galician"
!define LANG ${LANG_GALICIAN}

LangString L10N_UPGRADE_TITLE     ${LANG} "Xa está instalado."
LangString L10N_UPGRADE_SUBTITLE  ${LANG} "Escolle como queres instalar ${APP_NAME}."
LangString L10N_UPGRADE_HEADER    ${LANG} "${APP_NAME} xa está instalado no teu sistema. Selecciona a operación que queres realizar e preme Seguinte para continuar."
LangString L10N_UPGRADE_UPGRADE   ${LANG} "&Actualizar ${APP_NAME} á versión ${APP_VERSION}."
LangString L10N_UPGRADE_INI       ${LANG} "A configuración actual copiarase a ${APP_NAME}-old.ini."
LangString L10N_UPGRADE_INSTALL   ${LANG} "&Instalar de novo."
LangString L10N_UPGRADE_UNINSTALL ${LANG} "&Desinstalar ${APP_NAME}."
LangString L10N_UPDATE_SECTION    ${LANG} "Comprobar actualizacións antes de instalar"
LangString L10N_UPDATE_DIALOG     ${LANG} "Unha nova versión está dispoñible.$\nQueres abortar a instalación e ir á páxina web?"
LangString L10N_RUNNING           ${LANG} "${APP_NAME} está en funcionamento. Quérelo pechar?"
LangString L10N_RUNNING_UNINSTALL ${LANG} "Se seleccionas Non, ${APP_NAME} eliminarase completamente cando reinicies."
LangString L10N_SHORTCUT          ${LANG} "Atallo no menú de inicio"
LangString L10N_AUTOSTART         ${LANG} "Arrancar ó iniciar o sistema"
LangString L10N_AUTOSTART_HIDE    ${LANG} "Esconder a icona na bandexa do sistema"

LangString L10N_ALTSHIFT_TITLE    ${LANG} "Atallo no teclado"
LangString L10N_ALTSHIFT_SUBTITLE ${LANG} "O atallo Alt + Shift interfire con ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   ${LANG} "O instalador detectou que o atallo para cambiar de esquema de teclado é Alt + Shift.$\n$\nCando usas ${APP_NAME}, podes premer Shift mentras moves ventás para facer que se peguen a outras ventás. Isto significa que é probable que premas Alt + Shift, a mesma combinación que activa o cambio de distribución do teclado. ${APP_NAME} trata de bloquear o cambio accidental de teclado internamente, non será posible se premes Shift antes de comezar a mover a ventá.$\n$\nPodes desactivar ou cambiar o atallo que activa o cambio de teclado premendo este botón. Preme Seguinte para continuar."
LangString L10N_ALTSHIFT_BUTTON   ${LANG} "&Config. do teclado"

!undef LANG
