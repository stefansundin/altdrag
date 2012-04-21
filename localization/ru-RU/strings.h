/*
	AltDrag - ru-RU localization by Neek Eoneof (eoneof@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

struct strings ru_RU = {
 /* tray_enabled       */ APP_NAME,
 /* tray_disabled      */ APP_NAME " (выключено)",
 { /* menu */
   /* enable           */ L"Включить",
   /* disable          */ L"Выключить",
   /* hide             */ L"Спрятать иконку",
   /* update           */ L"Обновление недоступно!",
   /* config           */ L"Открыть настройки",
   /* about            */ L"О программе",
   /* exit             */ L"Закрыть",
 },
 { /* update */
   /* balloon          */ L"Обнаружена новая версия!",
   /* dialog           */ L"Доступна новая версия. Прейти на сайт программы?",
   /* nonew            */ L"Нет доступных обновлений.",
 },
 
 /* === config === */
 /* lang               */ L"Русский",
 /* title              */ APP_NAME L" Настройки",
 { /* tabs */
   /* general          */ L"Основные",
   /* input            */ L"Мышь и клавиатура",
   /* blacklist        */ L"Черный список",
   /* advanced         */ L"Дополнительные",
   /* about            */ L"О программе",
 },
 { /* general tab */
   /* box              */ L"Основные настройки",
   /* autofocus        */ L"&Фокусироваться на окне при перетаскивании.\nИначе, с зажатой клавишей Ctrl.",
   /* aero             */ L"&Имитировать Aero Snap",
   /* inactivescroll   */ L"&Прокручивать содержимое неактивных окон",
   /* autosnap         */ L"Пристыковывать:",
   /* autosnap0        */ L"Отключено",
   /* autosnap1        */ L"К краям экрана",
   /* autosnap2        */ L"+ снаружи окон",
   /* autosnap3        */ L"+ внутри окон",
   /* language         */ L"Язык:",
   /* helptranslate    */ L"Помочь с переводом",
   /* autostart_box    */ L"Автозагрузка",
   /* autostart        */ L"&Запускать "APP_NAME" при загрузке системы",
   /* autostart_hide   */ L"Прятать иконку в &трее",
   /* autosave         */ L"Внимание, настройки применяются немедленно!",
 },
 { /* input tab */
   { /* mouse */
     /* box            */ L"Действия мыши",
     /* lmb            */ L"Левая кнопка:",
     /* mmb            */ L"Средняя кнопка:",
     /* rmb            */ L"Правая кнопка:",
     /* mb4            */ L"Кнопка мыши 4:",
     /* mb5            */ L"Кнопка мыши 5:",
     /* scroll        `*/ L"Scroll:",
     /* more          `*/ L"Запросы на новые функции для мыши отправляйте <a href=\"http://code.google.com/p/altdrag/issues/list\">на сайт.</a>!",
   },
   { /* actions `*/
     /* move           */ L"Move",
     /* resize         */ L"Resize",
     /* close          */ L"Close",
     /* minimize       */ L"Minimize",
     /* lower          */ L"Lower",
     /* alwaysontop    */ L"AlwaysOnTop",
     /* center         */ L"Center",
     /* nothing        */ L"Nothing",
     /* alttab         */ L"Alt+Tab",
     /* volume         */ L"Volume",
   },
   { /* hotkeys */
     /* box            */ L"Горячие клавиши",
     /* leftalt        */ L"Левый &Alt",
     /* rightalt       */ L"Правый A&lt",
     /* leftwinkey     */ L"Левый &Winkey",
     /* rightwinkey    */ L"Правый W&inkey",
     /* leftctrl       */ L"Левый &Ctrl",
     /* rightctrl      */ L"Правый C&trl",
     /* more           */ L"Вы можете добавить любую клавишу, изменив ini-файл!\nИспользуйте клавишу Shift для пристыковки окон.",
   },
 },
 { /* blacklist tab */
   /* box              */ L"Настройки черного списка",
   /* processblacklist */ L"Черный список процессов:",
   /* blacklist        */ L"Черный список:",
   /* snaplist         */ L"Окна без стыковки",
   /* explanation      */ L"Прочтите <a href=\"http://code.google.com/p/altdrag/wiki/Blacklist\">вики-статью</a> на сайте, с подробным описанием принципа работы черного списка!",
   /* findwindow_box   */ L"Идентифицировать окно",
   /* _explanation     */ L"Используйте эту функцию, чтобы идентифицировать класс окна, и добавить его в списки выше.",
 },
 { /* advanced tab */
   /* box              */ L"Дополниельные настройки",
   /* hookwindows      */ L"Включить стыковку при обычном перемещении окна.\nРаботает только при включенной автоматической стыковке.",
   /* hookwindows_warn`*/ L"Имейте в виду, что это не на 100% безопасно, так как перехватывает другой процесс.",
   /* checkonstartup   */ L"Автоматически проверять наличие обновлений",
   /* beta             */ L"Проверять наличие &бета-версий",
   /* checknow         */ L"Проверить обновления",
   /* ini              */ "Настройки программы хранятся в файле "APP_NAME".ini. В нем вы можете отредактировать вручную всего несколько настроек.",
   /* openini          */ L"Открыть *.&ini",
 },
 { /* about tab */
   /* box              */ L"О программе "APP_NAME,
   /* version          */ L"Версия "APP_VERSION,
   /* author           */ L"Разработчик Stefan Sundin",
   /* license          */ APP_NAME L" — бесплатная программа с открытым исходным кодом!",
   /* donations_box    */ L"Поддержка",
   /* donations_plea   */ L"Если "APP_NAME" оказаласть вам полезной, помогите разработке проекта финансово.",
   /* donate           */ L"&Помочь",
 },
};
