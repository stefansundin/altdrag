/*
	AltDrag - de-DE localization by Markus Hentsch

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

struct strings de_DE = {
 /* === translation info === */
 /* code               */ L"de-DE",
 /* lang_english       */ L"German",
 /* lang               */ L"Deutsch",
 /* author             */ L"Markus Hentsch",

 /* === app === */
 /* tray_enabled       */ APP_NAME,
 /* tray_disabled      */ APP_NAME " (deaktiviert)",
 { /* menu */
   /* enable           */ L"Aktivieren",
   /* disable          */ L"Deaktivieren",
   /* hide             */ L"Symbol verstecken",
   /* update           */ L"Aktualisierung verfügbar!",
   /* config           */ L"Einstellungen",
   /* about            */ L"Über...",
   /* exit             */ L"Beenden",
 },
 { /* update */
   /* balloon          */ L"Neue Version verfügbar!",
   /* dialog           */ L"Eine neue Version ist verfügbar. Zur Webseite wechseln?",
   /* nonew            */ L"Keine Aktualisierung verfügbar.",
 },

 /* === config === */
 /* title              */ APP_NAME L" Einstellungen",
 { /* tabs */
   /* general          */ L"Allgemein",
   /* input            */ L"Maus und Tastatur",
   /* blacklist        */ L"Blacklist",
   /* advanced         */ L"Erweitert",
   /* about            */ L"Über",
 },
 { /* general tab */
   /* box              */ L"Allgemeine Einstellungen",
   /* autofocus        */ L"&Fenster beim Verschieben nach vorn bringen.\n(Dafür kann wahlweise auch Strg gedrückt werden)",
   /* aero             */ L"Aero S&nap immitieren",
   /* inactivescroll   */ L"&Scrollen des Mausrades in inaktiven Fenstern",
   /* mdi              */ L"&MDI support",
   /* autosnap         */ L"Fenster automatisch einrasten an:",
   /* autosnap0        */ L"Deaktiviert",
   /* autosnap1        */ L"Bildschirmkanten",
   /* autosnap2        */ L"+ Fenstern von außen",
   /* autosnap3        */ L"+ Fenstern von innen",
   /* language         */ L"Sprache:",
   /* autostart_box    */ L"Autostart",
   /* autostart        */ APP_NAME L" bei der A&nmeldung starten",
   /* autostart_hide   */ L"&Tray Symbol verstecken",
   /* autostart_elevate*/ L"als A&dministrator ausführen",
   /*       elevate_tip*/ L"Beachten Sie, dass jedes Mal nach dem Anmelden eine Benutzerkontenabfrage erscheint solange die Benutzerkontensteuerung aktiv ist.",
   /* elevate          */ L"Ad&min-Modus",
   /* elevated         */ L"Admin aktiv",
   /* elevate_tip      */ L"Dies wird eine neue Instanz von "APP_NAME" erstellen, die mit Administratorberechtigungen ausgeführt wird. Dies ermöglicht "APP_NAME" andere Programme zu verwalten, die auch mit Administratorberechtigungen gestartet wurden.\n\nSie müssen eine Benutzerkontenabfrage bestätigen, wenn Sie "APP_NAME" Administratorberechtigungen verleihen wollen.",
   /* elevation_aborted*/ L"Admin-Modus abgebrochen.",
   /* autosave         */ L"Hinweis: Änderungen werden sofort gespeichert und aktiv!",
 },
 { /* input tab */
   { /* mouse */
     /* box            */ L"Mausbefehle",
     /* lmb            */ L"Linke Maustaste:",
     /* mmb            */ L"Mittlere Maustaste:",
     /* rmb            */ L"Rechte Maustaste:",
     /* mb4            */ L"Maustaste 4:",
     /* mb5            */ L"Maustaste 5:",
     /* scroll         */ L"Scrollrad:",
     /* lowerwithmmb   */ L"&Lower windows by middle clicking on title bars",
   },
   { /* actions */
     /* move           */ L"Fenster verschieben",
     /* resize         */ L"Fenstergröße ändern",
     /* close          */ L"Fenster schließen",
     /* minimize       */ L"Fenster minimieren",
     /* lower          */ L"Fenster in den Hintergrund",
     /* alwaysontop    */ L"Immer im Vordergrund",
     /* center         */ L"Fenster zentrieren",
     /* nothing        */ L"Nichts",
     /* alttab         */ L"Alt+Tab",
     /* volume         */ L"Lautstärke",
     /* transparency   */ L"Transparenz",
   },
   { /* hotkeys */
     /* box            */ L"Tastaturkürzel",
     /* leftalt        */ L"Alt li&nks",
     /* rightalt       */ L"Alt &rechts",
     /* leftwinkey     */ L"&Windows links",
     /* rightwinkey    */ L"W&indows rechts",
     /* leftctrl       */ L"&Strg links",
     /* rightctrl      */ L"S&trg rechts",
     /* more           */ L"Weitere Tasten können in der ini Datei hinzugefügt werden.\nBenutzen Sie Shift um Fenster einrasten zu lassen.",
   },
 },
 { /* blacklist tab */
   /* box              */ L"Blacklist Einstellungen",
   /* processblacklist */ L"Blacklist für Prozesse:",
   /* blacklist        */ L"Blacklist:",
   /* snaplist         */ L"Snaplist (Einrasten verhindern):",
   /* explanation      */ L"Eine umfassende Erklärung, wie die Blacklist funktioniert, finden Sie in <a href=\"http://code.google.com/p/altdrag/wiki/Blacklist\">der Wiki</a>!",
   /* findwindow_box   */ L"Fenster identifizieren",
   /* _explanation     */ L"Klicken Sie auf das Symbol um den Klassennamen eines Fensters zu ermitteln, sodass dieser oben der Blacklist oder Snaplist hinzugefügt werden kann.",
 },
 { /* advanced tab */
   /* box              */ L"Erweiterte Einstellungen",
   /* hookwindows      */ L"&Einrasten beim normalen Verschieben von Fenstern.\nFunktioniert zusammen mit automatischem Einrasten!",
   /* hookwindows_warn */ L"Beachten Sie, dass diese Funktion nicht zu 100% sicher ist, da sie durch das Einhängen in andere Prozesse ermöglicht wird. Dies könnte bei Spielen riskant sein, die einen Anti-Cheat Schutz besitzen.\n\nSind Sie sich sicher, dass Sie diese Funktion aktivieren möchten?",
   /* checkonstartup   */ L"A&utomatisch nach Aktualisierungen suchen",
   /* beta             */ L"Auf &Beta-Versionen prüfen",
   /* checknow         */ L"&Jetzt prüfen",
   /* ini              */ L"Die Einstellungen von "APP_NAME" werden in "APP_NAME".ini gespeichert. Es gibt ein paar Dinge, die nur durch das Bearbeiten dieser Datei eingestellt werden können.",
   /* openini          */ L"&ini Datei öffnen",
 },
 { /* about tab */
   /* box              */ L"Über "APP_NAME,
   /* version          */ L"Version "APP_VERSION,
   /* author           */ L"Ertellt von Stefan Sundin",
   /* license          */ APP_NAME L" ist freie Open-Source Software!\nGeben Sie es ruhig weiter!",
   /* donate           */ L"&Spenden",
   /*translation_credit*/ L"Übersetzungen",
 },

 /* === misc === */
 /* unhook_error       */ L"There was an error disabling "APP_NAME". This was most likely caused by Windows having already disabled "APP_NAME"'s hooks.\n\nIf this is the first time this has happened, you can safely ignore it and continue using "APP_NAME".\n\nIf this is happening repeatedly, you can read on the website wiki how to prevent this from happening again (look for '"APP_NAME" mysteriously stops working' on the About page).",
};
