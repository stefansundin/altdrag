---
layout: subpage
title: Translate AltDrag
---

Before you start translating, I recommend that you take a look [in the source tree](https://github.com/stefansundin/altdrag/tree/master/localization) and see if your language already has been translated, or if an out-of-date translation exists.

AltDrag can be translated via an ini file. Download the [English Translation.ini file](https://raw.githubusercontent.com/stefansundin/altdrag/master/localization/en_US/Translation.ini). Save it in the same directory as `AltDrag.exe`. Start translating the strings and launch AltDrag to try it out.

Look for your language code in the [ISO 639-1 standard](http://en.wikipedia.org/wiki/List_of_ISO_639-2_codes), and your territory code in the [ISO 3166-1 alpha-2 standard](http://en.wikipedia.org/wiki/ISO_3166-1_alpha-2#Officially_assigned_code_elements).

Some things to note:

- You have to reload AltDrag between your changes (click the tray icon two times).
- AltDrag is fully unicode enabled, so all kinds of characters should be possible. Make sure to save the file with the correct encoding.
- The sequence `\n` is converted to a newline.
- The `&`-character specify that the next letter will be the shortcut to the control (e.g. the checkbox with the text `&MDI support` can be toggled with Alt+M).


## The installer

The installer has to be compiled to be translated. Download [installer.nsh](https://raw.githubusercontent.com/stefansundin/altdrag/master/localization/en_US/installer.nsh) and translate the strings. Include it in your submission.

It is possible to build the installer without compiling AltDrag itself, so it is not that difficult to test by yourself. You still have to download the source code and [NSIS Unicode](http://code.google.com/p/unsis/downloads/list). Reading the [build page](build.html) is helpful for this step.

Once you have the source code, create the directory `build` inside the directory you checked out, and copy the files `AltDrag.exe` and `hooks.dll` to it. If you have `HookWindows_x64.exe` and `hooks_x64.dll`, you can copy those as well.

You then need to add a line in `localization/installer.nsh` to make your language appear. To make the installer update the ini file with your language after install, you need to add a line in the middle of [`installer.nsi`](https://github.com/stefansundin/altdrag/blob/bdd76dd0ac6d24e447d43c7d378d8c13f17a3099/installer.nsi#L229).

Then use the NSIS wizard to compile `installer.nsi`.

If the installer isn't prompting you for a language, it means that it has stored the language you used previously. Simply uninstall AltDrag using the installer, and then try again.


## Submit the translation

Checklist before you submit your translation:

- Please double-check all your strings.
- Please make sure that the Alt shortcuts do not conflict with each other. There should not be two on the same page with the same shortcut.
- Please update the top comments with your name and (optional) email.

Then either [create an issue report](https://github.com/stefansundin/altdrag/issues/new?labels[]=translation&title=Swedish%20(se-SE)&body=-%20[%20]%20I%20have%20double-checked%20all%20strings%0A-%20[%20]%20I%20have%20double-checked%20the%20Alt-shortcuts%0A-%20[%20]%20I%20have%20updated%20the%20top%20comments%20with%20my%20name%20and%20(optional)%20email%0A-%20[%20]%20I%20have%20attached%20the%20files) and attach the files, or create a pull request from a forked branch.


{% include improve-me.md %}
