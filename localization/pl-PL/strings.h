/*
	AltDrag - pl-PL localization by Krystian Maksymowicz (krystian.maksymowicz@gmail.com) and Paweł 'Pawouek' Krafczyk (pkrafczyk@gmail.com)

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

struct strings pl_PL = {
 /* === translation info === */
 /* code               */ L"pl-PL",
 /* lang_english       */ L"Polish",
 /* lang               */ L"Polski",
 /* author             */ L"Krystian Maksymowicz, Paweł 'Pawouek' Krafczyk",

 /* === app === */
 /* tray_enabled       */ APP_NAME,
 /* tray_disabled      */ APP_NAME " (wyłączony)",
 { /* menu */
   /* enable           */ L"Włącz",
   /* disable          */ L"Wyłącz",
   /* hide             */ L"Ukryj ikonę",
   /* update           */ L"Aktualizacja jest dostępna!",
   /* config           */ L"Ustawienia",
   /* about            */ L"Informacje",
   /* exit             */ L"Zakończ",
 },
 { /* update */
   /* balloon          */ L"Znaleziono nową wersję!",
   /* dialog           */ L"Nowa wersja jest dostępna. Odwiedzić stronę?",
   /* nonew            */ L"Brak dostępnych aktualizacji.",
 },

 /* === config === */
 /* title              */ L"Ustawienia " APP_NAME,
 { /* tabs */
   /* general          */ L"Ogólne",
   /* input            */ L"Mysz i klawiatura",
   /* blacklist        */ L"Czarna lista",
   /* advanced         */ L"Zaawansowane",
   /* about            */ L"Informacje",
 },
 { /* general tab */
   /* box              */ L"Ustawienia ogólne",
   /* autofocus        */ L"A&ktywuj okno podczas przesuwania.\nMożesz także wcisnąć Ctrl by aktywować okna.",
   /* aero             */ L"&Naśladuj Aero Snap",
   /* inactivescroll   */ L"&Przełączaj między nieaktywnymi oknami",
   /* mdi              */ L"&MDI support",
   /* autosnap         */ L"Automatycznie przyciągaj do:",
   /* autosnap0        */ L"Wyłączone",
   /* autosnap1        */ L"Krawędzi ekranu",
   /* autosnap2        */ L"+ zew. krawędzi okien",
   /* autosnap3        */ L"+ wew. krawędzi okien",
   /* language         */ L"Język:",
   /* autostart_box    */ L"Autostart",
   /* autostart        */ L"Uruchom "APP_NAME" przy &starcie systemu",
   /* autostart_hide   */ L"&Ukryj w obszarze powiadomień",
   /* autostart_elevate*/ L"&Elevate to administrator privileges",
   /*       elevate_tip*/ L"Note that a UAC prompt will appear every time you log in, unless you disable UAC completely.",
   /* elevate          */ L"E&levate",
   /* elevated         */ L"Elevated",
   /* elevate_tip      */ L"This will create a new instance of "APP_NAME" which is running with administrator privileges. This allows "APP_NAME" to manage other programs which are running with administrator privileges.\n\nYou will have to approve a UAC prompt from Windows to allow "APP_NAME" to run with administrator privileges.",
   /* elevation_aborted*/ L"Elevation aborted.",
   /* autosave         */ L"Uwaga: Ustawienia są zapisywane i stosowane natychmiastowo!",
 },
 { /* input tab */
   { /* mouse */
     /* box            */ L"Akcje myszy",
     /* lmb            */ L"Lewy przycisk:",
     /* mmb            */ L"Środkowy przycisk:",
     /* rmb            */ L"Prawy przycisk:",
     /* mb4            */ L"Czwarty przycisk:",
     /* mb5            */ L"Piąty przycisk:",
     /* scroll         */ L"Scroll:",
     /* lowerwithmmb   */ L"&Lower windows by middle clicking on title bars",
   },
   { /* actions */
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
     /* transparency   */ L"Transparency",
   },
   { /* hotkeys */
     /* box            */ L"Skróty klawiszowe",
     /* leftalt        */ L"L&ewy Alt",
     /* rightalt       */ L"&Prawy Alt",
     /* leftwinkey     */ L"Lewy &Win",
     /* rightwinkey    */ L"Prawy W&in",
     /* leftctrl       */ L"Lewy &Ctrl",
     /* rightctrl      */ L"Prawy C&trl",
     /* more           */ L"Możesz dodać dowolny klawisz edytując plik ini!\nUżyj klawisza Shift by przyciągać okna do krawędzi innych okien.",
   },
 },
 { /* blacklist tab */
   /* box              */ L"Wyjątki",
   /* processblacklist */ L"Wyjątki procesów:",
   /* blacklist        */ L"Wyjątki okien:",
   /* snaplist         */ L"Lista przyciągania:",
   /* explanation      */ L"Dokładny opis działania wyjątków dostępny na <a href=\"http://code.google.com/p/altdrag/wiki/Blacklist\">wiki</a>!",
   /* findwindow_box   */ L"Identyfikuj okno",
   /* _explanation     */ L"Użyj powyższej funkcji do identyfikacji nazwy klasy okna by można było je dodać do jednej z powyższych list.",
 },
 { /* advanced tab */
   /* box              */ L"Ustawienia zaawansowane",
   /* hookwindows      */ L"&Włącz przyciąganie podczas zwykłego przesuwania okien.\nDziała w połączeniu z automatycznym przyciąganiem!",
   /* hookwindows_warn */ L"Nie jest to na 100% bezpieczne bo wymaga podczepiania się pod inne procesy itp.",
   /* checkonstartup   */ L"A&utomatycznie sprawdzaj dostępność aktualizacji",
   /* beta             */ L"Uw&zględniaj wersje testowe",
   /* checknow         */ L"&Sprawdź teraz",
   /* ini              */ L"Ustawienia programu "APP_NAME" są przechowywane w pliku "APP_NAME".ini. Jest kilka ustawień, które można ustawić jedynie poprzez samodzielną edycję tego pliku.",
   /* openini          */ L"Otwórz plik &ini",
 },
 { /* about tab */
   /* box              */ APP_NAME L" — Informacje",
   /* version          */ L"Wersja "APP_VERSION,
   /* author           */ L"Autorem jest Stefan Sundin",
   /* license          */ APP_NAME L" to wolne i otwarte oprogramowanie!\nNakłaniam do jego rozpowszechniania!",
   /* donate           */ L"&Dotuj",
   /*translation_credit*/ L"Translation credit",
 },

 /* === misc === */
 /* unhook_error       */ L"There was an error disabling "APP_NAME". This was most likely caused by Windows having already disabled "APP_NAME"'s hooks.\n\nIf this is the first time this has happened, you can safely ignore it and continue using "APP_NAME".\n\nIf this is happening repeatedly, you can read on the website wiki how to prevent this from happening again (look for '"APP_NAME" mysteriously stops working' on the About page).",
};
