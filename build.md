# Introduction

This document explains how to compile AltDrag. There are two distinct parts that you have to compile separately, the 32-bit part and the 64-bit part.

I recommend that you build AltDrag using mingw-w64 inside [Cygwin](https://cygwin.com/).

## Cygwin

In the Cygwin installer, install the following packages:
- `git`
- `mingw64-i686-gcc-core`
- `mingw64-x86_64-gcc-core`

Once installed, open the Cygwin terminal and type:
```
git clone https://github.com/stefansundin/altdrag.git
cd altdrag
./build.bat x64
explorer .
```

This should get the source code and build all the binaries. The last command should open the explorer window so you can see if you find:
- `AltDrag.exe`
- `hooks.dll`
- `HookWindows_x64.exe`
- `hooks_x64.dll`

You can also run `./build.bat run` to automatically launch AltDrag after it has been built.

### Installer

If you want to build the installer, you need to download [NSIS Unicode](http://www.scratchpaper.com/) and the [AccessControl plug-in](http://nsis.sourceforge.net/AccessControl_plug-in). Put NSIS in PATH (restart your terminal to update PATH), and then run `./build.bat all x64` to automatically build the installer after the binaries.

### GUI

The configuration window was made with [ResEdit](http://www.resedit.net/), a compact resource editor. ResEdit is not needed to build AltDrag.

### Languages

Run `./build.bat lang` to generate `strings.h` files from `Translate.ini` files. This is used to hard-code new translations before a release. New languages have to be added to `import_languages.c` to be generated.


## Cygwin-less

If you do not want to use Cygwin, you can get mingw-w64 and Git standalone:
- https://msysgit.github.io/
- https://code.google.com/p/tortoisegit/wiki/Download
- http://mingw-w64.sourceforge.net/

If you have trouble downloading the code, you can try [downloading a zip](https://github.com/stefansundin/altdrag/zipball/master).


### Configure PATH

After downloading the compilers, you need to add them to the PATH variable so the build script can use the them. Do this by _right clicking_ on the _Computer_ desktop icon, go to the _Advanced_ tab, press the _Environment variables_ button, locate the PATH variable and add _<mingw-w32 path>\bin_ to the list, and press ok on all the dialogs. If you want to build the 64-bit part, add mingw-w64 to the PATH as well.

Here is an example of my PATH variable: `C:\mingw-w32\bin;C:\mingw-w64\bin`

Now open `cmd.exe` and go to AltDrag's directory. If you had the terminal open when you changed the PATH variable, you must restart it.

### 32-bit part

The 32-bit part is `AltDrag.exe` and `hooks.dll`. To compile them simply run `build.bat`. It's convenient to run `build` in a terminal so you can see any error messages.

### 64-bit part

The 64-bit part is `HookWindows_x64.exe` and `hooks_x64.dll`. These files are only used if you enable HookWindows and are running x64. They need to be 64-bit so that they can hook into 64-bit processes. To compile them you must use mingw-w64. Run `build x64` to build the files.

Note that `AltDrag.exe` and `hooks.dll` are not supposed to be compiled as 64-bit programs. You still need a 32-bit component to be able to use HookWindows for 32-bit processes, and because there is a prevalence of 32-bit programs that we use even on 64-bit Windows, it doesn't make sense to make AltDrag 64-bit only (yet).


### Video walkthrough

There's an old [video walkthrough](https://www.youtube.com/watch?v=4ENwQxSr_So) that might be slightly useful. It actually explains how to build another program of mine and it was made before I moved to GitHub and it doesn't use Cygwin.


# Improve this page

Post [in the subreddit](http://www.reddit.com/r/stefansundin/) to suggest improvements to this page.
