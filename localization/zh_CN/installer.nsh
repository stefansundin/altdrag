; AltDrag - zh-CN localization by Jack Jin (superjwl@gmail.com), alex310110, Zepp Lu
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.

!insertmacro MUI_LANGUAGE "SimpChinese"
LangString L10N_LANG ${LANG_SIMPCHINESE} ""

LangString L10N_UPGRADE_TITLE     0 "软件已安装"
LangString L10N_UPGRADE_SUBTITLE  0 "选择如何安装 ${APP_NAME}。"
LangString L10N_UPGRADE_HEADER    0 "系统中已装有 ${APP_NAME}。选择您想进行的操作，点击下一步继续。"
LangString L10N_UPGRADE_UPGRADE   0 "把 ${APP_NAME} 升级到 ${APP_VERSION}(&U)。"
LangString L10N_UPGRADE_INI       0 "您的旧配置将被保存为 ${APP_NAME}-old.ini。"
LangString L10N_UPGRADE_INSTALL   0 "安装到新的位置(&I)。"
LangString L10N_UPGRADE_UNINSTALL 0 "卸载 ${APP_NAME}(&T)。"

LangString L10N_ALTSHIFT_TITLE    0 "快捷键"
LangString L10N_ALTSHIFT_SUBTITLE 0 "快捷键 Alt + Shift 和 ${APP_NAME} 冲突。"
LangString L10N_ALTSHIFT_HEADER   0 "安装程序检测到当前 Windows 切换键盘布局的快捷键为 Alt + Shift。$\n$\n而在使用 ${APP_NAME} 时，按住 Shift 键可以在窗口之间自动吸附。换言之，当您使用 Alt 键拖动窗口时，同时又按下了 Shift 键，键盘布局就有可能会被切换。${APP_NAME} 内部的机制会试图阻止这种意外情形，但这会导致您无法通过 Shift 键来自动吸附到窗口。$\n$\n您可以禁止切换键盘布局的快捷键或将其改成另一个值。点击下一步继续。"
LangString L10N_ALTSHIFT_BUTTON   0 "打开键盘设置(&O)"

LangString L10N_HOOKTIMEOUT_TITLE          0 "注册表优化"
LangString L10N_HOOKTIMEOUT_SUBTITLE       0 "注册表优化是可选的，能够避免 ${APP_NAME} 被意外中止。"
LangString L10N_HOOKTIMEOUT_HEADER         0 "微软在 Windows 7 中增加了一项功能，用于停用长时间无响应的键盘和鼠标钩子。但这个行为有时会出错，特别是当电脑长时间地休眠、睡眠或锁定时。$\n$\n这种情况发生时，${APP_NAME} 将会不发出任何警告而停止工作，您需要先停用再重新启用 ${APP_NAME} 来使之正常工作。$\n$\n该注册表优化可以延长 Windows 等待钩子响应的时间，您可以通过下面的按钮来启用或停用这个优化。请注意，这完全是个可选的优化。"
LangString L10N_HOOKTIMEOUT_APPLYBUTTON    0 "启用注册表优化(&E)"
LangString L10N_HOOKTIMEOUT_REVERTBUTTON   0 "禁用注册表优化(&D)"
LangString L10N_HOOKTIMEOUT_ALREADYAPPLIED 0 "注册表优化已启用。"
LangString L10N_HOOKTIMEOUT_FOOTER         0 "改动将会在您下次登录时生效。"
