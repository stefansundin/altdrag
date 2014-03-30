; AltDrag - ct-ES localization by Àlfons Sánchez
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.

!insertmacro MUI_LANGUAGE "Catalan"
LangString L10N_LANG ${LANG_CATALAN} ""

LangString L10N_UPGRADE_TITLE     0 "Ja instal·lat"
LangString L10N_UPGRADE_SUBTITLE  0 "Selecciona com instal·lar ${APP_NAME}."
LangString L10N_UPGRADE_HEADER    0 "${APP_NAME} ja està instal·lat en aquest sistema. Selecciona la operació a realitzar i clica Següent per continuar."
LangString L10N_UPGRADE_UPGRADE   0 "Act&ualitza ${APP_NAME} a ${APP_VERSION}."
LangString L10N_UPGRADE_INI       0 "Els teus paràmetres existents es copiaran a ${APP_NAME}-old.ini."
LangString L10N_UPGRADE_INSTALL   0 "&Instal·la en una nova ubicació."
LangString L10N_UPGRADE_UNINSTALL 0 "Desins&tal·la ${APP_NAME}."
LangString L10N_UPDATE_DIALOG     0 "A nova versió està disponible.$\nAbortar instal·lació i anar a la web?"

LangString L10N_ALTSHIFT_TITLE    0 "Drecera de teclat"
LangString L10N_ALTSHIFT_SUBTITLE 0 "La drecera Alt + Majúscules entra en conflicte amb ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   0 "El instal·lador ha detectat que la drecera de Windows per canviar la distribució de teclat és Alt + Shift.$\n$\nQuan uses ${APP_NAME}, pots premer Majúscules mentre arrossegues finestres per fer que s'adapten automàticament a altres. Açò implica que probablement premes Alt + Shift, la mateixa combinació que canvia la distribució de teclat. Mentre ${APP_NAME} intenta internament bloquejar un canvi accidental de la distribució de teclat, no funcionarà si prems Majúscules abans d'iniciar l'arrossegament.$\n$\nPots desactivar o canviar la drecera que canvia la distribució de teclat fent clic en aquest botó. Fes clic en Següent per continuar."
LangString L10N_ALTSHIFT_BUTTON   0 "&Obrir paràmetres de teclat"

LangString L10N_HOOKTIMEOUT_TITLE          0 "Alteració del registre"
LangString L10N_HOOKTIMEOUT_SUBTITLE       0 "Alteració opcional per evitar que ${APP_NAME} es tanqui inesperadament."
LangString L10N_HOOKTIMEOUT_HEADER         0 "En Windows 7, Microsoft va implementar una característica que fa que els ganxos de teclat i ratolí es desactiven si triguen a respondre. Malauradament, aquesta comprovació pot comportar-se inadequadament, especialment s'hibernes, suspens o bloquejes l'ordinador a sovint.$\n$\nSi açò passa, trobaràs que ${APP_NAME} deixa de funcionar sense avís, i hauràs de desactivar i tornar a activar manualment ${APP_NAME} per fer que torni a funcionar.$\n$\nHi ha una alteració del registre per fer que Windows esperi més abans de desactivar els ganxos, i pots activar i desactivar-la amb els botons de sota. Nota: aquesta alteració és opcional."
LangString L10N_HOOKTIMEOUT_APPLYBUTTON    0 "Activar alt&eració del registre"
LangString L10N_HOOKTIMEOUT_REVERTBUTTON   0 "&Desactivar alteració del registre"
LangString L10N_HOOKTIMEOUT_ALREADYAPPLIED 0 "L'alteració del registre ja s'ha aplicat."
LangString L10N_HOOKTIMEOUT_FOOTER         0 "El canvi tindrà efecte al pròxim inici de sessió."
