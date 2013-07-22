/*
	AltDrag - it-IT localization by Hexaae (hexaae@gmail.com)

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

struct strings it_IT = {
 /* === translation info === */
 /* code               */ L"it-IT",
 /* lang_english       */ L"Italian",
 /* lang               */ L"Italiano",
 /* author             */ L"Hexaae",

 /* === app === */
 /* tray_enabled       */ APP_NAME,
 /* tray_disabled      */ APP_NAME " (disattivato)",
 { /* menu */
   /* enable           */ L"Attiva",
   /* disable          */ L"Disattiva",
   /* hide             */ L"Nascondi icona di notifica",
   /* update           */ L"È disponibile un aggiornamento!",
   /* config           */ L"Configura",
   /* about            */ L"Info",
   /* exit             */ L"Esci",
 },
 { /* update */
   /* balloon          */ L"È stata trovata una nuova versione!",
   /* dialog           */ L"È disponibile una nuova versione. Vai al sito web?",
   /* nonew            */ L"Non è disponibile alcun aggiornamento.",
 },

 /* === config === */
 /* title              */ APP_NAME L" Configurazione",
 { /* tabs */
   /* general          */ L"Generale",
   /* input            */ L"Mouse e tastiera",
   /* blacklist        */ L"Lista nera",
   /* advanced         */ L"Avanzate",
   /* about            */ L"Info",
 },
 { /* general tab */
   /* box              */ L"Impostazioni generali",
   /* autofocus        */ L"Seleziona la &finestra durante il trascinamento.\nPuoi anche premere Ctrl per attivare la finestra.",
   /* aero             */ L"&Mima Aero Snap",
   /* inactivescroll   */ L"&Scorri finestre inattive",
   /* autosnap         */ L"Aggancia automaticamente a:",
   /* autosnap0        */ L"disattivato",
   /* autosnap1        */ L"bordi dello schermo",
   /* autosnap2        */ L"+ esterno finestre",
   /* autosnap3        */ L"+ interno finestre",
   /* language         */ L"Lingua:",
   /* autostart_box    */ L"Avvio automatico",
   /* autostart        */ L"A&vvia "APP_NAME" all'accesso",
   /* autostart_hide   */ L"Nascon&di icona di notifica",
   /* autostart_elevate*/ L"&Eleva con privilegi amministratore",
   /*       elevate_tip*/ L"Nota che una richiesta UAC apparirà tutte le volte che accederai, a meno di disattivare completamente l'UAC.",
   /* elevate          */ L"E&leva",
   /* elevated         */ L"Elevato",
   /* elevate_tip      */ L"Crea una nuova instanza di "APP_NAME" con privilegi da amministratore. Ciò permette a "APP_NAME" di gestire gli altri programmi con privilegi da amministratore in esecuzione.\n\nDovrai confermare una richiesta UAC da parte di Windows per permettere a "APP_NAME" di essere avviato con privilegi da amministratore.",
   /* elevation_aborted*/ L"Elevazione annullata.",
   /* autosave         */ L"N.B.: le impostazioni sono applicate e salvate istantaneamente!",
 },
 { /* input tab */
   { /* mouse */
     /* box            */ L"Azioni mouse",
     /* lmb            */ L"Tasto sinistro:",
     /* mmb            */ L"Tasto centrale:",
     /* rmb            */ L"Tasto destro:",
     /* mb4            */ L"Tasto mouse 4:",
     /* mb5            */ L"Tasto mouse 5:",
     /* scroll         */ L"Rotellina:",
     /* more           */ L"Se ti viene una grande idea per un'azione mouse, aggiungi una richiesta <a href=\"http://code.google.com/p/altdrag/issues/list\">sul sito web</a>!",
   },
   { /* actions */
     /* move           */ L"muovi finestra",
     /* resize         */ L"ingrandisci finestra",
     /* close          */ L"chiudi finestra",
     /* minimize       */ L"riduci a icona finestra",
     /* lower          */ L"sposta dietro finestra",
     /* alwaysontop    */ L"scambia Sempre sopra",
     /* center         */ L"centra finestra sullo schermo",
     /* nothing        */ L"nulla",
     /* alttab         */ L"Alt+Tab",
     /* volume         */ L"volume",
     /* transparency   */ L"trasparenza",
   },
   { /* hotkeys */
     /* box            */ L"Tasti di scelta rapida",
     /* leftalt        */ L"&Alt sinistro",
     /* rightalt       */ L"A&lt destro",
     /* leftwinkey     */ L"&Win sinistro",
     /* rightwinkey    */ L"W&in destro",
     /* leftctrl       */ L"&Ctrl sinistro",
     /* rightctrl      */ L"C&trl destro",
     /* more           */ L"Puoi aggiungere qualunque tasto modificando il file ini.\nUsa il tasto maiusc per agganciare una finestra all'altra.",
   },
 },
 { /* blacklist tab */
   /* box              */ L"Impostazioni lista nera",
   /* processblacklist */ L"Lista nera processi:",
   /* blacklist        */ L"Lista nera:",
   /* snaplist         */ L"Lista aggancio:",
   /* explanation      */ L"Leggi <a href=\"http://code.google.com/p/altdrag/wiki/Blacklist\">la wiki</a> sul sito web per una spiegazione approfondita di come funziona la lista nera!",
   /* findwindow_box   */ L"Indentifica finestra",
   /* _explanation     */ L"Clicca l'icona per identificare la classname della finestra in modo da poterla aggiungere alla Lista nera o Lista aggancio sopra.",
 },
 { /* advanced tab */
   /* box              */ L"Impostazioni avanzate",
   /* hookwindows      */ L"A&ttiva l'aggancio muovendo le finestre normalmente.\nFunziona insieme all'aggancio automatico!",
   /* hookwindows_warn */ L"Nota che non è sicuro al 100% dato che questa funzionalità lavora tramite hooking in altri processi. Potrebbe essere rischioso quando si utilizzano giochi con protezione anti-cheat.\n\nSicuro di volerla attivare?",
   /* checkonstartup   */ L"Verifica automaticamente gli a&ggiornamenti",
   /* beta             */ L"Verifica presenza di versioni &beta",
   /* checknow         */ L"&Controlla ora",
   /* ini              */ APP_NAME L" salva le impostazioni in "APP_NAME".ini. Ci sono alcune cose che possono essere configurate solo tramite modifica manuale.",
   /* openini          */ L"Apri file &ini",
 },
 { /* about tab */
   /* box              */ L"Informazioni su "APP_NAME,
   /* version          */ L"Versione "APP_VERSION,
   /* author           */ L"Creato da Stefan Sundin",
   /* license          */ APP_NAME L" è software libero e open source!\nSiete liberi di distribuirlo!",
   /* donate           */ L"&Dona",
   /*translation_credit*/ L"Crediti traduzione",
 },
};
