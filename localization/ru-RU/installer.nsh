; AltDrag - ru-RU localization by Nick Eoneof (eoneof@gmail.com)
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.

!insertmacro MUI_LANGUAGE "Russian"
LangString L10N_LANG ${LANG_RUSSIAN} ""

LangString L10N_UPGRADE_TITLE     0 "Уже установлено"
LangString L10N_UPGRADE_SUBTITLE  0 "Выберите параметры установки ${APP_NAME}."
LangString L10N_UPGRADE_HEADER    0 "${APP_NAME} уже установлена на этой системе. Выберите желаемое действие и нажмите «Далее», чтобы продолжить."
LangString L10N_UPGRADE_UPGRADE   0 "&Обновить ${APP_NAME} до версии ${APP_VERSION}."
LangString L10N_UPGRADE_INI       0 "Существующие настройки будут скопированы в ${APP_NAME}-old.ini."
LangString L10N_UPGRADE_INSTALL   0 "Установить в другом &месте."
LangString L10N_UPGRADE_UNINSTALL 0 "У&далить ${APP_NAME}."
LangString L10N_UPDATE_DIALOG     0 "Доступна новая версия.$\nПрервать установку и перейти на сайт?"

LangString L10N_ALTSHIFT_TITLE    0 "Горячая клавиша"
LangString L10N_ALTSHIFT_SUBTITLE 0 "Сочетание Alt + Shift конфликтует с ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   0 "Установщик обнаружил, что сочетание Alt + Shift переключает раскладку клавиатуры.$\n$\nТак как ${APP_NAME}, использует то же сочетание для перетаскивания окон с пристыковкой, она может блокировать случайное переключение раскладки. Блокировка не удастся, если нажать Shift до начала перетаскивания.$\n$\nВы можете отключить или изменить сочетание, переключающее раскладку, нажав эту кнопку. Нажмите «Далее», чтобы продолжить."
LangString L10N_ALTSHIFT_BUTTON   0 "Настройки &клавиатуры"

LangString L10N_HOOKTIMEOUT_TITLE          0 "Registry tweak"
LangString L10N_HOOKTIMEOUT_SUBTITLE       0 "Optional tweak to keep ${APP_NAME} from stopping unexpectedly."
LangString L10N_HOOKTIMEOUT_HEADER         0 "In Windows 7, Microsoft implemented a feature that stops keyboard and mouse hooks if they take too long to respond. Unfortunately, this check can erroneously misbehave, especially if you hibernate, sleep, or lock the computer a lot.$\n$\nIf this happens, you will find that ${APP_NAME} stop functioning without warning, and you have to manually disable and enable ${APP_NAME} to make it work again.$\n$\nThere is a registry tweak to make Windows wait longer before stopping hooks, which you can enable or disable by using the buttons below. Please note that this registry tweak is completely optional."
LangString L10N_HOOKTIMEOUT_APPLYBUTTON    0 "&Enable registry tweak"
LangString L10N_HOOKTIMEOUT_REVERTBUTTON   0 "&Disable registry tweak"
LangString L10N_HOOKTIMEOUT_ALREADYAPPLIED 0 "The registry tweak has already been applied."
LangString L10N_HOOKTIMEOUT_FOOTER         0 "The change will take effect on your next login."
