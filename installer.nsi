;Copyright (C) 2012  Stefan Sundin (recover89@gmail.com)
;
;This program is free software: you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.

;For silent install you can use these switches: /S /D=C:\installdir

;Requires AccessControl plug-in
;http://nsis.sourceforge.net/AccessControl_plug-in

!define APP_NAME      "AltDrag"
!define APP_VERSION   "1.0b2"
!define APP_URL       "http://code.google.com/p/altdrag/"
!define APP_UPDATEURL "http://altdrag.googlecode.com/svn/wiki/latest-unstable.txt"

; Libraries

!include "MUI2.nsh"
!include "Sections.nsh"
!include "LogicLib.nsh"
!include "FileFunc.nsh"

; General

Name "${APP_NAME} ${APP_VERSION}"
OutFile "build\${APP_NAME}-${APP_VERSION}.exe"
InstallDir "$PROGRAMFILES\${APP_NAME}"
InstallDirRegKey HKCU "Software\${APP_NAME}" "Install_Dir"
RequestExecutionLevel admin
ShowInstDetails hide
ShowUninstDetails show
SetCompressor /SOLID lzma

; Interface

!define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
!define MUI_LANGDLL_REGISTRY_KEY "Software\${APP_NAME}" 
!define MUI_LANGDLL_REGISTRY_VALUENAME "Language"

!define MUI_COMPONENTSPAGE_NODESC

!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_FUNCTION "Launch"

; Pages

Page custom PageUpgrade PageUpgradeLeave
!define MUI_PAGE_CUSTOMFUNCTION_PRE SkipPage
!define MUI_PAGE_CUSTOMFUNCTION_SHOW HideBackButton
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
Page custom PageAltShift
!define MUI_PAGE_CUSTOMFUNCTION_SHOW DisableBackButton
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Variables

Var UpgradeState

; Languages

!include "localization\installer.nsh"
!insertmacro MUI_RESERVEFILE_LANGDLL

!macro Lang lang id
${If} $LANGUAGE == ${id}
	WriteINIStr "$INSTDIR\${APP_NAME}.ini" "${APP_NAME}" "Language" "${lang}"
${EndIf}
!macroend

; Functions

!macro AddTray un
Function ${un}AddTray
	;Add tray icon if program is running
	FindWindow $0 "${APP_NAME}" ""
	IntCmp $0 0 done
		DetailPrint "Adding tray icon."
		System::Call "user32::RegisterWindowMessage(t 'AddTray') i .r1"
		SendMessage $0 $1 0 0 /TIMEOUT=500
	done:
FunctionEnd
!macroend
!insertmacro AddTray ""
!insertmacro AddTray "un."

!macro CloseApp un
Function ${un}CloseApp
	;Close app if running
	FindWindow $0 "${APP_NAME}" ""
	IntCmp $0 0 done
		DetailPrint "Closing running ${APP_NAME}."
		SendMessage $0 ${WM_CLOSE} 0 0 /TIMEOUT=500
		waitloop:
			Sleep 10
			FindWindow $0 "${APP_NAME}" ""
			IntCmp $0 0 closed waitloop waitloop
	closed:
	Sleep 100 ;Sleep a little extra to let Windows do its thing
	;If HookWindows is enabled, sleep even longer
	ReadINIStr $0 "$INSTDIR\${APP_NAME}.ini" "${APP_NAME}" "HookWindows"
	${If} $0 == "1"
		Sleep 1000
	${EndIf}
	done:
FunctionEnd
!macroend
!insertmacro CloseApp ""
!insertmacro CloseApp "un."

; Installer

Section "" sec_update
	NSISdl::download /TIMEOUT=5000 "${APP_UPDATEURL}" "$TEMP\${APP_NAME}-updatecheck"
	Pop $0
	StrCmp $0 "success" +3
		DetailPrint "Update check failed. Error: $0."
		Goto done
	FileOpen $0 "$TEMP\${APP_NAME}-updatecheck" r
	IfErrors done
	FileRead $0 $1
	FileClose $0
	Delete /REBOOTOK "$TEMP\${APP_NAME}-updatecheck"
	StrCmp $1 ${APP_VERSION} done
		MessageBox MB_ICONINFORMATION|MB_YESNO "$(L10N_UPDATE_DIALOG)" /SD IDNO IDNO done
			ExecShell "open" "${APP_URL}"
			Quit
	done:
SectionEnd

Section "" sec_app
	;Close app if running
	Call CloseApp
	
	SetOutPath "$INSTDIR"
	
	;Rename old ini file if it exists
	IfFileExists "${APP_NAME}.ini" 0 +3
		Delete "${APP_NAME}-old.ini"
		Rename "${APP_NAME}.ini" "${APP_NAME}-old.ini"
	
	;Delete files that existed in earlier versions
	Delete /REBOOTOK "$INSTDIR\info.txt" ;existed in <= 0.9
	Delete /REBOOTOK "$INSTDIR\Config.exe" ;existed in 1.0b1
	
	;Install files
	File "build\en-US\${APP_NAME}\${APP_NAME}.exe"
	File "build\en-US\${APP_NAME}\${APP_NAME}.ini"
	File "build\en-US\${APP_NAME}\hooks.dll"
	File /nonfatal "build\en-US\${APP_NAME}\HookWindows_x64.exe"
	File /nonfatal "build\en-US\${APP_NAME}\hooks_x64.dll"
	
	!insertmacro Lang en-US ${LANG_ENGLISH}
	!insertmacro Lang fr-FR ${LANG_FRENCH}
	!insertmacro Lang pl-PL ${LANG_POLISH}
	!insertmacro Lang pt-BR ${LANG_PORTUGUESEBR}
	!insertmacro Lang ru-RU ${LANG_RUSSIAN}
	!insertmacro Lang sk-SK ${LANG_SLOVAK}
	!insertmacro Lang zh-CN ${LANG_SIMPCHINESE}
	
	;Deactivate CheckOnStartup if check for update was deselected
	${IfNot} ${SectionIsSelected} ${sec_update}
		WriteINIStr "$INSTDIR\${APP_NAME}.ini" "Update" "CheckOnStartup" "0"
	${EndIf}
	
	;Grant write rights to ini file to all users
	AccessControl::GrantOnFile "$INSTDIR\${APP_NAME}.ini" "(BU)" "FullAccess"
	
	;Update registry
	WriteRegStr HKCU "Software\${APP_NAME}" "Install_Dir" "$INSTDIR"
	WriteRegStr HKCU "Software\${APP_NAME}" "Version" "${APP_VERSION}"
	
	;Create uninstaller
	WriteUninstaller "Uninstall.exe"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "UninstallString" '"$INSTDIR\Uninstall.exe"'
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "QuietUninstallString" '"$INSTDIR\Uninstall.exe" /S'
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "DisplayName" "${APP_NAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "DisplayIcon" '"$INSTDIR\${APP_NAME}.exe"'
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "DisplayVersion" "${APP_VERSION}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "HelpLink" "${APP_URL}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "Publisher" "Stefan Sundin"
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "NoModify" 1
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "NoRepair" 1
	
	;Compute size for uninstall information
	${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
	IntFmt $0 "0x%08X" $0
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "EstimatedSize" "$0"
SectionEnd

Section "" sec_shortcut
	CreateShortCut "$SMPROGRAMS\${APP_NAME}.lnk" "$INSTDIR\${APP_NAME}.exe" "" "$INSTDIR\${APP_NAME}.exe" 0
SectionEnd

Function Launch
	Exec "$INSTDIR\${APP_NAME}.exe"
FunctionEnd

;Alt+Shift notification
Function PageAltShift
	ClearErrors
	ReadRegStr $0 HKCU "Keyboard Layout\Toggle" "Language Hotkey"
	IfErrors prompt
	ReadRegStr $1 HKCU "Keyboard Layout\Toggle" "Layout Hotkey"
	${If} $0 != "1"
	${AndIf} $1 != "1"
		Abort
	${EndIf}
	
	prompt:
	nsDialogs::Create 1018
	!insertmacro MUI_HEADER_TEXT "$(L10N_ALTSHIFT_TITLE)" "$(L10N_ALTSHIFT_SUBTITLE)"
	${NSD_CreateLabel} 0 0 100% 143 "$(L10N_ALTSHIFT_HEADER)"
	${NSD_CreateButton} 0 162 92u 17u "$(L10N_ALTSHIFT_BUTTON)"
	Pop $0
	${NSD_OnClick} $0 OpenKeyboardSettings
	
	;Disable Cancel button
	GetDlgItem $0 $HWNDPARENT 2
	EnableWindow $0 0
	
	;Disable x button
	System::Call "user32::GetSystemMenu(i $HWNDPARENT, i 0) i .r1"
	System::Call "user32::EnableMenuItem(i $1, i 0xF060, i 1) v"
	
	nsDialogs::Show
FunctionEnd

Function OpenKeyboardSettings
	Exec "rundll32.exe shell32.dll,Control_RunDLL input.dll,,{C07337D3-DB2C-4D0B-9A93-B722A6C106E2}{HOTKEYS}"
FunctionEnd

; Detect previous installation

Var Upgradebox
Var Uninstallbox

Function PageUpgrade
	IfFileExists $INSTDIR +2
		Abort
	
	nsDialogs::Create 1018
	!insertmacro MUI_HEADER_TEXT "$(L10N_UPGRADE_TITLE)" "$(L10N_UPGRADE_SUBTITLE)"
	${NSD_CreateLabel} 0 0 100% 20u "$(L10N_UPGRADE_HEADER)"
	
	${NSD_CreateRadioButton} 0 45 100% 10u "$(L10N_UPGRADE_UPGRADE)"
	Pop $Upgradebox
	${NSD_Check} $Upgradebox
	${NSD_CreateLabel} 16 62 100% 20u "$(L10N_UPGRADE_INI)"
	
	${NSD_CreateRadioButton} 0 95 100% 10u "$(L10N_UPGRADE_INSTALL)"
	
	${NSD_CreateRadioButton} 0 130 100% 10u "$(L10N_UPGRADE_UNINSTALL)"
	Pop $Uninstallbox
	
	nsDialogs::Show
FunctionEnd

Function PageUpgradeLeave
	${NSD_GetState} $Uninstallbox $0
	${If} $0 == ${BST_CHECKED}
		Exec "$INSTDIR\Uninstall.exe"
		Quit
	${EndIf}
	
	${NSD_GetState} $Upgradebox $UpgradeState
	${If} $UpgradeState == ${BST_CHECKED}
		ReadINIStr $0 "$INSTDIR\${APP_NAME}.ini" "Update" "CheckOnStartup"
		${If} $0 == "0"
			!insertmacro UnselectSection ${sec_update}
		${EndIf}
	${EndIf}
FunctionEnd

;Used when upgrading to skip the components and directory pages
Function SkipPage
	${If} $UpgradeState == ${BST_CHECKED}
		!insertmacro UnselectSection ${sec_shortcut}
		Abort
	${EndIf}
FunctionEnd

Function HideBackButton
	GetDlgItem $0 $HWNDPARENT 3
	ShowWindow $0 ${SW_HIDE}
FunctionEnd

Function DisableBackButton
	GetDlgItem $0 $HWNDPARENT 3
	EnableWindow $0 0
FunctionEnd

Function .onInit
	;Display language selection and add tray if program is running
	!insertmacro MUI_LANGDLL_DISPLAY
	Call AddTray
	
	;Handle silent install
	IfSilent 0 done
		!insertmacro UnselectSection ${sec_update}
	done:
FunctionEnd

Function .onInstSuccess
	;Run program if silent
	IfSilent 0 +2
		Call Launch
FunctionEnd

; Uninstaller

Function un.onInit
	!insertmacro MUI_UNGETLANGUAGE
	Call un.AddTray
FunctionEnd

Section "Uninstall"
	Call un.CloseApp

	Delete /REBOOTOK "$INSTDIR\${APP_NAME}.exe"
	Delete /REBOOTOK "$INSTDIR\hooks.dll"
	Delete /REBOOTOK "$INSTDIR\HookWindows_x64.exe"
	Delete /REBOOTOK "$INSTDIR\hooks_x64.dll"
	Delete /REBOOTOK "$INSTDIR\${APP_NAME}.ini"
	Delete /REBOOTOK "$INSTDIR\${APP_NAME}-old.ini"
	Delete /REBOOTOK "$INSTDIR\Uninstall.exe"
	RMDir  /REBOOTOK "$INSTDIR"

	Delete /REBOOTOK "$SMPROGRAMS\${APP_NAME}.lnk"

	DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "${APP_NAME}"
	DeleteRegKey /ifempty HKCU "Software\${APP_NAME}"
	DeleteRegKey /ifempty HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"
SectionEnd
