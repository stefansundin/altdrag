# AltDrag v1.0b1

2011-07-31:
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
AltDrag-1.0b1.7z | 3
