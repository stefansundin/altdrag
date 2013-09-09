 ; AltDrag - ru-RU localization by Nick Eoneof (eoneof@gmail.com)
; and Dmitry Trubin (dtruebin@gmail.com)
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
LangString L10N_UPGRADE_INSTALL   0 "Установить в другое &место."
LangString L10N_UPGRADE_UNINSTALL 0 "У&далить ${APP_NAME}."
LangString L10N_UPDATE_DIALOG     0 "Доступна новая версия.$\nПрервать установку и перейти на сайт?"

LangString L10N_ALTSHIFT_TITLE    0 "Горячая клавиша"
LangString L10N_ALTSHIFT_SUBTITLE 0 "Сочетание Alt + Shift конфликтует с ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   0 "Установщик обнаружил, что сочетание Alt + Shift переключает раскладку клавиатуры.$\n$\nТак как ${APP_NAME}, использует то же сочетание для перетаскивания окон с пристыковкой, она может блокировать случайное переключение раскладки. Блокировка не удастся, если нажать Shift до начала перетаскивания.$\n$\nВы можете отключить или изменить сочетание, переключающее раскладку, нажав эту кнопку. Нажмите «Далее», чтобы продолжить."
LangString L10N_ALTSHIFT_BUTTON   0 "Настройки &клавиатуры"

LangString L10N_HOOKTIMEOUT_TITLE          0 "Твик реестра"
LangString L10N_HOOKTIMEOUT_SUBTITLE       0 "Твик, позволяющий предотвратить внезапную остановку ${APP_NAME}."
LangString L10N_HOOKTIMEOUT_HEADER         0 "В Windows 7, Microsoft реализовали функцию, которая останавливает перехват нажатий клавиатуры и мыши, если они занимают слишком много времени. К сожалению, очень часто эта функция срабатывает ошибочно (особенно, если вы отправили компьютер в гибернацию, режим сна или заблокировали систему).$\n$\nЕсли это произошло, ${APP_NAME} прекратит функционировать без предупреждения и для возобновления работы вам будет необходимо вручную выключить и включить ${APP_NAME}. $\n$\nСуществует твик реестра, увеличивающий время ожидания перед остановкой перехвата. Его можно включить и выключить кнопками ниже. Имейте ввиду, что этот твик не является обязательным."
LangString L10N_HOOKTIMEOUT_APPLYBUTTON    0 "&Включить твик реестра"
LangString L10N_HOOKTIMEOUT_REVERTBUTTON   0 "&Отключить твик реестра"
LangString L10N_HOOKTIMEOUT_ALREADYAPPLIED 0 "Твик реестра уже включен."
LangString L10N_HOOKTIMEOUT_FOOTER         0 "Изменения вступят в силу при следующем входе в систему."
