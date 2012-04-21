/*
	AltDrag - pl-PL localization by Krystian Maksymowicz (krystian.maksymowicz@gmail.com) and Paweł 'Pawouek' Krafczyk (pkrafczyk@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

struct strings pl_PL = {
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
 /* lang               */ L"Polish",
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
   /* autofocus        */ L"&Aktywuj okno podczas przesuwania.\nMożesz także wcisnąć Ctrl by aktywować okna.",
   /* aero             */ L"&Naśladuj Aero Snap",
   /* inactivescroll   */ L"&Przełączaj między nieaktywnymi oknami",
   /* autosnap         */ L"Automatycznie przyciągaj do:",
   /* autosnap0        */ L"Wyłączone",
   /* autosnap1        */ L"Krawędzi ekranu",
   /* autosnap2        */ L"+ zew. krawędzi okien",
   /* autosnap3        */ L"+ wew. krawędzi okien",
   /* language         */ L"Język:",
   /* helptranslate    */ L"Jak pomóc w tłumaczeniu?",
   /* autostart_box    */ L"Autostart",
   /* autostart        */ L"Uruchom "APP_NAME" przy &starcie systemu",
   /* autostart_hide   */ L"&Ukryj w obszarze powiadomień",
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
     /* scroll        `*/ L"Scroll:",
     /* more          `*/ L"Jeżeli masz pomysł na akcję myszy to nie krępuj się zgłosić ją na <a href=\"http://code.google.com/p/altdrag/issues/list\">stronie programu</a>!",
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
     /* box            */ L"Skróty klawiszowe",
     /* leftalt        */ L"Lewy &Alt",
     /* rightalt       */ L"Prawy A&lt",
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
   /* hookwindows_warn`*/ L"Nie jest to na 100% bezpieczne bo wymaga podczepiania się pod inne procesy itp.",
   /* checkonstartup   */ L"&Automatycznie sprawdzaj dostępność aktualizacji",
   /* beta             */ L"&Uwzględniaj wersje testowe",
   /* checknow         */ L"&Sprawdź teraz",
   /* ini              */ L"Ustawienia programu "APP_NAME" są przechowywane w pliku "APP_NAME".ini. Jest kilka ustawień, które można ustawić jedynie poprzez samodzielną edycję tego pliku.",
   /* openini          */ L"Otwórz plik &ini",
 },
 { /* about tab */
   /* box              */ APP_NAME L" — Informacje",
   /* version          */ L"Wersja "APP_VERSION,
   /* author           */ L"Autorem jest Stefan Sundin",
   /* license          */ APP_NAME L" to wolne i otwarte oprogramowanie!\nNakłaniam do jego rozpowszechniania!",
   /* donations_box    */ L"Dotacje",
   /* donations_plea   */ L"Jeżeli uważasz, że "APP_NAME" jest programem przydatnym, rozważ proszę złożenie dotacji. Nie proszę o wiele, każda kwota będzie mile widziana.",
   /* donate           */ L"&Dotuj",
 },
};
