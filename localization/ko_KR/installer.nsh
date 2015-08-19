; AltDrag - ko-KR localization by @jeyraof
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.

!insertmacro MUI_LANGUAGE "Korean" ; English name of this language
LangString L10N_LANG ${LANG_KOREAN} ""

LangString L10N_UPGRADE_TITLE     0 "이미 설치 되었습니다."
LangString L10N_UPGRADE_SUBTITLE  0 "${APP_NAME}을 설치할 방법을 선택하세요."
LangString L10N_UPGRADE_HEADER    0 "이 컴퓨터에 ${APP_NAME}이 이미 설치되어 있습니다. 원하는 동작을 선택하고, 다음을 눌러 진행하세요."
LangString L10N_UPGRADE_UPGRADE   0 "(&U) ${APP_NAME}을 ${APP_VERSION}로 업그레이드 합니다."
LangString L10N_UPGRADE_INI       0 "기존 세팅을 ${APP_NAME}-old.ini 파일로 백업합니다."
LangString L10N_UPGRADE_INSTALL   0 "(&I) 새로운 위치에 설치합니다."
LangString L10N_UPGRADE_UNINSTALL 0 "${APP_NAME}을 제거합니다."

LangString L10N_ALTSHIFT_TITLE    0 "키보드 단축키"
LangString L10N_ALTSHIFT_SUBTITLE 0 "단축키 Alt + Shift 가 ${APP_NAME}과 충돌합니다."
LangString L10N_ALTSHIFT_HEADER   0 "현재 키보드 레이아웃을 변경하기 위한 윈도우 단축키가 Alt + Shift 인것을 감지했습니다.$\n$\n${APP_NAME}을 사용하는 동안, 창을 드래그 할때 Shift 를 눌러 다른 창에 달라붙게 만들 수 있습니다. 키보드 레이아웃을 변경하기 위한 단축키와 같은 동작을 하는것을 의미합니다. ${APP_NAME}이 내부적으로 갑작스런 키보드 레이아웃 변경을 차단하려는동안, 창을 드래그 시작하기 전에 Shift 를 누른다면 동작하지 않습니다. 이 버튼을 눌러 키보드 레이아웃 변경을 위한 단축키를 비활성화 혹은 변경할 수 있습니다. 다음을 눌러 진행하세요."
LangString L10N_ALTSHIFT_BUTTON   0 "(&O) 키보드 세팅 열기"

LangString L10N_HOOKTIMEOUT_TITLE          0 "레지스트리 수정"
LangString L10N_HOOKTIMEOUT_SUBTITLE       0 "${APP_NAME}이 갑작스럽게 멈추는것을 방지합니다."
LangString L10N_HOOKTIMEOUT_HEADER         0 "윈도우 7 에서는 반응하기에 너무 길면 키보드와 마우스의 훅을 멈추는 기능이 추가되었습니다. 불행히도 이 기능은 잘못되게 동작할 수 있습니다. 특히 절전모드 혹은 잠금화면으로 들어갈때 자주 발생합니다.$\n$\n만약 이런일이 발생한다면, 경고 없이 ${APP_NAME}이 작동을 멈추는것을 찾고 직접 ${APP_NAME}을 종료 후 재실행 하셔야 합니다.$\n$\n이 레지스트리 수정은 윈도우가 훅이 멈추기 전에 더 길게 기다리도록 만듭니다. 아래의 버튼으로 활성화 혹은 비활성화 하실 수 있습니다. 이 옵션은 선택이라는것을 인지해 주십시오."
LangString L10N_HOOKTIMEOUT_APPLYBUTTON    0 "(&E) 레지스트리 수정 활성화"
LangString L10N_HOOKTIMEOUT_REVERTBUTTON   0 "(&D) 레지스트리 수정 비활성화" 
LangString L10N_HOOKTIMEOUT_ALREADYAPPLIED 0 "레지스트리 수정이 이미 적용되었습니다."
LangString L10N_HOOKTIMEOUT_FOOTER         0 "이 변경은 컴퓨터를 재시작하면 반영됩니다."
