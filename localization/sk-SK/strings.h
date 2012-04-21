/*
	AltDrag - sk-SK localization by Miroslav Miklus (miroslav.miklus@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

struct strings sk_SK = {
 /* tray_enabled       */ APP_NAME,
 /* tray_disabled      */ APP_NAME " (vypnutý)",
 { /* menu */
   /* enable           */ L"Zapnúť",
   /* disable          */ L"Vypnúť",
   /* hide             */ L"Odstrániť z lišty",
   /* update           */ L"Aktualizácia dostupná!",
   /* config           */ L"Otvoriť nastavenia",
   /* about            */ L"O aplikácii",
   /* exit             */ L"Koniec",
 },
 { /* update */
   /* balloon          */ L"Dostupná nová verzia!",
   /* dialog           */ L"Je dostupná nová verzia. Prejsť na webstránku?",
   /* nonew            */ L"Nie sú dostupné žiadne aktualizácie.",
 },
 
 /* === config === */
 /* lang               */ L"Slovenčina",
 /* title              */ APP_NAME L" Konfigurácia",
 { /* tabs */
   /* general          */ L"Všeobecné",
   /* input            */ L"Myš a klávesnica",
   /* this should be traslated like "Výnimky" (Exceptions), but I think Blacklist is also good :-D */
   /* blacklist        */ L"Blacklist",
   /* advanced         */ L"Pokročilé",
   /* about            */ L"O aplikácii",
 },
 { /* general tab */
   /* box              */ L"Všeobecné nastavenia",
   /* autofocus        */ L"&Označiť okno počas premiesňovania.\nOkno je možné označiť stlačením klávesy Ctrl.",
   /* aero             */ L"Mimic &Aero Snap",
   /* inactivescroll   */ L"&Skrolovanie neaktívnych okien",
   /* autosnap         */ L"Automatické uchytenie na:",
   /* autosnap0        */ L"Vypnuté",
   /* autosnap1        */ L"Na okraje obrazovky",
   /* autosnap2        */ L"+ mimo okien",
   /* autosnap3        */ L"+ vnútri okien",
   /* language         */ L"Jazyk:",
   /* helptranslate    */ L"Pomôcť s prekladom",
   /* autostart_box    */ L"Autoštart",
   /* autostart        */ L"Š&tart "APP_NAME" po prihlásení",
   /* autostart_hide   */ L"&Schovať lištu",
   /* autosave         */ L"Upozornenie: Nastavenia sú ukladané a aplikované okamžite!",
 },
 { /* input tab */
   { /* mouse */
     /* box            */ L"Akcie myši",
     /* lmb            */ L"Ľavé tlačidlo myši:",
     /* mmb            */ L"Stredné tlačidlo myši:",
     /* rmb            */ L"Pravé tlačidlo myši:",
     /* mb4            */ L"4. tlačidlo myši:",
     /* mb5            */ L"5. tlačidlo myši:",
     /* scroll        `*/ L"Scroll:",
     /* more          `*/ L"Ak máte skvelý nápad na ďalšie akcie myšou, dajte nám vedieť prostredníctvom <a href=\"http://code.google.com/p/altdrag/issues/list\">web stránky</a>.",
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
     /* box            */ L"Klávesové skratky",
     /* leftalt        */ L"Left &Alt",
     /* rightalt       */ L"Right A&lt",
     /* leftwinkey     */ L"Left &Winkey",
     /* rightwinkey    */ L"Right W&inkey",
     /* leftctrl       */ L"Left &Ctrl",
     /* rightctrl      */ L"Right C&trl",
     /* more           */ L"Ďalšie klávesové skratky môžete zadefinovať v .ini súbore! Prilepenie okien docielite podržaním klávesy Shift.",
   },
 },
 { /* blacklist tab */
   /* box              */ L"Nastavenie blacklistu",
   /* processblacklist */ L"Blacklist procesov:",
   /* blacklist        */ L"Blacklist:",
   /* snaplist         */ L"Snaplist:",
   /* explanation      */ L"Princíp fungovania blacklistu je možné nájsť na <a href=\"http://code.google.com/p/altdrag/wiki/Blacklist\">wiki</a>",
   /* findwindow_box   */ L"Identifikácia okna",
   /* _explanation     */ L"Túto funkciu je možné využiť pre určenie \"classname\" okna, použiteľného pre funkcionality blacklistu, či prichytávania okien v rámci snaplistu.",
 },
 { /* advanced tab */
   /* box              */ L"Pokročilé nastavenia",
   /* hookwindows      */ L"&Povoliť prichytávanie počas presunu okien.\nPracuje pri zapnutom automatickom prichytávaní okien!",
   /* hookwindows_warn`*/ L"Táto funkcia nie je 100% bezpečná, nakoľko je potrebný zásah do iných procesov.",
   /* checkonstartup   */ L"Automatická kontrola akt&ualizácii",
   /* beta             */ L"Kontrolovať aj dostupnosť &beta verzii",
   /* checknow         */ L"Skontrolovať aktualizácie",
   /* ini              */ APP_NAME L" nastavenia sú ukladané do súboru "APP_NAME".ini. Niektoré nastavenia je možné upraviť len manuálnou editáciou tohto súboru.",
   /* openini          */ L"Otvoriť &ini súbor",
 },
 { /* about tab */
   /* box              */ L"O aplikácií "APP_NAME,
   /* version          */ L"Verzia "APP_VERSION,
   /* author           */ L"Autor Stefan Sundin",
   /* license          */ APP_NAME L" je voľne dostupný softvér s otvoreným zdrojovým kódom. Šírte ho prosím ďalej.",
   /* donations_box    */ L"Príspevok",
   /* donations_plea   */ L"Ak vám pripadá "APP_NAME" užitočný, zvážte finančný príspevok. Ľubovoľná suma poteší.",
   /* donate           */ L"&Prispej",
 },
};
