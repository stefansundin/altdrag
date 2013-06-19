/*
	AltDrag - ru-RU localization by Neek Eoneof (eoneof@gmail.com) and Dmitry Trubin (dtruebin@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

struct strings ru_RU = {
 /* === translation info === */
 /* code               */ L"ru-RU",
 /* lang_english       */ L"Russian",
 /* lang               */ L"Русский",
 /* author             */ L"Neek Eoneof, Dmitry Trubin",
 
 /* === app === */
 /* tray_enabled       */ APP_NAME,
 /* tray_disabled      */ APP_NAME " (выключено)",
 { /* menu */
   /* enable           */ L"Включить",
   /* disable          */ L"Выключить",
   /* hide             */ L"Спрятать иконку",
   /* update           */ L"Доступно обновление!",
   /* config           */ L"Настройки",
   /* about            */ L"О программе",
   /* exit             */ L"Выход",
 },
 { /* update */
   /* balloon          */ L"Обнаружена новая версия!",
   /* dialog           */ L"Доступна новая версия. Прейти на сайт программы?",
   /* nonew            */ L"Нет доступных обновлений.",
 },
 
 /* === config === */
 /* title              */ L"Настройки " APP_NAME,
 { /* tabs */
   /* general          */ L"Основные",
   /* input            */ L"Мышь и клавиатура",
   /* blacklist        */ L"Черный список",
   /* advanced         */ L"Дополнительные",
   /* about            */ L"О программе",
 },
 { /* general tab */
   /* box              */ L"Основные настройки",
   /* autofocus        */ L"&Активировать окно при перетаскивании.\nИначе, с зажатой клавишей Ctrl.",
   /* aero             */ L"&Имитировать Aero Snap",
   /* inactivescroll   */ L"&Прокручивать содержимое неактивных окон",
   /* autosnap         */ L"Автоматическая стыковка:",
   /* autosnap0        */ L"Отключена",
   /* autosnap1        */ L"К краям экрана",
   /* autosnap2        */ L"+ к окнам снаружи",
   /* autosnap3        */ L"+ к окнам изнутри",
   /* language         */ L"Язык:",
   /* autostart_box    */ L"Автозагрузка",
   /* autostart        */ L"&Запускать "APP_NAME" при загрузке системы",
   /* autostart_hide   */ L"Прятать иконку в &области уведомлений",
   /* autostart_elevate*/ L"&Дать программе полномочия администратора",
   /*       elevate_tip*/ L"Подтверждение UAC будет запрашиваться при каждом входе в систему, если не отключить UAC полностью.",
   /* elevate          */ L"П&овысить",
   /* elevated         */ L"Повышено",
   /* elevate_tip      */ L"Новый экземпляр "APP_NAME" будет запущен от имени администратора. Это позволит программе "APP_NAME" управлять другими приложениями, запущенными от имени администратора.\n\nНеобходимо выполнить подтверждение UAC, чтобы разрешить запуск "APP_NAME" с полномочиями администратора.",
   /* elevation_aborted*/ L"Повышение полномочий отменено.",
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
     /* scroll        `*/ L"Колесо прокрутки:",
     /* more          `*/ L"Запрос на новое действие мыши можно оставить <a href=\"http://code.google.com/p/altdrag/issues/list\">на сайте</a>!",
   },
   { /* actions `*/
     /* move           */ L"Переместить",
     /* resize         */ L"Размер",
     /* close          */ L"Закрыть",
     /* minimize       */ L"Свернуть",
     /* lower          */ L"На задний план",
     /* alwaysontop    */ L"Поверх всех окон",
     /* center         */ L"Установить по центру",
     /* nothing        */ L"Ничего",
     /* alttab         */ L"Alt+Tab",
     /* volume         */ L"Громкость",
     /* transparency   */ L"Transparency",
   },
   { /* hotkeys */
     /* box            */ L"Горячие клавиши",
     /* leftalt        */ L"Левый &Alt",
     /* rightalt       */ L"Правый A&lt",
     /* leftwinkey     */ L"Левый &Win",
     /* rightwinkey    */ L"Правый W&in",
     /* leftctrl       */ L"Левый &Ctrl",
     /* rightctrl      */ L"Правый C&trl",
     /* more           */ L"Вы можете добавить любую клавишу, изменив ini-файл.\nИспользуйте Shift для стыковки окон друг к другу.",
   },
 },
 { /* blacklist tab */
   /* box              */ L"Настройки черного списка",
   /* processblacklist */ L"Черный список процессов:",
   /* blacklist        */ L"Черный список:",
   /* snaplist         */ L"Окна для стыковки:",
   /* explanation      */ L"Прочтите <a href=\"http://code.google.com/p/altdrag/wiki/Blacklist\">вики-статью</a> на сайте с подробным описанием принципа работы черного списка!",
   /* findwindow_box   */ L"Определить окно",
   /* _explanation     */ L"Используйте эту функцию, чтобы определить имя класса окна и добавить его в списки выше.",
 },
 { /* advanced tab */
   /* box              */ L"Дополнительные настройки",
   /* hookwindows      */ L"Включить стыковку при перетаскавании окна за заголовок.\nРаботает в связке с автоматической стыковкой!",
   /* hookwindows_warn`*/ L"Имейте в виду, что функция не на 100% безопасна, так как приходится внедряться в другие процессы и т.д. Это может повлиять, например, на игры с системой защитой от жульничества.\n\nВключить стыковку?",
   /* checkonstartup   */ L"&Автоматически проверять наличие обновлений",
   /* beta             */ L"Проверять наличие &бета-версий",
   /* checknow         */ L"&Проверить",
   /* ini              */ "Настройки программы хранятся в файле "APP_NAME".ini. Некоторые настройки можно изменить только вручную через этот файл.",
   /* openini          */ L"&Открыть .ini",
 },
 { /* about tab */
   /* box              */ L"О программе "APP_NAME,
   /* version          */ L"Версия "APP_VERSION,
   /* author           */ L"Разработал Stefan Sundin",
   /* license          */ APP_NAME L" — бесплатная программа с открытым исходным кодом. Расскажите о ней друзьям!",
   /* donate           */ L"&Помочь",
   /*translation_credit*/ L"Translation credit",
 },
};
