; AltDrag - gl-ES localization by Jorge Amigo (alpof@yahoo.com)
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.

!insertmacro MUI_LANGUAGE "Galician"
LangString L10N_LANG ${LANG_GALICIAN} ""

LangString L10N_UPGRADE_TITLE     0 "Xa instalado"
LangString L10N_UPGRADE_SUBTITLE  0 "Elixa cómo instalar ${APP_NAME}."
LangString L10N_UPGRADE_HEADER    0 "${APP_NAME} xa está instalado neste sistema. Escolla a operación que quere realizar e prema Seguinte para continuar."
LangString L10N_UPGRADE_UPGRADE   0 "&Actualizar ${APP_NAME} a ${APP_VERSION}."
LangString L10N_UPGRADE_INI       0 "A configuración existente será copiada a ${APP_NAME}-old.ini."
LangString L10N_UPGRADE_INSTALL   0 "&Instalar nunha nova ubicación."
LangString L10N_UPGRADE_UNINSTALL 0 "Desinstalar ${APP_NAME}."
LangString L10N_UPDATE_DIALOG     0 "Existe unha nova versión dispoñible.$\nAborta-la instalación e ir ó sitio web?"

LangString L10N_ALTSHIFT_TITLE    0 "Atallo de teclado"
LangString L10N_ALTSHIFT_SUBTITLE 0 "O atallo Alt + Maiúsculas entra en conflicto con ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   0 "O instalador detectou que o atallo de Windows para activa-la actual distribución de teclado é Alt + Maiúsculas.$\n$\nCando se usa ${APP_NAME}, pódese premer Maiúsculas mentres arrástranse as ventás para facer que se peguen a outras ventás. Esto significa que é posible que se prema Alt + Maiúsculas, a mesma combinación que activa a distribución de teclado. Mentres que ${APP_NAME} internamente trata de bloquea-lo cambio accidental de distribución de teclado, non o conseguirá se prémese Maiúsculas antes de comenza-lo arrastre dunha ventá.$\n$\nPódese deshabilitar ou cambia-lo atallo que activa a distribución de teclado premendo este botón. Prema Seguinte para continuar."
LangString L10N_ALTSHIFT_BUTTON   0 "&Abri-la configuración do teclado"

LangString L10N_HOOKTIMEOUT_TITLE          0 "Axuste do rexistro"
LangString L10N_HOOKTIMEOUT_SUBTITLE       0 "Axuste opcional para evitar que ${APP_NAME} péchese inesperadamente."
LangString L10N_HOOKTIMEOUT_HEADER         0 "En Windows 7, Microsoft implementóu unha funcionalidade para evitar ganchos de teclado e rato se tardan de máis en respostar. Desgraciadamente, esta funcionalidade pode comportarse erróneamente, especialmente se hiberna, suspende o bloquéase moito o equipo.$\n$\nSe esto ocurre, observarase que ${APP_NAME} deixará de funcionar sen avisar, e ${APP_NAME} deberá ser manualmente deshabilitado e habilitado para facer que volte a funcionar.$\n$\nExiste un axuste de rexistro para facer que Windows espere máis tempo antes de para-los ganchos, que pode ser habilitado ou deshabilitado usando os botóns de abaixo. Por favor, nótese que este axuste de rexistro é completamente opcional."
LangString L10N_HOOKTIMEOUT_APPLYBUTTON    0 "&Habilita-lo axuste de rexistro"
LangString L10N_HOOKTIMEOUT_REVERTBUTTON   0 "&Deshabilita-lo axuste de rexistro"
LangString L10N_HOOKTIMEOUT_ALREADYAPPLIED 0 "O axuste de registro xa se aplicóu."
LangString L10N_HOOKTIMEOUT_FOOTER         0 "O cambio farase efectivo no próximo inicio de sesión."
