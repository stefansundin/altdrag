;AltDrag - sk-SK localization by Miroslav Miklus (miroslav.miklus@gmail.com)
;
;This program is free software: you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.

!insertmacro MUI_LANGUAGE "Slovak"
!define LANG ${LANG_SLOVAK}

LangString L10N_UPGRADE_TITLE     ${LANG} "Aplikácia už je nainštalovaná"
LangString L10N_UPGRADE_SUBTITLE  ${LANG} "Zvoľte si prosím spôsob inštalácie ${APP_NAME}."
LangString L10N_UPGRADE_HEADER    ${LANG} "${APP_NAME} už je na tomto systéme nainštalovaná. Zvoľte prosím operáciu, ktorú si želáte vykonať."
LangString L10N_UPGRADE_UPGRADE   ${LANG} "&Upgrade ${APP_NAME} na ${APP_VERSION}."
LangString L10N_UPGRADE_INI       ${LANG} "Existujúce nastavenia sa skopírujú do ${APP_NAME}-old.ini."
LangString L10N_UPGRADE_INSTALL   ${LANG} "&Inštalovať na novú lokalitu."
LangString L10N_UPGRADE_UNINSTALL ${LANG} "&Odinštalovať ${APP_NAME}."
LangString L10N_UPDATE_SECTION    ${LANG} "Pred inštaláciou skontrolovať aktualizácie"
LangString L10N_UPDATE_DIALOG     ${LANG} "Je dostupná novšia verzia.$\nUkončiť inštaláciu a prejsť na web stránku?"
LangString L10N_SHORTCUT          ${LANG} "Štart Menu Odkaz"
LangString L10N_AUTOSTART         ${LANG} "Automatický štart"
LangString L10N_AUTOSTART_HIDE    ${LANG} "Schovať lištu"

LangString L10N_ALTSHIFT_TITLE    ${LANG} "Klávesová skratka"
LangString L10N_ALTSHIFT_SUBTITLE ${LANG} "Klávesová skratka Alt + Shift už je používaná aplikáciou ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   ${LANG} "Inštalátor zistil, že klávesová skratka Alt + Shift je používaná na prepínanie rozloženia klávesnice.$\n$\nPomocou ${APP_NAME}, je možné počas presúvania okien prilepiť jedno k druhému podržaním klávesy Shift. To znamená, že budete používať kombináciu kláves Alt + Shift. Tú istú kombináciu ktorú používate na prepínanie rozloženia klávesnice. Aj keď sa ${APP_NAME} blokuje náhodné prepnutie rozloženia klávesnice, nie je to možné ak stlačíte Shift ešte pred prenosom samotného okna.$\n$\nPo stlačení tlačidla $\"Nastaviť$\" môžete túto klávesovú skratku vypnúť, či zmeniť."
LangString L10N_ALTSHIFT_BUTTON   ${LANG} "&Nastaviť"

!undef LANG
