; AltDrag - it-IT localization by Hexaae (hexaae@gmail.com)
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.

!insertmacro MUI_LANGUAGE "Italian"
LangString L10N_LANG ${LANG_ITALIAN} ""

LangString L10N_UPGRADE_TITLE     0 "Già installato"
LangString L10N_UPGRADE_SUBTITLE  0 "Scegli come installare ${APP_NAME}."
LangString L10N_UPGRADE_HEADER    0 "${APP_NAME} è già istallato su questo sistema. Scegli cosa vuoi fare e clicca Avanti per continuare."
LangString L10N_UPGRADE_UPGRADE   0 "&Aggiorna ${APP_NAME} alla ${APP_VERSION}."
LangString L10N_UPGRADE_INI       0 "Le tue impostazioni verranno copiate come ${APP_NAME}-old.ini."
LangString L10N_UPGRADE_INSTALL   0 "&Installa in una nuova posizione."
LangString L10N_UPGRADE_UNINSTALL 0 "Disins&talla ${APP_NAME}."
LangString L10N_UPDATE_DIALOG     0 "È disponibile una nuova versione.$\nInterrompi l'installazione e vai al sito web?"

LangString L10N_ALTSHIFT_TITLE    0 "Tasto di scelta rapida"
LangString L10N_ALTSHIFT_SUBTITLE 0 "Il tasto di scelta rapida Alt + Shift è in conflitto con ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   0 "L'installer ha trovato che il tasto di scelta rapida di Windows per cambiare la lingua di input è Alt + Maiusc.$\n$\nUtilizzando ${APP_NAME}, potrai premere Maiusc durante il trascinamento di una finestra per agganciarla alle altre. Ciò significa che andrai a premere Alt + Maiusc, la stessa combinazione per cambiare le lingue di input. Sebbene ${APP_NAME} provi internamente a bloccare cambi accidentali delle lingue di input, non ce la farà se premerai Maiusc prima di iniziare a trascinare la finestra.$\n$\nPuoi disattivare o modificare il tasto di selta rapida per cambiare le lingue di input premendo questo bottone. Premi Avanti per continuare."
LangString L10N_ALTSHIFT_BUTTON   0 "A&pri impostazioni tastiera"

LangString L10N_HOOKTIMEOUT_TITLE          0 "Registry tweak"
LangString L10N_HOOKTIMEOUT_SUBTITLE       0 "Optional tweak to keep ${APP_NAME} from stopping unexpectedly."
LangString L10N_HOOKTIMEOUT_HEADER         0 "In Windows 7, Microsoft implemented a feature that stops keyboard and mouse hooks if they take too long to respond. Unfortunately, this check can erroneously misbehave, especially if you hibernate, sleep, or lock the computer a lot.$\n$\nIf this happens, you will find that ${APP_NAME} stop functioning without warning, and you have to manually disable and enable ${APP_NAME} to make it work again.$\n$\nThere is a registry tweak to make Windows wait longer before stopping hooks, which you can enable or disable by using the buttons below. Please note that this registry tweak is completely optional."
LangString L10N_HOOKTIMEOUT_APPLYBUTTON    0 "&Enable registry tweak"
LangString L10N_HOOKTIMEOUT_REVERTBUTTON   0 "&Disable registry tweak"
LangString L10N_HOOKTIMEOUT_ALREADYAPPLIED 0 "The registry tweak has already been applied."
LangString L10N_HOOKTIMEOUT_FOOTER         0 "The change will take effect on your next login."
