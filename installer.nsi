;Copyright (C) 2010  Stefan Sundin (recover89@gmail.com)
;
;This program is free software: you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.


!define APP_NAME      "AltDrag"
!define APP_VERSION   "0.8"
!define APP_URL       "http://code.google.com/p/altdrag/"
!define APP_UPDATEURL "http://altdrag.googlecode.com/svn/wiki/latest-stable.txt"

; Libraries

!include "MUI2.nsh"
!include "Sections.nsh"
!include "LogicLib.nsh"
!include "StrFunc.nsh"
${StrLoc}

; General

Name "${APP_NAME} ${APP_VERSION}"
OutFile "build/${APP_NAME}-${APP_VERSION}.exe"
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

!define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\info.txt"
!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_FUNCTION "Launch"

; Pages

Page custom PageUpgrade PageUpgradeLeave
!define MUI_PAGE_CUSTOMFUNCTION_PRE SkipPage
!insertmacro MUI_PAGE_COMPONENTS
!define MUI_PAGE_CUSTOMFUNCTION_PRE SkipPage
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
Page custom PageAltShift
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Variables

Var UpgradeState
Var AutostartSectionState ;Helps keep track of the autostart checkboxes

; Languages

!include "localization\installer.nsh"
!insertmacro MUI_RESERVEFILE_LANGDLL

!macro Lang id lang
${If} $LANGUAGE == ${id}
	File "build\${lang}\${APP_NAME}\info.txt"
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
		SendMessage $0 $1 0 0
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
		${If} $UpgradeState != ${BST_CHECKED}
			StrCpy $1 "$(L10N_RUNNING)"
			${If} "${un}" == "un."
				StrCpy $1 "$1$\n$(L10N_RUNNING_UNINSTALL)"
			${EndIf}
			MessageBox MB_ICONINFORMATION|MB_YESNO "$1" /SD IDYES IDNO done
		${EndIf}
		DetailPrint "Closing running ${APP_NAME}."
		SendMessage $0 ${WM_CLOSE} 0 0
		waitloop:
			Sleep 10
			FindWindow $0 "${APP_NAME}" ""
			IntCmp $0 0 closed waitloop waitloop
	closed:
	Sleep 500 ;Sleep a little extra to let Windows do its thing.
	;This Sleep value is a little higher than normal since HookWindows can take a while to fully unload
	done:
FunctionEnd
!macroend
!insertmacro CloseApp ""
!insertmacro CloseApp "un."

; Detect previous installation

Var Upgradebox
Var Uninstallbox

Function PageUpgrade
	ReadRegStr $0 HKCU "Software\${APP_NAME}" "Install_Dir"
	IfFileExists $0 +2
		Abort
	
	nsDialogs::Create 1018
	!insertmacro MUI_HEADER_TEXT "$(L10N_UPGRADE_TITLE)" "$(L10N_UPGRADE_SUBTITLE)"
	${NSD_CreateLabel} 0 0 100% 20u "$(L10N_UPGRADE_HEADER)"
	
	${NSD_CreateRadioButton} 0 45 100% 10u "$(L10N_UPGRADE_UPGRADE)"
	Pop $Upgradebox
	${NSD_CreateLabel} 16 60 100% 20u "$(L10N_UPGRADE_INI)"
	
	${NSD_CreateRadioButton} 0 95 100% 10u "$(L10N_UPGRADE_INSTALL)"
	Pop $0
	
	${NSD_CreateRadioButton} 0 130 100% 10u "$(L10N_UPGRADE_UNINSTALL)"
	Pop $Uninstallbox
	
	;Check the correct button when going back to this page
	${If} $UpgradeState == ${BST_UNCHECKED}
		${NSD_Check} $0
	${Else}
		${NSD_Check} $Upgradebox
	${EndIf}
	
	nsDialogs::Show
FunctionEnd

Function PageUpgradeLeave
	${NSD_GetState} $Upgradebox $UpgradeState
	${NSD_GetState} $Uninstallbox $0
	${If} $0 == ${BST_CHECKED}
		Exec "$INSTDIR\Uninstall.exe"
		Quit
	${EndIf}
FunctionEnd

; Installer

Section "$(L10N_UPDATE_SECTION)" sec_update
	NSISdl::download "${APP_UPDATEURL}" "$TEMP\${APP_NAME}-updatecheck"
	Pop $0
	StrCmp $0 "success" +3
		DetailPrint "Update check failed. Error: $0."
		Goto done
	FileOpen $0 "$TEMP\${APP_NAME}-updatecheck" r
	IfErrors done
	FileRead $0 $1
	FileClose $0
	Delete /REBOOTOK "$TEMP\${APP_NAME}-updatecheck"
	${If} $1 > ${APP_VERSION}
		MessageBox MB_ICONINFORMATION|MB_YESNO "$(L10N_UPDATE_DIALOG)" /SD IDNO IDNO done
			ExecShell "open" "${APP_URL}"
			Quit
	${EndIf}
	done:
SectionEnd

Section "${APP_NAME}" sec_app
	SectionIn RO
	
	;Close app if running
	Call CloseApp
	
	SetOutPath "$INSTDIR"
	
	;Store directory and version
	WriteRegStr HKCU "Software\${APP_NAME}" "Install_Dir" "$INSTDIR"
	WriteRegStr HKCU "Software\${APP_NAME}" "Version" "${APP_VERSION}"
	
	;Rename old ini file if it exists
	IfFileExists "${APP_NAME}.ini" 0 +2
		Rename "${APP_NAME}.ini" "${APP_NAME}-old.ini"
	
	;Install files
	File "build\en-US\${APP_NAME}\${APP_NAME}.exe"
	File "build\en-US\${APP_NAME}\hooks.dll"
	File /nonfatal "build\en-US\${APP_NAME}\HookWindows_x64.exe"
	File /nonfatal "build\en-US\${APP_NAME}\hooks_x64.dll"
	File "build\en-US\${APP_NAME}\${APP_NAME}.ini"
	
	!insertmacro Lang ${LANG_ENGLISH}      en-US
	!insertmacro Lang ${LANG_SPANISH}      es-ES
	!insertmacro Lang ${LANG_GALICIAN}     gl-ES
	;!insertmacro Lang ${LANG_KOREAN}       ko-KR
	!insertmacro Lang ${LANG_RUSSIAN}      ru-RU
	
	;Create uninstaller
	WriteUninstaller "Uninstall.exe"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "UninstallString" '"$INSTDIR\Uninstall.exe"'
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "DisplayName" "${APP_NAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "DisplayIcon" '"$INSTDIR\${APP_NAME}.exe"'
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "DisplayVersion" "${APP_VERSION}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "HelpLink" "${APP_URL}"
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "NoModify" 1
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "NoRepair" 1
SectionEnd

Section "$(L10N_SHORTCUT)" sec_shortcut
	CreateShortCut "$SMPROGRAMS\${APP_NAME}.lnk" "$INSTDIR\${APP_NAME}.exe" "" "$INSTDIR\${APP_NAME}.exe" 0
SectionEnd

SectionGroup /e "$(L10N_AUTOSTART)"
	Section /o "$(L10N_AUTOSTART)" sec_autostart
	SectionEnd
	Section /o "$(L10N_AUTOSTART_HIDE)" sec_hide
	SectionEnd
SectionGroupEnd

Function Launch
	Exec "$INSTDIR\${APP_NAME}.exe"
FunctionEnd

;Alt+Shift notification
Function PageAltShift
	ClearErrors
	ReadRegStr $0 HKCU "Keyboard Layout\Toggle" "Language Hotkey"
	IfErrors check
	ReadRegStr $1 HKCU "Keyboard Layout\Toggle" "Layout Hotkey"
	${If} $0 != "1"
	${AndIf} $1 != "1"
		Abort
	${EndIf}
	
	check:
	StrCpy $0 0
	loop:
		ClearErrors
		EnumRegValue $1 HKCU "Keyboard Layout\Preload" $0
		IfErrors done
		IntOp $0 $0 + 1
		Goto loop
	done:
	${If} $0 == 1
		Abort
	${EndIf}
	
	nsDialogs::Create 1018
	!insertmacro MUI_HEADER_TEXT "$(L10N_ALTSHIFT_TITLE)" "$(L10N_ALTSHIFT_SUBTITLE)"
	${NSD_CreateLabel} 0 0 100% 143 "$(L10N_ALTSHIFT_HEADER)"
	${NSD_CreateButton} 0 144 90u 17u "$(L10N_ALTSHIFT_BUTTON)"
	Pop $0
	${NSD_OnClick} $0 OpenKeyboardSettings
	
	nsDialogs::Show
FunctionEnd

Function OpenKeyboardSettings
	Exec "RunDll32.exe shell32.dll,Control_RunDLL input.dll"
FunctionEnd

;Used when upgrading to skip the components and directory pages
Function SkipPage
	${If} $UpgradeState == ${BST_CHECKED}
		!insertmacro UnselectSection ${sec_shortcut}
		Abort
	${EndIf}
FunctionEnd

Function .onInit
	;Display language selection and add tray if program is running
	!insertmacro MUI_LANGDLL_DISPLAY
	Call AddTray
	;If silent, deselect check for update
	IfSilent 0 autostart_check
		!insertmacro UnselectSection ${sec_update}
	autostart_check:
	;Determine current autostart setting
	StrCpy $AutostartSectionState 0
	ReadRegStr $0 HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "${APP_NAME}"
	IfErrors done
		!insertmacro SelectSection ${sec_autostart}
		${StrLoc} $0 $0 "-hide" "<"
		${If} $0 != ""
			StrCpy $AutostartSectionState 1
			!insertmacro SelectSection ${sec_hide}
		${EndIf}
	done:
FunctionEnd

Function .onSelChange
	;Hide tray automatically checks Autostart
	${If} ${SectionIsSelected} ${sec_hide}
		${If} $AutostartSectionState == 0
			StrCpy $AutostartSectionState 1
			!insertmacro SelectSection ${sec_autostart}
		${ElseIfNot} ${SectionIsSelected} ${sec_autostart}
			StrCpy $AutostartSectionState 0
			!insertmacro UnselectSection ${sec_hide}
		${EndIf}
	${Else}
		StrCpy $AutostartSectionState 0
	${EndIf}
FunctionEnd

Function .onInstSuccess
	;Set or remove autostart
	${If} ${SectionIsSelected} ${sec_hide}
		WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "${APP_NAME}" '"$INSTDIR\${APP_NAME}.exe" -hide'
	${ElseIf} ${SectionIsSelected} ${sec_autostart}
		WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "${APP_NAME}" '"$INSTDIR\${APP_NAME}.exe"'
	${Else}
		DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "${APP_NAME}"
	${EndIf}
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
	Delete /REBOOTOK "$INSTDIR\info.txt"
	Delete /REBOOTOK "$INSTDIR\Uninstall.exe"
	RMDir  /REBOOTOK "$INSTDIR"

	Delete /REBOOTOK "$SMPROGRAMS\${APP_NAME}.lnk"

	DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "${APP_NAME}"
	DeleteRegKey /ifempty HKCU "Software\${APP_NAME}"
	DeleteRegKey /ifempty HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"
SectionEnd
