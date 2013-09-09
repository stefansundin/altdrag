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
   /* config           */ L"Configura...",
   /* about            */ L"Info...",
   /* exit             */ L"Esci",
 },
 { /* update */
   /* balloon          */ L"È stata trovata una nuova versione!",
   /* dialog           */ L"È disponibile una nuova versione. Vai al sito web?",
   /* nonew            */ L"Non è disponibile alcun aggiornamento.",
 },

 /* === config === */
 /* title              */ L"Configurazione " APP_NAME,
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
   /* aero             */ L"Mima Aero S&nap",
   /* inactivescroll   */ L"&Scorri finestre inattive",
   /* mdi              */ L"Supporto &MDI",
   /* autosnap         */ L"Aggancia automaticamente a:",
   /* autosnap0        */ L"Disattivato",
   /* autosnap1        */ L"Bordi dello schermo",
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
   /* elevate_tip      */ L"Crea una nuova istanza di "APP_NAME" con privilegi da amministratore. Assicura ad "APP_NAME" di poter gestire gli altri programmi in esecuzione con privilegi da amministratore.\n\nDovrai confermare una richiesta UAC da parte di Windows per permettere ad "APP_NAME" di essere avviato con privilegi da amministratore.",
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
     /* lowerwithmmb   */ L"S&posta dietro le finestre premendo il tasto centrale sul titolo della barra.",
   },
   { /* actions */
     /* move           */ L"Muovi finestra",
     /* resize         */ L"Ingrandisci finestra",
     /* close          */ L"Chiudi finestra",
     /* minimize       */ L"Riduci a icona finestra",
     /* lower          */ L"Sposta dietro finestra",
     /* alwaysontop    */ L"Scambia Sempre sopra",
     /* center         */ L"Centra finestra sullo schermo",
     /* nothing        */ L"Niente",
     /* alttab         */ L"Alt+Tab",
     /* volume         */ L"Volume",
     /* transparency   */ L"Trasparenza",
   },
   { /* hotkeys */
     /* box            */ L"Tasti di scelta rapida",
     /* leftalt        */ L"Alt &sinistro",
     /* rightalt       */ L"Alt &destro",
     /* leftwinkey     */ L"&Win sinistro",
     /* rightwinkey    */ L"W&in destro",
     /* leftctrl       */ L"&Ctrl sinistro",
     /* rightctrl      */ L"C&trl destro",
     /* more           */ L"Puoi aggiungere qualunque tasto modificando il file ini.\nUsa il tasto maiusc per agganciare una finestra all'altra.",
   },
 },
 { /* blacklist tab */
   /* box              */ L"Impostazioni lista nera",
   /* processblacklist */ L"Processi in lista nera:",
   /* blacklist        */ L"Lista nera:",
   /* snaplist         */ L"Lista aggancio:",
   /* explanation      */ L"Leggi la <a href=\"http://code.google.com/p/altdrag/wiki/Blacklist\">wiki</a> sul sito web per una spiegazione approfondita di come funziona la lista nera!",
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

 /* === misc === */
 /* unhook_error       */ L"Errore durante la disattivazione di "APP_NAME". Probabilmente causato da Windows che aveva già disattivato l'hook di "APP_NAME".\n\nSe è la prima volta che succede puoi tranquillamente ignorarlo e continuare ad usare "APP_NAME".\n\nSe succede spesso, puoi leggere il sito web wiki su come evitare che si ripeta (cerca '"APP_NAME" mysteriously stops working' nella pagina About).",
};
