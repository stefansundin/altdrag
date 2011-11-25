;AltDrag - pl-PL localization by Krystian Maksymowicz (krystian.maksymowicz@gmail.com) and Paweł 'Pawouek' Krafczyk (pkrafczyk@gmail.com)
;
;This program is free software: you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.

!insertmacro MUI_LANGUAGE "Polish"
!define LANG ${LANG_POLISH}

LangString L10N_UPGRADE_TITLE     ${LANG} "Już zainstalowany"
LangString L10N_UPGRADE_SUBTITLE  ${LANG} "Wybierz sposób instalacji ${APP_NAME}."
LangString L10N_UPGRADE_HEADER    ${LANG} "${APP_NAME} już jest zainstalowany. Wybierz jedną z poniższych czynności i klinknij Dalej by kontynuować."
LangString L10N_UPGRADE_UPGRADE   ${LANG} "&Uaktualniono ${APP_NAME} do wersji ${APP_VERSION}."
LangString L10N_UPGRADE_INI       ${LANG} "Twoje aktualne ustawienia będą skopiowane do ${APP_NAME}-stary.ini."
LangString L10N_UPGRADE_INSTALL   ${LANG} "&Zainstaluj w innej lokacji."
LangString L10N_UPGRADE_UNINSTALL ${LANG} "&Odinstaluj ${APP_NAME}."
LangString L10N_UPDATE_SECTION    ${LANG} "Szukaj uaktualnienia przed instalacją"
LangString L10N_UPDATE_DIALOG     ${LANG} "Wykryto nowszą wersję.$\nPrzerwać instalację i odwiedzić stronę programu?"
LangString L10N_SHORTCUT          ${LANG} "Skrót menu start"
LangString L10N_AUTOSTART         ${LANG} "Autostart"
LangString L10N_AUTOSTART_HIDE    ${LANG} "Ukryj ikonę obszaru powiadomienia"

LangString L10N_ALTSHIFT_TITLE    ${LANG} "Skrót klawiszowy"
LangString L10N_ALTSHIFT_SUBTITLE ${LANG} "Skrót klawiszowy Alt + Shift jest w konflikcie z ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   ${LANG} "Instalator wykrył, że system korzysta z kombinacji klawiszy Alt + Shift przy zmianie układu klawiatury.$\n$\nPrzy korzystaniu z ${APP_NAME} możesz przytrzymać Shift podczas przeciągania okna by dokować je do krawędzi innego okna. Oznacza to, że prawdopodobne iż wciśniesz Alt + Shift, co również spowoduje zmianę układu klawiatury. Mimo iż ${APP_NAME} stara się blokować przypadkowe zmiany układu klawiatury to nie będzie to możliwe, gdy klawisz Shift będzie wciśnięty przed rozpoczęciem przeciągania okna.$\n$\nMożesz wyłączyć lub zmienić skrót klawiszowy do zmiany układu klawiatury wciskając ten przycisk. Wciśnij Dalej by kontynuować."
LangString L10N_ALTSHIFT_BUTTON   ${LANG} "&Otwórz ustawienia klawiszy"

!undef LANG
