AltDrag application coded by [@stefanstundin](https://github.com/stefansundin/altdrag).  

This repository contains updated build options to enable some of the `gcc/ld` hardening options.

```powershell
> Get-PESecurity -Dir .

FileName         : X:\altdrag\bin\AltDrag.exe
ARCH             : I386
DotNET           : False
ASLR             : True
DEP              : True
Authenticode     : False
StrongNaming     : N/A
SafeSEH          : False
ControlFlowGuard : False
HighentropyVA    : False

FileName         : X:\altdrag\bin\hooks.dll
ARCH             : I386
DotNET           : False
ASLR             : True
DEP              : True
Authenticode     : False
StrongNaming     : N/A
SafeSEH          : False
ControlFlowGuard : False
HighentropyVA    : False

FileName         : X:\altdrag\bin\hooks_x64.dll
ARCH             : AMD64
DotNET           : False
ASLR             : True
DEP              : True
Authenticode     : False
StrongNaming     : N/A
SafeSEH          : N/A
ControlFlowGuard : False
HighentropyVA    : True

FileName         : X:\altdrag\bin\HookWindows_x64.exe
ARCH             : AMD64
DotNET           : False
ASLR             : True
DEP              : True
Authenticode     : False
StrongNaming     : N/A
SafeSEH          : N/A
ControlFlowGuard : False
HighentropyVA    : True
```
