# AltDrag.exe

AltDrag has historically only had one command line option; `-hide`. With the release of 1.0 beta 2, several new options were added. Please note that all options are case-sensitive.

| Short option | Long option   | Explanation |
| ------------ | -----------   | ----------- |
| `-h`         | `-hide`       | Do not add tray icon, hide it if already running. |
| `-q`         | `-quiet`      | Do nothing if already running (i.e. do not open configuration window or refresh settings). |
| `-e`         | `-elevate`    | Request elevation to administrator privileges when run. |
| `-c [n]`     | `-config [n]` | Open the configuration window (optionally go to page number `n`). |


# Installer

The installer accepts a few options. It has been designed to be able to run silently, which means it should be easy to deploy in large networks.

| Option             | Explanation |
| ------             | ----------- |
| `/S`               | Run silently. The installer will not check for updates. |
| `/L=fr-FR`         | Use supplied language. |
| `/D=C:\installdir` | Set destination directory. It must be the last parameter and must not contain any quotes, even if the path contains spaces. Only absolute paths are supported. |


# Improve this page

Post [in the subreddit](http://www.reddit.com/r/stefansundin/) to suggest improvements to this page.
