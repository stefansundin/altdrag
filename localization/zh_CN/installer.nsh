; AltDrag - zh-CN localization by Jack Jin (superjwl@gmail.com) and alex310110
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.

!insertmacro MUI_LANGUAGE "Simplified Chinese" ; English name of this language
LangString L10N_LANG ${LANG_SIMPCHINESE} ""

LangString L10N_UPGRADE_TITLE     0 "已经安装"
LangString L10N_UPGRADE_SUBTITLE  0 "选择如何安装${APP_NAME}。"
LangString L10N_UPGRADE_HEADER    0 "${APP_NAME}已经安装在你的系统中。选择你想进行的操作，并点击下一步继续。"
LangString L10N_UPGRADE_UPGRADE   0 "(&U)把${APP_NAME}升级到${APP_VERSION}。"
LangString L10N_UPGRADE_INI       0 "你的旧配置将被保存为${APP_NAME}-old.ini。"
LangString L10N_UPGRADE_INSTALL   0 "(&I)安装到新的位置。"
LangString L10N_UPGRADE_UNINSTALL 0 "(&t)卸载${APP_NAME}。"

LangString L10N_ALTSHIFT_TITLE    0 "快捷键"
LangString L10N_ALTSHIFT_SUBTITLE 0 "快捷键Alt + Shift和${APP_NAME}冲突。"
LangString L10N_ALTSHIFT_HEADER   0 "安装程序检测到当前windows切换键盘布局的快捷键为Alt + Shift。$\n$\n而在使用${APP_NAME}时，按住shift键可以在窗口之间相互吸附。所以切换键盘布局和窗口相互吸附这两个功能的快捷键相同。当${APP_NAME}内部试图阻止意外的键盘布局切换时，会造成按住shift键移动窗口操作的失败。$\n$\n此时，你可以禁止或修改切换键盘布局的快捷键。点击下一步继续。"
LangString L10N_ALTSHIFT_BUTTON   0 "(&O)打开键盘设置"

LangString L10N_HOOKTIMEOUT_TITLE          0 "注册表优化"
LangString L10N_HOOKTIMEOUT_SUBTITLE       0 "注山表优化是可选的，以保持${APP_NAME}不会意外中止。"
LangString L10N_HOOKTIMEOUT_HEADER         0 "在Windows 7中，微软加入了一个特性，如果键盘和鼠标勾子长时间没有响应，则会停用它们。不幸的是，这个检查会出错，特别是当你对电脑进行长时间休眠，睡眠或锁定时。$\n$\n如果发生这种情况，你会发现${APP_NAME}没有任何提示而停止工作，你必须先停用并再启用${APP_NAME}使之再次工作。$\n$\n有一个注册表项可以设定windows7等待hooks响应的时间，你可以通过下面的按钮来启用或停用这个注册表项。请注意，这个注册表项完全是可选的。"
LangString L10N_HOOKTIMEOUT_APPLYBUTTON    0 "&E启用注册表优化"
LangString L10N_HOOKTIMEOUT_REVERTBUTTON   0 "&D禁用注册表优化"
LangString L10N_HOOKTIMEOUT_ALREADYAPPLIED 0 "注册表优化已经成功应用。"
LangString L10N_HOOKTIMEOUT_FOOTER         0 "改动将会在你下次登录时起作用。"
