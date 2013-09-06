; AltDrag - pl-PL localization by Krystian Maksymowicz (krystian.maksymowicz@gmail.com) and Paweł 'Pawouek' Krafczyk (pkrafczyk@gmail.com)
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.

!insertmacro MUI_LANGUAGE "Polish"
LangString L10N_LANG ${LANG_POLISH} ""

LangString L10N_UPGRADE_TITLE     0 "Już zainstalowany"
LangString L10N_UPGRADE_SUBTITLE  0 "Wybierz sposób instalacji ${APP_NAME}."
LangString L10N_UPGRADE_HEADER    0 "${APP_NAME} już jest zainstalowany. Wybierz jedną z poniższych czynności i klinknij Dalej by kontynuować."
LangString L10N_UPGRADE_UPGRADE   0 "&Uaktualniono ${APP_NAME} do wersji ${APP_VERSION}."
LangString L10N_UPGRADE_INI       0 "Twoje aktualne ustawienia będą skopiowane do ${APP_NAME}-stary.ini."
LangString L10N_UPGRADE_INSTALL   0 "&Zainstaluj w innej lokacji."
LangString L10N_UPGRADE_UNINSTALL 0 "&Odinstaluj ${APP_NAME}."
LangString L10N_UPDATE_DIALOG     0 "Wykryto nowszą wersję.$\nPrzerwać instalację i odwiedzić stronę programu?"

LangString L10N_ALTSHIFT_TITLE    0 "Skrót klawiszowy"
LangString L10N_ALTSHIFT_SUBTITLE 0 "Skrót klawiszowy Alt + Shift jest w konflikcie z ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   0 "Instalator wykrył, że system korzysta z kombinacji klawiszy Alt + Shift przy zmianie układu klawiatury.$\n$\nPrzy korzystaniu z ${APP_NAME} możesz przytrzymać Shift podczas przeciągania okna by dokować je do krawędzi innego okna. Oznacza to, że prawdopodobne iż wciśniesz Alt + Shift, co również spowoduje zmianę układu klawiatury. Mimo iż ${APP_NAME} stara się blokować przypadkowe zmiany układu klawiatury to nie będzie to możliwe, gdy klawisz Shift będzie wciśnięty przed rozpoczęciem przeciągania okna.$\n$\nMożesz wyłączyć lub zmienić skrót klawiszowy do zmiany układu klawiatury wciskając ten przycisk. Wciśnij Dalej by kontynuować."
LangString L10N_ALTSHIFT_BUTTON   0 "&Otwórz ustawienia klawiszy"

LangString L10N_HOOKTIMEOUT_TITLE          0 "Registry tweak"
LangString L10N_HOOKTIMEOUT_SUBTITLE       0 "Optional tweak to keep ${APP_NAME} from stopping unexpectedly."
LangString L10N_HOOKTIMEOUT_HEADER         0 "In Windows 7, Microsoft implemented a feature that stops keyboard and mouse hooks if they take too long to respond. Unfortunately, this check can erroneously misbehave, especially if you hibernate, sleep, or lock the computer a lot.$\n$\nIf this happens, you will find that ${APP_NAME} stop functioning without warning, and you have to manually disable and enable ${APP_NAME} to make it work again.$\n$\nThere is a registry tweak to make Windows wait longer before stopping hooks, which you can enable or disable by using the buttons below. Please note that this registry tweak is completely optional."
LangString L10N_HOOKTIMEOUT_APPLYBUTTON    0 "&Enable registry tweak"
LangString L10N_HOOKTIMEOUT_REVERTBUTTON   0 "&Disable registry tweak"
LangString L10N_HOOKTIMEOUT_ALREADYAPPLIED 0 "The registry tweak has already been applied."
LangString L10N_HOOKTIMEOUT_FOOTER         0 "The change will take effect on your next login."
