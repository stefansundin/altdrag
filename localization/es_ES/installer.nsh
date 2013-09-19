; AltDrag - es-ES localization by Jorge Amigo (alpof@yahoo.com)
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.

!insertmacro MUI_LANGUAGE "Spanish"
LangString L10N_LANG ${LANG_SPANISH} ""

LangString L10N_UPGRADE_TITLE     0 "Ya instalado"
LangString L10N_UPGRADE_SUBTITLE  0 "Elija cómo instalar ${APP_NAME}."
LangString L10N_UPGRADE_HEADER    0 "${APP_NAME} ya está instalado en este sistema. Seleccione la operación que quiere realizar y pulse Siguiente para continuar."
LangString L10N_UPGRADE_UPGRADE   0 "&Actualizar ${APP_NAME} a ${APP_VERSION}."
LangString L10N_UPGRADE_INI       0 "La configuración existente será copiada a ${APP_NAME}-old.ini."
LangString L10N_UPGRADE_INSTALL   0 "&Instalar en una nueva ubicación."
LangString L10N_UPGRADE_UNINSTALL 0 "Desinstalar ${APP_NAME}."
LangString L10N_UPDATE_DIALOG     0 "Existe una nueva versión disponible.$\nAbortar la instalación e ir al sitio web?"

LangString L10N_ALTSHIFT_TITLE    0 "Atajo de teclado"
LangString L10N_ALTSHIFT_SUBTITLE 0 "El atajo Alt + Mayúsculas entra en conflicto con ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   0 "El instalador ha detectado que el atajo de Windows para activar la actual distribución de teclado es Alt + Mayúsculas.$\n$\nCuando se usa ${APP_NAME}, se puede pulsar Mayúsculas mientras se arrastran ventanas para hacer que se peguen a otras ventanas. Esto significa que es posible que se presione Alt + Mayúsculas, la misma combinación que activa la distribución de teclado. Mientras que ${APP_NAME} internamente trata de bloquear el cambio accidental de distribución de teclado, no lo conseguirá si se presiona Mayúsculas antes de comenzar a arrastrar una ventana.$\n$\nSe puede deshabilitar o cambiar el atajo que avtiva la distribución de teclado presionando este botón. Pulse Siguiente para continuar."
LangString L10N_ALTSHIFT_BUTTON   0 "&Abrir la configuración del teclado"

LangString L10N_HOOKTIMEOUT_TITLE          0 "Ajuste del registro"
LangString L10N_HOOKTIMEOUT_SUBTITLE       0 "Ajuste opcional para evitar que ${APP_NAME} se cierre inesperadamente."
LangString L10N_HOOKTIMEOUT_HEADER         0 "En Windows 7, Microsoft implementó una funcionalidad para evitar ganchos de teclado y ratón si tardan demasiado en responder. Desgraciadamente, esta funcionalidad puede comportarse erróneamente, especialmente si se hiberna, suspende o bloquea mucho el equipo.$\n$\nSi esto ocurre, se observará que ${APP_NAME} dejará de funcionar sin ningún aviso, y ${APP_NAME} deberá ser manualmente deshabilitado y habilitado para hacer que vuelva a funcionar.$\n$\nExiste un ajuste de registro para hacer que Windows espere más tiempo antes de parar los ganchos, que puede ser habilitado o deshabilitado usando los botones de abajo. Por favor, nótese que este ajuste de registro es completamente opcional."
LangString L10N_HOOKTIMEOUT_APPLYBUTTON    0 "&Habilitar el ajuste de registro"
LangString L10N_HOOKTIMEOUT_REVERTBUTTON   0 "&Deshabilitar el ajuste de registro"
LangString L10N_HOOKTIMEOUT_ALREADYAPPLIED 0 "El ajuste de registro ya ha sido aplicado."
LangString L10N_HOOKTIMEOUT_FOOTER         0 "El cambio se hará efectivo en el próximo inicio de sesión."
