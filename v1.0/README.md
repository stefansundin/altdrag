# AltDrag v1.0

**final** - 2013-09-13:
- Add `Lower` scroll wheel action. Scroll down to lower windows, scroll up to raise windows, shift+scroll down to minimize windows, shift+scroll up to toggle maximized state.
- Other `Lower` actions will now also minimize the window if you hold shift.
- Updated most of the translations.

**release candidate** - 2013-09-06:
- Add support for MDI windows. Works with many features (e.g. `HookWindows`, `AltTab` scrolling, snapping).
- Add ability to lower windows by middle clicking title bars.
- Add ability to translate AltDrag with an ini file.
- Many small fixes for multiple monitors.
- Made the corner regions for resizing bigger, now corners are 38% and middle regions 24%. Before all regions were 33%.
- Fixed `HookWindows` sometimes blocking the space key.
- Fixed a few issues with `InactiveScroll`.

**beta 3** - 2013-06-20:
- If you enable `InactiveScroll`, you can now hold the shift key to scroll horizontally. Note that this only works in programs that have implemented horizontal scrolling.
- New scroll action: `Transparency`. It allows you to set the transparency of arbitrary windows.
- The code that prevents the window menu from triggering has been reworked to work with Internet Explorer.

**beta 2** - 2013-04-20:
- Merged `Config.exe` with `AltDrag.exe`.
- Made it possible to disable the Alt+Scroll action. It was silly of me to not let you do this in beta 1, sorry!
- Added new Alt+Scroll action that changes the volume.
- You can now press the Space key during a drag to reset any custom `AutoSnap` setting for the duration of the drag.
- AltDrag can now request elevation to administrator privileges on demand, allowing for much easier elevation on autostart.
- AltDrag can now be run in multiple instances, useful if you e.g. want to use Alt+LMB to move and Ctrl+LMB to resize. See `MultipleInstances` in the ini file.
- Added experimental feature `FocusOnTyping` that moves focus to the window the mouse is hovering when you start typing. This is a new approach to the hidden X-Mouse feature in Windows. See the ini file for more details.
- Fixed issue with iTunes.

**beta 1** - 2011-07-31:
- Added configuration GUI.
- Added ability to scroll through windows with the scrollwheel (like Alt+Tab!).
- Added ability to blacklist by process name.
- Added new mouse action: `Lower`.
- Added performance options `MoveRate` and `ResizeRate`. Increase if dragging or resizing is slow.
- The scroll hook should no longer stop working when locking the screen or resuming the computer from sleep. A timer checks every 5 seconds if the scroll hook is still working, and re-enables it if it is not.
- You can now press Escape to stop AltDrag. Useful if AltDrag gets in a weird state.
- You can now press Ctrl to make AltDrag think the mouse cursor is still inside the current monitor. Should help people who have multiple monitors and want to snap at borders between monitors.
- Installer now grants access to `AltDrag.ini` to all users. Should help people who use UAC.
- Removed `info.txt`. Use the website for help.

## Downloads by filename

Filename | Download Count
-------- | --------------
AltDrag-1.0.7z | 1,945
AltDrag-1.0.exe | 8,586
