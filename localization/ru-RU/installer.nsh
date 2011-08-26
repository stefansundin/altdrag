;AltDrag - ru-RU localization by Nick Eoneof (eoneof@gmail.com)
;
;This program is free software: you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.

!insertmacro MUI_LANGUAGE "Russian" ;English name of the language
!define LANG ${LANG_RUSSIAN}

LangString L10N_UPGRADE_TITLE     ${LANG} "Уже установлено"
LangString L10N_UPGRADE_SUBTITLE  ${LANG} "Выберите параметры установки ${APP_NAME}."
LangString L10N_UPGRADE_HEADER    ${LANG} "${APP_NAME} уже установлена на этой системе. Выберите желаемое действие и нажмите «Далее», чтобы продолжить."
LangString L10N_UPGRADE_UPGRADE   ${LANG} "&Обновить ${APP_NAME} до версии ${APP_VERSION}."
LangString L10N_UPGRADE_INI       ${LANG} "Существующие настройки будут скопированы в ${APP_NAME}-old.ini."
LangString L10N_UPGRADE_INSTALL   ${LANG} "Установить в другом &месте."
LangString L10N_UPGRADE_UNINSTALL ${LANG} "У&далить ${APP_NAME}."
LangString L10N_UPDATE_SECTION    ${LANG} "Проверить наличие обновлений перед установкой"
LangString L10N_UPDATE_DIALOG     ${LANG} "Доступна новая версия.$\nПрервать установку и перейти на сайт?"
LangString L10N_SHORTCUT          ${LANG} "Ярлык в меню Пуск"
LangString L10N_AUTOSTART         ${LANG} "Автозагрузка"
LangString L10N_AUTOSTART_HIDE    ${LANG} "Спрятать иконку в трее"

LangString L10N_ALTSHIFT_TITLE    ${LANG} "Горячая клавиша"
LangString L10N_ALTSHIFT_SUBTITLE ${LANG} "Сочетание Alt + Shift конфликтует с ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   ${LANG} "Установщик обнаружил, что сочетание Alt + Shift переключает раскладку клавиатуры.$\n$\nТак как ${APP_NAME}, использует то же сочетание для перетаскивания окон с пристыковкой, она может блокировать случайное переключение раскладки. Блокировка не удастся, если нажать Shift до начала перетаскивания.$\n$\nВы можете отключить или изменить сочетание, переключающее раскладку, нажав эту кнопку. Нажмите «Далее», чтобы продолжить."
LangString L10N_ALTSHIFT_BUTTON   ${LANG} "Настройки &клавиатуры"

!undef LANG
