; AltDrag - ja-JP localization by kakakaya
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.

!insertmacro MUI_LANGUAGE "Japanese" ; English name of this language
LangString L10N_LANG ${LANG_JAPANESE} ""

LangString L10N_UPGRADE_TITLE     0 "インストール済み"
LangString L10N_UPGRADE_SUBTITLE  0 "${APP_NAME}のインストール方法を選択してください。"
LangString L10N_UPGRADE_HEADER    0 "${APP_NAME}は既にこのコンピュータにインストールされています。行いたい操作を選択してください。"
LangString L10N_UPGRADE_UPGRADE   0 "(&U)${APP_NAME}を更新する(${APP_VERSION})。"
LangString L10N_UPGRADE_INI       0 "今までの設定は${APP_NAME}-old.iniにコピーされます。"
LangString L10N_UPGRADE_INSTALL   0 "(&I)新しい場所にインストールする。"
LangString L10N_UPGRADE_UNINSTALL 0 "(&n)${APP_NAME}をアンインストールする。"

LangString L10N_ALTSHIFT_TITLE    0 "キーボードショートカット"
LangString L10N_ALTSHIFT_SUBTITLE 0 "Alt + Shiftはこのプログラムと競合しています: ${APP_NAME}。"
LangString L10N_ALTSHIFT_HEADER   0 "Windowsのキーボードレイアウトを変更するショートカットキーがAlt+Shiftに設定されていることが検出されました。$\n$\n${APP_NAME}では、Shiftを押しながらウィンドウをドラッグすることで他のウィンドウにスナップさせられます。つまり、スナップさせようとしてキーボードレイアウトが変わってしまうかもしれません。${APP_NAME}はキーボードレイアウトの変更をさせないように試しますが、もしウィンドウをドラッグし始める前にShiftキーを押していたら上手く動作しません。$\n$\nこのボタンを押すことで、キーボードレイアウト変更のショートカットキーを無効化したり、別のショートカットキーに割り当てを変更することができます。"
LangString L10N_ALTSHIFT_BUTTON   0 "(&O)キーボード設定を開く"

LangString L10N_HOOKTIMEOUT_TITLE          0 "レジストリ変更"
LangString L10N_HOOKTIMEOUT_SUBTITLE       0 "${APP_NAME}が突然停止することを防ぎます。"
LangString L10N_HOOKTIMEOUT_HEADER         0 "MicrosoftはWindows 7に、時間が掛かるキーボードやマウス操作へのフックを停止する機能を付加しました。これは、コンピュータを頻繁に休止や、スリープ、ロックする時にしばしば誤って動作します。$\n$\nそのような場合、${APP_NAME}は動かなくなってしまい、直すには動作を一旦停止させてから再度有効化させる必要があります。$\n$\n下のボタンからレジストリ変更を有効化・無効化することで、Windowsがフックの停止をするまでの時間を伸ばすことができます。この変更は現在のユーザにのみ反映されます。レジストリ変更をしなくてもAltDragは利用可能です。"
LangString L10N_HOOKTIMEOUT_APPLYBUTTON    0 "(&E)レジストリ変更を有効化する"
LangString L10N_HOOKTIMEOUT_REVERTBUTTON   0 "(&D)レジストリ変更を無効化する"
LangString L10N_HOOKTIMEOUT_ALREADYAPPLIED 0 "レジストリ変更は既に行われています。"
LangString L10N_HOOKTIMEOUT_FOOTER         0 "この変更は次にログインした時から有効になります。"
