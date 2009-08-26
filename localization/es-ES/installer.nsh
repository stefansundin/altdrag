;AltDrag - es-ES localization by Fabrizio Ferri (algernon@gmail.com)
;Do not localize APP_NAME, it will be automatically replaced.
;
;Copyright (C) 2009  Stefan Sundin (recover89@gmail.com)
;
;This program is free software: you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.

!if ${L10N_VERSION} == 2

!insertmacro MUI_LANGUAGE "Spanish"
!define LANG ${LANG_SPANISH}

LangString L10N_UPGRADE_TITLE     ${LANG} "Ya está instalado"
LangString L10N_UPGRADE_SUBTITLE  ${LANG} "Elige el modo de instalación de ${APP_NAME}."
LangString L10N_UPGRADE_HEADER    ${LANG} "${APP_NAME} ya se encuentra instalado. Selecciona la operación que quieres llevar a cabo y pulsa Siguiente para continuar."
LangString L10N_UPGRADE_UPGRADE   ${LANG} "&Actualiza ${APP_NAME} a la versión ${APP_VERSION}."
LangString L10N_UPGRADE_INI       ${LANG} "Se creará una copia de la configuración actual en ${APP_NAME}-old.ini."
LangString L10N_UPGRADE_INSTALL   ${LANG} "&Instalar en otra ubicación"
LangString L10N_UPDATE_SECTION    ${LANG} "Comprobar actualizaciones antes de instalar"
LangString L10N_UPDATE_DIALOG     ${LANG} "Una nueva versión está disponible.$\n¿Anular instalación y visitar la página de descarga?"
LangString L10N_RUNNING           ${LANG} "${APP_NAME} se está ejecutando. ¿Desea cerrarlo?"
LangString L10N_RUNNING_UNINSTALL ${LANG} "Si elige No, ${APP_NAME} será eliminado por complento en el próximo arranque."
LangString L10N_SHORTCUT          ${LANG} "Acceso directo en el menú Inicio"
LangString L10N_AUTOSTART         ${LANG} "Iniciar con Windows"
LangString L10N_AUTOSTART_HIDE    ${LANG} "Ocultar icono"

!undef LANG

!else
!warning "Localization out of date!" ;Don't localize this
!endif
