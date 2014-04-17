# Introduction

There are three blacklists in AltDrag. Here is a brief explanation of them:

| Setting            | Explanation |
| -------            | ----------- |
| `ProcessBlacklist` | A list of processes that *should not* be affected by dragging and other actions. It is here you specify processes that you don't want AltDrag to touch. |
| `Blacklist`        | A list of windows that *should not* be affected by dragging and other actions. This list also affects `InactiveScroll`. |
| `Snaplist`         | A list of windows that *should* be snapped to. You should only need to specify windows that do not have a normal border here (like Winamp or Spotify). |

Since 1.0, you can specify processes in `ProcessBlacklist`. It is a comma separated list of executable names. The matching is case-insensitive.

`Blacklist` and `Snaplist` identify windows based on the title and class name. The class name is an identifier used internally in Windows, so it is normally not visible. You can use AltDrag's configuration window to identify class names of windows. The entries are comma separated in the format `title|classname`. Do not use spaces when separating the entries, since the space will end up being included in the title. The matching is case-sensitive.

Omitting the title or classname will make it match an empty string. To make it ignore the title or class name, use the string `*`. This does not enable you to use wildcards, `*` is only valid when you want to completely ignore matching the title or the class name. Omitting the pipe character will also make it ignore the class name.

Borderless windows are not snapped to by default. These windows have to be specified in `Snaplist`.

Something worth mentioning is that when AltDrag conflicts with other applications, you can often press the alt button after you press the mouse button to avoid invoking AltDrag.


# Identifying windows

Since version 1.0, you can use AltDrag's configuration window to identify the title and classname of other windows. However, in some cases that little feature is not enough. If you need a better program for this task, try one of these:

- [WinSpy++](http://www.catch22.net/software/winspy-17)
- [AutoIt3 Window Spy](http://www.autohotkey.com/) (available in AutoHotkey)
- Spy++, a program packaged with Visual Studio (not Express Edition)


# Windows blacklisted by default

Here is an explanation of the default entries:

| `Blacklist`                                | Explanation |
| -----------                                | ----------- |
| <code>*&#124;TaskSwitcherWnd</code>        | This is part of the Alt+Tab window in Vista and Windows 7. |
| <code>*&#124;TaskSwitcherOverlayWnd</code> | This is part of the Alt+Tab window in Vista and Windows 7. |
| <code>&#124;#32770</code>                  | This is the system volume window. It is in this list so that you can still use the mouse wheel to change volume even if the mouse is not hovering the small window. |

| `Snaplist`                                 | Explanation |
| ----------                                 | ----------- |
| <code>*&#124;BaseWindow_RootWnd</code>     | The Winamp window. |
| <code>*&#124;SkinWnd</code>                | The Xfire main window. |
| <code>*&#124;ChatSkinWnd</code>            | The Xfire chat window. |
| <code>*&#124;SpotifyMainWindow</code>      | The Spotify window. |
| <code>*&#124;USurface_33202</code>         | The Steam windows. This identifier changes from version to version so you might have to update it manually. |

You should not need to edit or remove any of these entries. 


# List of applications

Here is a list of applications with their blacklist rule. Add the rule to `Blacklist` if you don't want to be able to drag it.

| Application   | Blacklist rule |
| -----------   | -------------- |
| Photoshop     | <code>*&#124;Photoshop,&#42;&#124;OWL.DocumentWindow</code> |
| Illustrator   | <code>*&#124;illustrator</code> |
| PuTTY         | <code>*&#124;PuTTY</code> |


# Improve this page

Post [on the subreddit](http://www.reddit.com/r/stefansundin/) to suggest improvements to this page.
