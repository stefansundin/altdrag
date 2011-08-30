;AltDrag - es-ES localization by Alexander De Sousa (xandy.lua@gmail.com)
;
;This program is free software: you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.

!insertmacro MUI_LANGUAGE "Spanish"
!define LANG ${LANG_SPANISH}

LangString L10N_UPGRADE_TITLE     ${LANG} "Ya está instalado"
LangString L10N_UPGRADE_SUBTITLE  ${LANG} "Elige el modo de instalación de ${APP_NAME}."
LangString L10N_UPGRADE_HEADER    ${LANG} "${APP_NAME} ya se encuentra instalado. Selecciona la operación que quieres llevar a cabo y pulsa Siguiente para continuar."
LangString L10N_UPGRADE_UPGRADE   ${LANG} "&Actualiza ${APP_NAME} a la versión ${APP_VERSION}."
LangString L10N_UPGRADE_INI       ${LANG} "Se creará una copia de la configuración actual en ${APP_NAME}-old.ini."
LangString L10N_UPGRADE_INSTALL   ${LANG} "&Instalar en otra ubicación"
LangString L10N_UPGRADE_UNINSTALL ${LANG} "&Desinstalar ${APP_NAME}."
LangString L10N_UPDATE_SECTION    ${LANG} "Comprobar actualizaciones antes de instalar"
LangString L10N_UPDATE_DIALOG     ${LANG} "Una nueva versión está disponible.$\n¿Anular instalación y visitar la página de descarga?"
LangString L10N_SHORTCUT          ${LANG} "Acceso directo en el menú Inicio"
LangString L10N_AUTOSTART         ${LANG} "Iniciar con Windows"
LangString L10N_AUTOSTART_HIDE    ${LANG} "Ocultar icono"

LangString L10N_ALTSHIFT_TITLE    ${LANG} "Atajo de teclado"
LangString L10N_ALTSHIFT_SUBTITLE ${LANG} "El atajo Alt + Mayús. entra en conflicto con ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   ${LANG} "El instalador ha detectado que el atajo de teclado para activar el esquema de teclado actual es Alt+Mayús.$\n$\nAl usar ${APP_NAME}, puedes pulsar Mayús mientras arrastras ventanas para hacer que se peguen a otras. Es probable que pulses Alt + Mayús., la misma combinación que cambia tu distribución de teclado. Aunque ${APP_NAME} intente bloquear estos cambios accidentales, no lo conseguirá si presionas Mayús antes de empezar a mover una ventana.$\n$\nPuedes desactivar o cambiar el atajo que cambia la distribución de teclado pulsando este botón. Pulsa aquí para continuar."
LangString L10N_ALTSHIFT_BUTTON   ${LANG} "&Configuración de Teclado"

!undef LANG
