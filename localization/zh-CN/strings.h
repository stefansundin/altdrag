/*
	AltDrag - zh-CN localization by Jack Jin (superjwl@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

struct strings zh_CN = {
 /* tray_enabled       */ APP_NAME,
 /* tray_disabled      */ APP_NAME " (禁止)",
 { /* menu */
   /* enable           */ L"允许",
   /* disable          */ L"禁止",
   /* hide             */ L"隐藏通知栏图标",
   /* update           */ L"发现有可用的更新!",
   /* config           */ L"打开设置",
   /* about            */ L"关于",
   /* exit             */ L"退出",
 },
 { /* update */
   /* balloon          */ L"发现新版本!",
   /* dialog           */ L"有可用的新版本. 打开AltDrag网站吗？?",
   /* nonew            */ L"没有可用的更新.",
 },
 
 /* === config === */
 /* lang               */ L"简体",
 /* title              */ APP_NAME L" 配置",
 { /* tabs */
   /* general          */ L"通用",
   /* input            */ L"鼠标和键盘",
   /* blacklist        */ L"不作用列表",
   /* advanced         */ L"增强",
   /* about            */ L"关于",
 },
 { /* general tab */
   /* box              */ L"通用设置",
   /* autofocus        */ L"(&F)把动时聚焦窗口.\n按Ctrl也可以聚集到窗口.",
   /* aero             */ L"(&A)Win7窗口贴边自动调整",
   /* inactivescroll   */ L"&滚动非活动窗口",
   /* autosnap         */ L"自动吸附在:",
   /* autosnap0        */ L"禁止",
   /* autosnap1        */ L"到屏幕边缘",
   /* autosnap2        */ L"+ 在窗口外",
   /* autosnap3        */ L"+ 在窗口内",
   /* language         */ L"语言:",
   /* helptranslate    */ L"怎样参与翻译?",
   /* autostart_box    */ L"随系统自动启动",
   /* autostart        */ L"(&t)登录时启动"APP_NAME"",
   /* autostart_hide   */ L"(&H)隐藏通知栏图标",
   /* autosave         */ L"注意: 设置改动后会立即保存!",
 },
 { /* input tab */
   { /* mouse */
     /* box            */ L"鼠标动作",
     /* lmb            */ L"鼠标左键:",
     /* mmb            */ L"鼠标中键:",
     /* rmb            */ L"鼠标右键:",
     /* mb4            */ L"鼠标第4键:",
     /* mb5            */ L"鼠标第5键:",
     /* scroll        `*/ L"Scroll:",
     /* more          `*/ L"如果你有好的鼠标动作想法, 可以在<a href=\"http://code.google.com/p/altdrag/issues/list\">AltDrag网站</a>上提交一个请求!",
   },
   { /* actions `*/
     /* move           */ L"Move",
     /* resize         */ L"Resize",
     /* close          */ L"Close",
     /* minimize       */ L"Minimize",
     /* lower          */ L"Lower",
     /* alwaysontop    */ L"AlwaysOnTop",
     /* center         */ L"Center",
     /* nothing        */ L"Nothing",
     /* alttab         */ L"Alt+Tab",
     /* volume         */ L"Volume",
   },
   { /* hotkeys */
     /* box            */ L"热键",
     /* leftalt        */ L"左&Alt",
     /* rightalt       */ L"右A&lt",
     /* leftwinkey     */ L"左&Winkey",
     /* rightwinkey    */ L"右W&inkey",
     /* leftctrl       */ L"左&Ctrl",
     /* rightctrl      */ L"右C&trl",
     /* more           */ L"你可以通过修改ini配置文件来加入任何键!\n使用shift键使窗口间进行吸附.",
   },
 },
 { /* blacklist tab */
   /* box              */ L"不作用列表设置",
   /* processblacklist */ L"按进程名称的不作用列表:",
   /* blacklist        */ L"不作用列表:",
   /* snaplist         */ L"自动吸附列表:",
   /* explanation      */ L"不作用列表具体如何工作，请参阅<a href=\"http://code.google.com/p/altdrag/wiki/Blacklist\">AltDrag网站维基</a>!",
   /* findwindow_box   */ L"辩识窗口",
   /* _explanation     */ L"全用此工具来辩识窗口的类名，以便于你在不作用列表或自动吸附列表中进行添加.",
 },
 { /* advanced tab */
   /* box              */ L"增强设置",
   /* hookwindows      */ L"(&E)在普通移动中吸附窗口.\n窗口之间自动吸附!",
   /* hookwindows_warn`*/ L"注意这个特性不是100%安全的，因为他会对其他进程进行挂接或类似操作.",
   /* checkonstartup   */ L"(&u)处动检查更新",
   /* beta             */ L"为&beta版检查更新",
   /* checknow         */ L"检查更新",
   /* ini              */ APP_NAME L"的设置保存在"APP_NAME".ini. 有少量配置可以通过手工编辑此文件来更改.",
   /* openini          */ L"打开&ini文件",
 },
 { /* about tab */
   /* box              */ L"关于"APP_NAME,
   /* version          */ L"版本"APP_VERSION,
   /* author           */ L"作者: Stefan Sundin",
   /* license          */ APP_NAME L" 是自由和开源软件!\n欢迎重新发布!",
   /* donations_box    */ L"捐赠",
   /* donations_plea   */ L"如果你觉得"APP_NAME"有用, 你可以考虑进行一定的捐蹭. 欢迎任何数量的捐赠.",
   /* donate           */ L"(&D)捐赠",
 },
};
