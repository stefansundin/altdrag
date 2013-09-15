 /*
	AltDrag - zh-CN localization by Jack Jin (superjwl@gmail.com)

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

struct strings zh_CN = {
 /* === translation info === */
 /* code               */ L"zh-CN",
 /* lang_english       */ L"Chinese",
 /* lang               */ L"简体",
 /* author             */ L"Jack Jin",

 /* === app === */
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
 /* title              */ APP_NAME L" 配置",
 { /* tabs */
   /* general          */ L"通用",
   /* input            */ L"鼠标和键盘",
   /* blacklist        */ L"排除列表",
   /* advanced         */ L"增强",
   /* about            */ L"关于",
 },
 { /* general tab */
   /* box              */ L"通用设置",
   /* autofocus        */ L"(&F)拖动时聚焦窗口.\nCtrl也可以聚焦到窗口.",
   /* aero             */ L"(&N)Win7窗口贴边自动调整",
   /* inactivescroll   */ L"&滚动非活动窗口",
   /* mdi              */ L"&MDI support",
   /* autosnap         */ L"自动吸附在:",
   /* autosnap0        */ L"禁止",
   /* autosnap1        */ L"到屏幕边缘",
   /* autosnap2        */ L"+ 在窗口外",
   /* autosnap3        */ L"+ 在窗口内",
   /* language         */ L"语言:",
   /* autostart_box    */ L"随系统自动启动",
   /* autostart        */ L"(&t)登录时启动"APP_NAME"",
   /* autostart_hide   */ L"(&H)隐藏通知栏图标",
   /* autostart_elevate*/ L"(&E)提升至超级用户权限",
   /*       elevate_tip*/ L"注意,除非你完全禁用UAC,否则每次登录都会出现UAC提示.",
   /* elevate          */ L"(&l)提升",
   /* elevated         */ L"已经提升",
   /* elevate_tip      */ L"这会创建一个新的以超级用户权限运行的"APP_NAME"实例. 以允许"APP_NAME"管理其他以超级用户权限运行的程序.\n\n.在弹出UAC提示时，你必须同意"APP_NAME"以超级用户权限运行",
   /* elevation_aborted*/ L"提升过程终止.",
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
     /* scroll         */ L"滚动:",
     /* lowerwithmmb   */ L"&Lower windows by middle clicking on title bars",
   },
   { /* actions */
     /* move           */ L"移动",
     /* resize         */ L"更改大小",
     /* close          */ L"关闭",
     /* minimize       */ L"最小化",
     /* lower          */ L"降低",
     /* alwaysontop    */ L"一直在最上层",
     /* center         */ L"居中",
     /* nothing        */ L"无动作",
     /* alttab         */ L"Alt+Tab",
     /* volume         */ L"音量",
     /* transparency   */ L"透明度",
   },
   { /* hotkeys */
     /* box            */ L"热键",
     /* leftalt        */ L"左A&lt",
     /* rightalt       */ L"右Al&t",
     /* leftwinkey     */ L"左&Winkey",
     /* rightwinkey    */ L"右W&inkey",
     /* leftctrl       */ L"左&Ctrl",
     /* rightctrl      */ L"右Ct&rl",
     /* more           */ L"你可以通过修改ini配置文件来加入任何键!\n用shift键使窗口间进行吸附.",
   },
 },
 { /* blacklist tab */
   /* box              */ L"排除列表设置",
   /* processblacklist */ L"按进程名称的排除列表:",
   /* blacklist        */ L"排除列表:",
   /* snaplist         */ L"自动吸附列表:",
   /* explanation      */ L"排除列表具体如何工作，请参阅<a href=\"http://code.google.com/p/altdrag/wiki/Blacklist\">AltDrag网站维基</a>!",
   /* findwindow_box   */ L"辩识窗口",
   /* _explanation     */ L"全用此工具来辩识窗口的类名，以便于你在排除列表或自动吸附列表中进行添加.",
 },
 { /* advanced tab */
   /* box              */ L"增强设置",
   /* hookwindows      */ L"(&E)在普通移动中吸附窗口.\n口之间自动吸附!",
   /* hookwindows_warn */ L"注意这个特性不是100%安全的，因为他会对其他进程进行挂接或类似操作.",
   /* checkonstartup   */ L"(&u)自动检查更新",
   /* beta             */ L"为&beta版检查更新",
   /* checknow         */ L"检查更新",
   /* ini              */ APP_NAME L"的设置保存在"APP_NAME".ini. 有少量配置可以通过手工编辑此文件来更改.",
   /* openini          */ L"打开&ini文件",
 },
 { /* about tab */
   /* box              */ L"关于"APP_NAME,
   /* version          */ L"版本"APP_VERSION,
   /* author           */ L"作者: Stefan Sundin",
   /* license          */ APP_NAME L" 是自由和开源软件!\n迎重新发布!",
   /* donate           */ L"(&D)捐赠",
   /*translation_credit*/ L"翻译贡献者",
 },

 /* === misc === */
 /* unhook_error       */ L"在禁用"APP_NAME"时发生错误. 这可能是因为windows已经禁用了"APP_NAME"的勾子而引起的.\n\n如果是第一次发生这种情况, 你可以安全的忽略并继续使用"APP_NAME".\n\n如果这个错误重复出现, 你可以阅读wiki来防范这种情况的再次发生 (在About页上寻找'"APP_NAME"意外停止工作').",
};
