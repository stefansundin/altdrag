; AltDrag - zh-TW localization by Zkm (zkm@ilowkey.net)
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.

!insertmacro MUI_LANGUAGE "Traditional Chinese" ; English name of this language
LangString L10N_LANG ${LANG_TRADCHINESE} ""

LangString L10N_UPGRADE_TITLE     0 "早已安裝"
LangString L10N_UPGRADE_SUBTITLE  0 "請選擇您要安裝 ${APP_NAME} 的方式。"
LangString L10N_UPGRADE_HEADER    0 "${APP_NAME} 早已安裝至您的系統中。選取您想執行的內容並按下「下一步」以繼續動作。"
LangString L10N_UPGRADE_UPGRADE   0 "升級 ${APP_NAME} 至 ${APP_VERSION} 版(&U)。"
LangString L10N_UPGRADE_INI       0 "您原先使用的設定檔案將被複製為 ${APP_NAME}-old.ini。"
LangString L10N_UPGRADE_INSTALL   0 "安裝至新位置(&I)"
LangString L10N_UPGRADE_UNINSTALL 0 "移除 ${APP_NAME}(&T)。"

LangString L10N_ALTSHIFT_TITLE    0 "鍵盤快捷鍵"
LangString L10N_ALTSHIFT_SUBTITLE 0 "Alt 鍵 + Shift 鍵將會跟 ${APP_NAME} 衝突。"
LangString L10N_ALTSHIFT_HEADER   0 "安裝程式已偵測到 Windows 使用鍵盤的 Alt 鍵 + Shift 鍵作為切換快捷鍵。$\n$\n當您使用 ${APP_NAME} 時，您可以按下 Shift 來讓兩個視窗彼此貼齊。這表示切換快捷鍵與貼齊快捷鍵相同。當 ${APP_NAME} 內部嘗試阻止鍵盤快捷鍵的切換時，您按下 Shift 鍵拖曳視窗貼齊的功能將無法使用。$\n$\n因此您可以停用或改變鍵盤切換快捷鍵。按下「下一步」繼續動作。"
LangString L10N_ALTSHIFT_BUTTON   0 "開啟鍵盤快捷鍵設定(&O)"

LangString L10N_HOOKTIMEOUT_TITLE          0 "登錄檔最佳化"
LangString L10N_HOOKTIMEOUT_SUBTITLE       0 "此最佳化是可選的選項，讓 ${APP_NAME} 能免於意外地終止。"
LangString L10N_HOOKTIMEOUT_HEADER         0 "在 Windows 7 中微軟增加了一個功能，讓長時間沒有回應的鍵盤與滑鼠 Hook 變為停用狀態。不幸的是，這個檢查很容易出錯，特別是當您讓電腦長時間休眠、睡眠或鎖定的時候。$\n$\n若是發生此情形，您會發現 ${APP_NAME} 無預警的停止工作，接著您得手動停用再啟用 ${APP_NAME} 以讓它能再次正常執行。$\n$\n最佳化此登錄檔可以讓 Windows 在停止 hooks 前的等待時間變長，您可以使用底下的按鈕啟用或停用此選項。請注意：這個登錄檔最佳化完全是可選的選項。"
LangString L10N_HOOKTIMEOUT_APPLYBUTTON    0 "啟用登錄檔最佳化(&E)"
LangString L10N_HOOKTIMEOUT_REVERTBUTTON   0 "停用登錄檔最佳化(&D)"
LangString L10N_HOOKTIMEOUT_ALREADYAPPLIED 0 "此登錄檔最佳化早已被套用。"
LangString L10N_HOOKTIMEOUT_FOOTER         0 "此變更將於下次登入時套用。"
