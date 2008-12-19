;AltDrag installer
;NSIS Unicode: http://www.scratchpaper.com/
;
;Copyright (C) 2008  Stefan Sundin (recover89@gmail.com)
;
;This program is free software: you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.


!define APP_NAME      "AltDrag"
!define APP_VERSION   "0.5"
!define APP_URL       "http://altdrag.googlecode.com/"
!define APP_UPDATEURL "http://altdrag.googlecode.com/svn/wiki/latest-stable.txt"
!define L10N_VERSION  1

;General

!include "MUI2.nsh"

Name "${APP_NAME}"
OutFile "build/${APP_NAME}-${APP_VERSION} (Installer).exe"
InstallDir "$PROGRAMFILES\${APP_NAME}"
InstallDirRegKey HKCU "Software\${APP_NAME}" "Install_Dir"
RequestExecutionLevel user
ShowInstDetails hide
ShowUninstDetails show
SetCompressor lzma

;Interface

!define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
!define MUI_LANGDLL_REGISTRY_KEY "Software\${APP_NAME}" 
!define MUI_LANGDLL_REGISTRY_VALUENAME "Language"

!define MUI_COMPONENTSPAGE_NODESC

!define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
;!define MUI_FINISHPAGE_SHOWREADME_TEXT "Read info.txt"
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\info.txt"
!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_FUNCTION "Launch"

;Pages

!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

;Languages

!include "localization\installer.nsh"

!insertmacro MUI_RESERVEFILE_LANGDLL

;Installer

Section "$(L10N_UPDATE_SECTION)"
	NSISdl::download "${APP_UPDATEURL}" "$TEMP\${APP_NAME}-updatecheck"
	Pop $R0
		StrCmp $R0 "success" +3
		DetailPrint "Update check failed. Error: $R0"
		Goto done
	FileOpen $0 "$TEMP\${APP_NAME}-updatecheck" r
	IfErrors done
	FileRead $0 $1
	FileClose $0
	Delete /REBOOTOK "$TEMP\${APP_NAME}-updatecheck"
	StrCmp ${APP_VERSION} $1 done 0
	MessageBox MB_ICONINFORMATION|MB_YESNO "$(L10N_UPDATE_DIALOG)" IDNO done
		ExecShell "open" "${APP_URL}"
		Quit
	done:
SectionEnd

Section "${APP_NAME} (${APP_VERSION})"
	SectionIn RO

	FindWindow $0 "${APP_NAME}" ""
	IntCmp $0 0 continue
		MessageBox MB_ICONINFORMATION|MB_YESNO "$(L10N_RUNNING_INSTALL)" IDNO continue
			DetailPrint "$(L10N_CLOSING)"
			SendMessage $0 ${WM_CLOSE} 0 0
			Sleep 200
	continue:

	SetOutPath "$INSTDIR"

	;Store directory and version
	WriteRegStr HKCU "Software\${APP_NAME}" "Install_Dir" "$INSTDIR"
	WriteRegStr HKCU "Software\${APP_NAME}" "Version" "${APP_VERSION}"

	IntCmp $LANGUAGE ${LANG_ENGLISH} english
	english:
		File "build\en-US\${APP_NAME}\*"
		Goto files_installed

	files_installed:

	;Create uninstaller
	WriteUninstaller "Uninstall.exe"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "DisplayName" "${APP_NAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "UninstallString" '"$INSTDIR\Uninstall.exe"'
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "NoModify" 1
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "NoRepair" 1
SectionEnd

Section "$(L10N_SHORTCUT)"
	CreateShortCut "$SMPROGRAMS\${APP_NAME}.lnk" "$INSTDIR\${APP_NAME}.exe" "" "$INSTDIR\${APP_NAME}.exe" 0
SectionEnd

Section /o "$(L10N_AUTOSTART)"
	MessageBox MB_ICONINFORMATION|MB_YESNO|MB_DEFBUTTON2 "$(L10N_AUTOSTART_HIDE)" IDYES hide
		WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "${APP_NAME}" '"$INSTDIR\${APP_NAME}.exe"'
		Goto continue
	hide:
		WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "${APP_NAME}" '"$INSTDIR\${APP_NAME}.exe" -hide'
	continue:
SectionEnd

Function Launch
	Exec "$INSTDIR\${APP_NAME}.exe"
FunctionEnd

Function .onInit
	!insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

;Uninstaller

Section "Uninstall"
	FindWindow $0 "${APP_NAME}" ""
	IntCmp $0 0 continue
		MessageBox MB_ICONINFORMATION|MB_YESNO "$(L10N_RUNNING_UNINSTALL)" IDNO continue
			DetailPrint "$(L10N_CLOSING)"
			SendMessage $0 ${WM_CLOSE} 0 0
			Sleep 200
	continue:

	Delete /REBOOTOK "$INSTDIR\${APP_NAME}.exe"
	Delete /REBOOTOK "$INSTDIR\${APP_NAME}.ini"
	Delete /REBOOTOK "$INSTDIR\info.txt"
	Delete /REBOOTOK "$INSTDIR\Uninstall.exe"
	RMDir /REBOOTOK "$INSTDIR"

	Delete /REBOOTOK "$SMPROGRAMS\${APP_NAME}.lnk"

	DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "${APP_NAME}"
	DeleteRegKey /ifempty HKCU "Software\${APP_NAME}"
	DeleteRegKey /ifempty HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"
SectionEnd

Function un.onInit
	!insertmacro MUI_UNGETLANGUAGE
FunctionEnd
