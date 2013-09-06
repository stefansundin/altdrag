
struct strings {
	wchar_t *menu_find;
	wchar_t *menu_finddelay;
	wchar_t *menu_findall;
	wchar_t *menu_about;
	wchar_t *menu_exit;
	wchar_t *wnddetails;
	wchar_t *allwnds;
	wchar_t *about_title;
	wchar_t *about;
};

struct strings en_US = {
 /* menu_find      */ L"Find",
 /* menu_finddelay */ L"Find in 3 seconds",
 /* menu_findall   */ L"Enumerate all windows",
 /* menu_about     */ L"About",
 /* menu_exit      */ L"Exit",
 /* wnddetails     */ L"Window details",
 /* allwnds        */ L"Visible windows",
 /* about_title    */ L"About " APP_NAME,
 /* about          */ APP_NAME " - " APP_VERSION "\n\
Made by Stefan Sundin\n\
\n\
Find window's title and classname for use in AltDrag's blacklist. \
Click the tray icon and then click the window you want to blacklist. \
A popup will appear with information about that window. \
For more tricky windows, use the delayed find.\n\
\n\
Note that you can't blacklist windows in AltDrag by using the child or component information! \
They are provided purely for informational purposes!\n\
\n\
Read more on wiki page: "APP_URL"\n\
Visit the author's website at http://stefansundin.com/"
};

struct strings *l10n=&en_US;
