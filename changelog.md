
**1.0 final** - 2013-09-13:
  * Add `Lower` scroll wheel action. Scroll down to lower windows, scroll up to raise windows, shift+scroll down to minimize windows, shift+scroll up to toggle maximized state.
  * Other `Lower` actions will now also minimize the window if you hold shift.
  * Updated most of the translations.

**1.0 release candidate** - 2013-09-06:
  * Add support for MDI windows. Works with many features (e.g. `HookWindows`, `AltTab` scrolling, snapping).
  * Add ability to lower windows by middle clicking title bars.
  * Add ability to translate AltDrag with an ini file.
  * Many small fixes for multiple monitors.
  * Made the corner regions for resizing bigger, now corners are 38% and middle regions 24%. Before all regions were 33%.
  * Fixed `HookWindows` sometimes blocking the space key.
  * Fixed a few issues with `InactiveScroll`.

**1.0 beta 3** - 2013-06-20:
  * If you enable `InactiveScroll`, you can now hold the shift key to scroll horizontally. Note that this only works in programs that have implemented horizontal scrolling.
  * New scroll action: `Transparency`. It allows you to set the transparency of arbitrary windows.
  * The code that prevents the window menu from triggering has been reworked to work with Internet Explorer.

**1.0 beta 2** - 2013-04-20:
  * Merged `Config.exe` with `AltDrag.exe`.
  * Made it possible to disable the Alt+Scroll action. It was silly of me to not let you do this in beta 1, sorry!
  * Added new Alt+Scroll action that changes the volume.
  * You can now press the Space key during a drag to reset any custom `AutoSnap` setting for the duration of the drag.
  * AltDrag can now request elevation to administrator privileges on demand, allowing for much easier elevation on autostart.
  * AltDrag can now be run in multiple instances, useful if you e.g. want to use Alt+LMB to move and Ctrl+LMB to resize. See `MultipleInstances` in the ini file.
  * Added experimental feature `FocusOnTyping` that moves focus to the window the mouse is hovering when you start typing. This is a new approach to the hidden X-Mouse feature in Windows. See the ini file for more details.
  * Fixed issue with iTunes.

**1.0 beta 1** - 2011-07-31:
  * Added configuration GUI.
  * Added ability to scroll through windows with the scrollwheel (like Alt+Tab!).
  * Added ability to blacklist by process name.
  * Added new mouse action: `Lower`.
  * Added performance options `MoveRate` and `ResizeRate`. Increase if dragging or resizing is slow.
  * The scroll hook should no longer stop working when locking the screen or resuming the computer from sleep. A timer checks every 5 seconds if the scroll hook is still working, and re-enables it if it is not.
  * You can now press Escape to stop AltDrag. Useful if AltDrag gets in a weird state.
  * You can now press Ctrl to make AltDrag think the mouse cursor is still inside the current monitor. Should help people who have multiple monitors and want to snap at borders between monitors.
  * Installer now grants access to `AltDrag.ini` to all users. Should help people who use UAC.
  * Removed `info.txt`. Use the website for help.

**0.9** - 2011-02-01:
  * Added `Aero` option. Alt-drag windows to the left, right and top side of the screen. Corners work too!
  * Added `AutoFocus` and `AutoRemaximize` options.
  * Added `InactiveScroll`. Enable it to scroll inactive windows.
  * Added new mouse actions: `AlwaysOnTop` and `Close`.
  * It is now possible to toggle the maximized state with a resize button while dragging.
  * Replaced roll-up behavior with Aero behavior.
  * Renamed "stick" to "snap" as people seem to "get" that term better.
  * Fixed some bugs that prevented `HookWindows` from working properly.
  * Tweaked the blacklist.

**0.8** - 2009-11-15:
  * Added `AutoStick`, enable it to make windows always stick without needing to press shift. This replaces `StickyScreen`.
  * Renamed `ExperimentalFeatures` to `HookWindows`, also improved it so resizing is now supported and windows should no longer flicker. 64-bit programs also work now. I no longer consider it experimental!
  * The alt key will no longer activate the window menu!
  * Added the ability to configure what keys should trigger the dragging, resizing, etc.
  * Added the ability to configure what the mouse buttons do. New actions: minimize and center.
  * Improved multi-monitor support a lot.
  * Tweaked the blacklist.

**0.7** - 2009-04-24:
  * Added `StickyScreen` which makes windows automatically stick to the screen borders without needing shift (off by default).
  * Added the ability to roll-up windows by double-clicking with the middle mouse button.
  * Fixed some bugs in `ExperimentalFeatures`, it's working pretty well now so give it a try.
  * Fixed an annoying error message that sometimes popped up when AltDrag was on autostart.

**0.6** - 2009-02-12:
  * Sticky behavior now works with resize (except when resizing in all directions).
  * Fixed tray not being hidden by `-hide`.
  * Fixed mouse cursor not changing when Aero was disabled in Vista.
  * Language can now be changed in `AltDrag.ini`.

**0.5** - 2009-01-12:
  * You can now resize windows with the middle or right mouse button.
  * You can now double-click a window to maximize it.
  * You can now blacklist certain windows from being dragged and certain windows from being sticked to.
  * Blacklisted by default: the alt+tab window in Vista.
  * Sticky-blacklisted by default: the Vista start menu orb.
  * The mouse cursor will now change when you move and resize windows.
  * Only one instance of AltDrag can now be run. Starting AltDrag again will make the first copy add its tray icon.
  * Experimental feature disabled by default: you can now press the shift key while normally moving windows (dragging the titlebar) to make them stick. Doesn't work on all windows in Vista. Doesn't work with 64-bit processes.

**0.4** - 2008-09-14:
  * You can now press the shift key to make the window stick to other windows.

**0.3** - 2008-08-03:
  * Fixed a bug that made AltDrag ignore all windows which were at the screen borders since it thought they were fullscreen windows.
  * Improved performance by only installing the mouse hook when the alt key is pressed.

**0.2** - 2008-08-01:
  * You can no longer drag fullscreen windows.
  * The window is better placed when it's restored from its maximized state.

**0.1** - 2008-06-05:
  * Initial release.
