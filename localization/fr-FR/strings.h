/*
	AltDrag - fr-FR localization by Samy Mechiri (samy.mechiri@kxen.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

struct strings fr_FR = {
 /* tray_enabled       */ APP_NAME,
 /* tray_disabled      */ APP_NAME " (désactivé)",
 { /* menu */
   /* enable           */ L"Activer",
   /* disable          */ L"Désactiver",
   /* hide             */ L"Cacher l'icone",
   /* update           */ L"Mise-à-jour disponible!",
   /* config           */ L"Paramètres",
   /* about            */ L"A Propos",
   /* exit             */ L"Quitter",
 },
 { /* update */
   /* balloon          */ L"Nouvelle version disponible!",
   /* dialog           */ L"Une nouvelle version est disponible. Aller sur le site web?",
   /* nonew            */ L"Pas de mise-à-jour disponible.",
 },
 
 /* === config === */
 /* lang               */ L"Français",
 /* title              */ L"Configuration de " APP_NAME,
 { /* tabs */
   /* general          */ L"Général",
   /* input            */ L"Souris et clavier",
   /* blacklist        */ L"Liste noire",
   /* advanced         */ L"Avancé",
   /* about            */ L"A propos",
 },
 { /* general tab */
   /* box              */ L"Paramètres généraux",
   /* autofocus        */ L"&Activer les fenêtres en les déplaçant.\nAppuyez sur Ctrl pour activer les fenêtres.",
   /* aero             */ L"Imiter l'aimantation A&ero",
   /* inactivescroll   */ L"&Défiler les fenêtres inactives",
   /* autosnap         */ L"Aimanter les fenêtres:",
   /* autosnap0        */ L"Désactivé",
   /* autosnap1        */ L"Bords de l'écran",
   /* autosnap2        */ L"+ Extérieur des fenêtres",
   /* autosnap3        */ L"+ Intérieur des fenêtres",
   /* language         */ L"Langue:",
   /* helptranslate    */ L"Aider à la traduction?",
   /* autostart_box    */ L"Démarrage automatique",
   /* autostart        */ L"Déma&rrer "APP_NAME" en se connectant",
   /* autostart_hide   */ L"Cac&her l'icone de la barre d'outils",
   /* autostart_elevate*/ L"&Elevate to administrator privileges",
   /*       elevate_tip*/ L"Note that a UAC prompt will appear every time you log in, unless you disable UAC completely.",
   /* elevate          */ L"E&levate",
   /* elevated         */ L"Elevated",
   /* elevate_tip      */ L"This will create a new instance of "APP_NAME" which is running with administrator privileges. This allows "APP_NAME" to manage other programs which are running with administrator privileges.\n\nYou will have to approve a UAC prompt from Windows to allow "APP_NAME" to run with administrator privileges.",
   /* elevation_aborted*/ L"Elevation aborted.",
   /* autosave         */ L"Note: Les paramètres sont sauvés et appliqués instantanément!",
 },
 { /* input tab */
   { /* mouse */
     /* box            */ L"Souris",
     /* lmb            */ L"Bouton gauche:",
     /* mmb            */ L"Bouton du milieu:",
     /* rmb            */ L"Bouton droit:",
     /* mb4            */ L"Bouton 4:",
     /* mb5            */ L"Bouton 5:",
     /* scroll        `*/ L"Scroll:",
     /* more          `*/ L"Si vous avez une autre idée d'utilisation de la souris, <a href=\"http://code.google.com/p/altdrag/issues/list\">envoyez une demande</a>! (ndt:en anglais)",
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
     /* box            */ L"Activation",
     /* leftalt        */ L"&Alt gauche",
     /* rightalt       */ L"A&lt droit",
     /* leftwinkey     */ L"&Windows gauche",
     /* rightwinkey    */ L"W&indows droit",
     /* leftctrl       */ L"&Ctrl gauche",
     /* rightctrl      */ L"C&trl droit",
     /* more           */ L"Ajoutez les touches de votre choix en éditant le fichier ini!\nUtilisez shift pour aimanter les fenêtres les unes aux autres.",
   },
 },
 { /* blacklist tab */
   /* box              */ L"Liste noire",
   /* processblacklist */ L"Processus à ignorer:",
   /* blacklist        */ L"Fenêtres à ignorer:",
   /* snaplist         */ L"Fenêtres à aimanter:",
   /* explanation      */ L"Lire <a href=\"http://code.google.com/p/altdrag/wiki/Blacklist\">le wiki</a> sur le site web pour une explication complète sur le fonctionnement de la liste noire!",
   /* findwindow_box   */ L"Identifer une fenêtre",
   /* _explanation     */ L"Utilisez cette fonction pour identifier la classe d'une fenêtre afin de l'ajouter dans la liste des fenêtres à ignorer ou des fenêtres à aimanter ci-dessus.",
 },
 { /* advanced tab */
   /* box              */ L"Paramètres avancés",
   /* hookwindows      */ L"&Aimenter les fenêtres déplacées normalement.\nFonctionne en plus de l'aimantation automatique!",
   /* hookwindows_warn`*/ L"Cette fonction n'est pas sûr à 100% car elle s'insère dans des processus étrangers et des choses de ce genre.",
   /* checkonstartup   */ L"Chercher des mises-à-jours automatiquement",
   /* beta             */ L"Chercher des versions &beta",
   /* checknow         */ L"Vérifier la mise-à-jour",
   /* ini              */ L"Les paramètres de "APP_NAME" sont sauvés dans le fichier "APP_NAME".ini. Certains paramètres ne sont modifiable qu'en éditant le fichier manuellement.",
   /* openini          */ L"&Editer le fichier",
 },
 { /* about tab */
   /* box              */ L"A propos de "APP_NAME,
   /* version          */ L"Version "APP_VERSION,
   /* author           */ L"Crée par Stefan Sundin",
   /* license          */ APP_NAME L" est un logiciel gratuit et open source!\nFaites en profiter vos amis!",
   /* donations_box    */ L"Dons",
   /* donations_plea   */ L"Si "APP_NAME" vous est utile, donnez un coup de pouce financier. Donnez le montant de votre choix.",
   /* donate           */ L"&Donner",
 },
};
