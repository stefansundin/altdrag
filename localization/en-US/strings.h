/*
	AltDrag - en-US localization by Stefan Sundin (recover89@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

struct strings en_US = {
 /* tray_enabled       */ APP_NAME,
 /* tray_disabled      */ APP_NAME " (disabled)",
 { /* menu */
   /* enable           */ L"Enable",
   /* disable          */ L"Disable",
   /* hide             */ L"Hide tray",
   /* update           */ L"Update available!",
   /* config           */ L"Configure",
   /* about            */ L"About",
   /* exit             */ L"Exit",
 },
 { /* update */
   /* balloon          */ L"New version found!",
   /* dialog           */ L"A new version is available. Go to website?",
   /* nonew            */ L"No update available.",
 },
 
 /* === config === */
 /* lang               */ L"English",
 /* title              */ APP_NAME L" Configuration",
 { /* tabs */
   /* general          */ L"General",
   /* input            */ L"Mouse and keyboard",
   /* blacklist        */ L"Blacklist",
   /* advanced         */ L"Advanced",
   /* about            */ L"About",
 },
 { /* general tab */
   /* box              */ L"General settings",
   /* autofocus        */ L"&Focus windows when dragging.\nYou can also press Ctrl to focus windows.",
   /* aero             */ L"&Mimic Aero Snap",
   /* inactivescroll   */ L"&Scroll inactive windows",
   /* autosnap         */ L"Automatically snap to:",
   /* autosnap0        */ L"Disabled",
   /* autosnap1        */ L"To screen borders",
   /* autosnap2        */ L"+ outside of windows",
   /* autosnap3        */ L"+ inside of windows",
   /* language         */ L"Language:",
   /* helptranslate    */ L"How can I help translate?",
   /* autostart_box    */ L"Autostart",
   /* autostart        */ L"S&tart "APP_NAME" when logging on",
   /* autostart_hide   */ L"&Hide tray",
   /* autostart_elevate*/ L"&Elevate to administrator privileges",
   /*       elevate_tip*/ L"Note that the UAC prompt will appear every time you log in. There is a workaround for this (described on the website wiki), but it is not 100% trouble free.",
   /* elevate          */ L"E&levate",
   /* elevated         */ L"Elevated",
   /* elevate_tip      */ L"This will create a new instance of "APP_NAME" which is running with administrator privileges. This allows "APP_NAME" to manage other programs which are running with administrator privileges.\n\nYou will have to approve a UAC prompt from Windows to allow "APP_NAME" to run with administrator privileges.",
   /* elevation_aborted*/ L"Elevation aborted.",
   /* autosave         */ L"Note: Settings are saved and applied instantly!",
 },
 { /* input tab */
   { /* mouse */
     /* box            */ L"Mouse actions",
     /* lmb            */ L"Left mouse button:",
     /* mmb            */ L"Middle mouse button:",
     /* rmb            */ L"Right mouse button:",
     /* mb4            */ L"Mouse button 4:",
     /* mb5            */ L"Mouse button 5:",
     /* scroll         */ L"Scrollwheel:",
     /* more           */ L"If you have a great idea for a mouse action, put in a request <a href=\"http://code.google.com/p/altdrag/issues/list\">on the website</a>!",
   },
   { /* actions */
     /* move           */ L"Move window",
     /* resize         */ L"Resize window",
     /* close          */ L"Close window",
     /* minimize       */ L"Minimize window",
     /* lower          */ L"Lower window",
     /* alwaysontop    */ L"Toggle always on top",
     /* center         */ L"Center window on screen",
     /* nothing        */ L"Nothing",
     /* alttab         */ L"Alt+Tab",
     /* volume         */ L"Volume",
   },
   { /* hotkeys */
     /* box            */ L"Hotkeys",
     /* leftalt        */ L"Left &Alt",
     /* rightalt       */ L"Right A&lt",
     /* leftwinkey     */ L"Left &Winkey",
     /* rightwinkey    */ L"Right W&inkey",
     /* leftctrl       */ L"Left &Ctrl",
     /* rightctrl      */ L"Right C&trl",
     /* more           */ L"You can add any key by editing the ini file!\nUse the shift key to snap windows to each other.",
   },
 },
 { /* blacklist tab */
   /* box              */ L"Blacklist settings",
   /* processblacklist */ L"Process blacklist:",
   /* blacklist        */ L"Blacklist:",
   /* snaplist         */ L"Snaplist:",
   /* explanation      */ L"Read <a href=\"http://code.google.com/p/altdrag/wiki/Blacklist\">the wiki</a> on the website for a thorough explanation of how the blacklist works!",
   /* findwindow_box   */ L"Identify window",
   /* _explanation     */ L"Use this feature to identify the classname of a window so you can add it to the Blacklist or Snaplist above.",
 },
 { /* advanced tab */
   /* box              */ L"Advanced settings",
   /* hookwindows      */ L"&Enable snapping when normally moving windows.\nWorks in conjunction with automatic snapping!",
   /* hookwindows_warn */ L"Note that this is not 100% safe as it hooks into other processes and stuff like that. Might be risky to use when playing games with anti-cheat protection.\n\nAre you sure you want to enable this feature?",
   /* checkonstartup   */ L"Automatically check for &updates",
   /* beta             */ L"Check for &beta versions",
   /* checknow         */ L"&Check now",
   /* ini              */ APP_NAME L"'s settings are saved in "APP_NAME".ini. There are a few things that you can only configure by editing the file manually.",
   /* openini          */ L"Open &ini file",
 },
 { /* about tab */
   /* box              */ L"About "APP_NAME,
   /* version          */ L"Version "APP_VERSION,
   /* author           */ L"Created by Stefan Sundin",
   /* license          */ APP_NAME L" is free and open source software!\nPlease redistribute!",
   /* donations_box    */ L"Donations",
   /* donations_plea   */ L"If you find "APP_NAME" useful, please consider donating money. I don't ask for much, any amount will do.",
   /* donate           */ L"&Donate",
 },
};
