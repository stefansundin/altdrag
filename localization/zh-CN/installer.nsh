;AltDrag - zh-CN localization by Jack Jin (superjwl@gmail.com)
;
;This program is free software: you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.

!insertmacro MUI_LANGUAGE "SimpChinese"
LangString L10N_LANG ${LANG_SIMPCHINESE} ""

LangString L10N_UPGRADE_TITLE     0 "已经安装"
LangString L10N_UPGRADE_SUBTITLE  0 "选择如何安装${APP_NAME}."
LangString L10N_UPGRADE_HEADER    0 "${APP_NAME}已经安装在你的系统中. 选择你想进行的操作，并点击下一步继续."
LangString L10N_UPGRADE_UPGRADE   0 "(&U)把${APP_NAME}升级到${APP_VERSION}."
LangString L10N_UPGRADE_INI       0 "你的旧配置将被保存为${APP_NAME}-old.ini."
LangString L10N_UPGRADE_INSTALL   0 "(&I)安装到新的位置."
LangString L10N_UPGRADE_UNINSTALL 0 "(&U)卸载${APP_NAME}."
LangString L10N_UPDATE_SECTION    0 "安装前检查是否有更新"
LangString L10N_UPDATE_DIALOG     0 "有可用的新版本.$\n中断安装并浏览AltDrag网站吗?"
LangString L10N_SHORTCUT          0 "开始菜单项"
LangString L10N_AUTOSTART         0 "随系统自动启动"
LangString L10N_AUTOSTART_HIDE    0 "隐藏通知栏图标"

LangString L10N_ALTSHIFT_TITLE    0 "快捷键"
LangString L10N_ALTSHIFT_SUBTITLE 0 "快捷键Alt + Shift和${APP_NAME}冲突."
LangString L10N_ALTSHIFT_HEADER   0 "安装程序检测到当前windows切换键盘布局的快捷键为Alt + Shift.$\n$\n而在使用${APP_NAME}时, 按住shift键可以在窗口之间相互吸附. 所以切换键盘布局和窗口相互吸附这两个功能的快捷键相同. 当${APP_NAME}内部试图阻止意外的键盘布局切换时, 会造成按住shift键移动窗口操作的失败.$\n$\n此时，你可以禁止或修改切换键盘布局的快捷键. 点击下一步继续."
LangString L10N_ALTSHIFT_BUTTON   0 "(&O)打开键盘设置"
