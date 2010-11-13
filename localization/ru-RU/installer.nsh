;AltDrag - ru-RU локализация, сделана Gluek (mrgluek@gmail.com)
;
;Эта программа является свободным программным обеспечением, вы можете распространять ее и / или модифицировать
;в соответствии с условиями GNU General Public License, опубликованной
;Free Software Foundation, пользуясь либо версию 3 лицензии, либо
;(по вашему выбору) любой более поздней версиий.

!insertmacro MUI_LANGUAGE "Russian"
!define LANG ${LANG_RUSSIAN}

LangString L10N_UPGRADE_TITLE     ${LANG} "Уже установлена"
LangString L10N_UPGRADE_SUBTITLE  ${LANG} "Выберите, как вы хотите установить ${APP_NAME}."
LangString L10N_UPGRADE_HEADER    ${LANG} "${APP_NAME} уже установлена в системе. Выберите операцию, которую вы хотите осуществить и нажмите Далее (Next) для продолжения."
LangString L10N_UPGRADE_UPGRADE   ${LANG} "&Обновить ${APP_NAME} до версии ${APP_VERSION}."
LangString L10N_UPGRADE_INI       ${LANG} "Ваши существующие настройки будут скопированы в ${APP_NAME}-old.ini."
LangString L10N_UPGRADE_INSTALL   ${LANG} "&Установить в новое место."
LangString L10N_UPGRADE_UNINSTALL ${LANG} "&Uninstall ${APP_NAME}."
LangString L10N_UPDATE_SECTION    ${LANG} "Проверить обновления перед установкой"
LangString L10N_UPDATE_DIALOG     ${LANG} "Доступна новая версия.$\nПрервать установку и перейти на сайт?"
LangString L10N_RUNNING           ${LANG} "${APP_NAME} запущена. Закрыть?"
LangString L10N_RUNNING_UNINSTALL ${LANG} "Если вы выберите Нет (No), ${APP_NAME} будет полностью удалена при следующей перезагрузке."
LangString L10N_SHORTCUT          ${LANG} "Ярлык в меню Пуск"
LangString L10N_AUTOSTART         ${LANG} "Автозапуск"
LangString L10N_AUTOSTART_HIDE    ${LANG} "Скрыть иконку"

LangString L10N_ALTSHIFT_TITLE    ${LANG} "Сочетание клавиш"
LangString L10N_ALTSHIFT_SUBTITLE ${LANG} "Сочетание Alt + Shift конфликтует с программой ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   ${LANG} "Программа установки обнаружила, что вы используете более одной раскладки клавиатуры, и сочетание клавиш для переключения раскладки Alt + Shift.$\n$\nПри использовании ${APP_NAME}, вы можете удерживать Shift, перемещая окна, чтобы они прилипали к другим. Это значит, что вы скорее всего нажмете Alt + Shift, то же сочетание клавиш, что и переключает раскладку. Пока ${APP_NAME} самостоятельно пытается блокировать случайное переключение раскладки, это может быть не всегда успешно.$\n$\nВы можете отключить это или изменить сочетание клавиш, переключающее раскладку, нажав на эту кнопку. Нажмите Далее (Next) для продолжения."
LangString L10N_ALTSHIFT_BUTTON   ${LANG} "&Настройки клавиатуры"

!undef LANG
