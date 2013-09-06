;AltDrag - sk-SK localization by Miroslav Miklus (miroslav.miklus@gmail.com)
;
;This program is free software: you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.

!insertmacro MUI_LANGUAGE "Slovak"
LangString L10N_LANG ${LANG_SLOVAK} ""

LangString L10N_UPGRADE_TITLE     0 "Aplikácia už je nainštalovaná"
LangString L10N_UPGRADE_SUBTITLE  0 "Zvoľte si prosím spôsob inštalácie ${APP_NAME}."
LangString L10N_UPGRADE_HEADER    0 "${APP_NAME} už je na tomto systéme nainštalovaná. Zvoľte prosím operáciu, ktorú si želáte vykonať."
LangString L10N_UPGRADE_UPGRADE   0 "&Upgrade ${APP_NAME} na ${APP_VERSION}."
LangString L10N_UPGRADE_INI       0 "Existujúce nastavenia sa skopírujú do ${APP_NAME}-old.ini."
LangString L10N_UPGRADE_INSTALL   0 "&Inštalovať na novú lokalitu."
LangString L10N_UPGRADE_UNINSTALL 0 "&Odinštalovať ${APP_NAME}."
LangString L10N_UPDATE_DIALOG     0 "Je dostupná novšia verzia.$\nUkončiť inštaláciu a prejsť na web stránku?"

LangString L10N_ALTSHIFT_TITLE    0 "Klávesová skratka"
LangString L10N_ALTSHIFT_SUBTITLE 0 "Klávesová skratka Alt + Shift už je používaná aplikáciou ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   0 "Inštalátor zistil, že klávesová skratka Alt + Shift je používaná na prepínanie rozloženia klávesnice.$\n$\nPomocou ${APP_NAME}, je možné počas presúvania okien prilepiť jedno k druhému podržaním klávesy Shift. To znamená, že budete používať kombináciu kláves Alt + Shift. Tú istú kombináciu ktorú používate na prepínanie rozloženia klávesnice. Aj keď sa ${APP_NAME} blokuje náhodné prepnutie rozloženia klávesnice, nie je to možné ak stlačíte Shift ešte pred prenosom samotného okna.$\n$\nPo stlačení tlačidla $\"Nastaviť$\" môžete túto klávesovú skratku vypnúť, či zmeniť."
LangString L10N_ALTSHIFT_BUTTON   0 "&Nastaviť"

LangString L10N_HOOKTIMEOUT_TITLE          0 "Registry tweak"
LangString L10N_HOOKTIMEOUT_SUBTITLE       0 "Optional tweak to keep ${APP_NAME} from stopping unexpectedly."
LangString L10N_HOOKTIMEOUT_HEADER         0 "In Windows 7, Microsoft implemented a feature that stops keyboard and mouse hooks if they take too long to respond. Unfortunately, this check can erroneously misbehave, especially if you hibernate, sleep, or lock the computer a lot.$\n$\nIf this happens, you will find that ${APP_NAME} stop functioning without warning, and you have to manually disable and enable ${APP_NAME} to make it work again.$\n$\nThere is a registry tweak to make Windows wait longer before stopping hooks, which you can enable or disable by using the buttons below. Please note that this registry tweak is completely optional."
LangString L10N_HOOKTIMEOUT_APPLYBUTTON    0 "&Enable registry tweak"
LangString L10N_HOOKTIMEOUT_REVERTBUTTON   0 "&Disable registry tweak"
LangString L10N_HOOKTIMEOUT_ALREADYAPPLIED 0 "The registry tweak has already been applied."
LangString L10N_HOOKTIMEOUT_FOOTER         0 "The change will take effect on your next login."
