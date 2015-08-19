/*
  AltDrag - en-US localization by Stefan Sundin

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
*/

struct strings en_US = {
 /* === translation info === */
 /* code               */ L"ko-KR",
 /* lang_english       */ L"Korean",
 /* lang               */ L"Korean",
 /* author             */ L"Stefan Sundin",

 /* === app === */
 /* tray_enabled       */ APP_NAME L"",
 /* tray_disabled      */ APP_NAME L" (비활성화)",

 /* menu */
 /* enable             */ L"활성화",
 /* disable            */ L"비활성화",
 /* hide               */ L"트레이로 숨기기",
 /* update             */ L"새 버젼이 있습니다!",
 /* config             */ L"설정",
 /* about              */ L"정보",
 /* exit               */ L"종료",

 /* update */
 /* balloon            */ L"새 버젼이 있습니다!",
 /* dialog             */ L"새 버젼을 사용하실 수 있습니다. 웹사이트로 가시겠습니까?",
 /* nonew              */ L"최신버젼입니다.",

 /* === config === */
 /* title              */ APP_NAME L" 설정",
 /* tabs */
 /* general            */ L"일반",
 /* input              */ L"마우스, 키보드",
 /* blacklist          */ L"블랙리스트",
 /* advanced           */ L"고급",
 /* about              */ L"정보",

 /* general tab */
 /* box                */ L"일반 설정",
 /* autofocus          */ L"(&F) 드래그하는동안 창을 강조합니다.\nCtrl 키를 눌러서도 창을 강조할 수 있습니다.",
 /* aero               */ L"(&A) 에어로 기능을 모방합니다.",
 /* inactivescroll     */ L"(&S) 비활성화된 창을 스크롤합니다.",
 /* mdi                */ L"(&M)DI 지원",
 /* autosnap           */ L"자동으로 대상에 맞춥니다:",
 /* autosnap0          */ L"비활성화",
 /* autosnap1          */ L"스크린 화면",
 /* autosnap2          */ L"+ 창 밖으로",
 /* autosnap3          */ L"+ 창 안으로",
 /* language           */ L"언어:",
 /* autostart_box      */ L"자동 실행",
 /* autostart          */ L"(&t) 로그온 할때 자동으로 "APP_NAME" 실행",
 /* autostart_hide     */ L"(&H) 트레이로 숨기기",
 /* autostart_elevate  */ L"(&E) 관리자 권한으로 높히기",
 /*       elevate_tip  */ L"UAC를 완전히 비활성화 하지 않는 한, 로그인 할 때 마다 UAC 프롬프트가 나타납니다.",
 /* elevate            */ L"(&l) 높히기",
 /* elevated           */ L"관리자 권한입니다.",
 /* elevate_tip        */ L"관리자 권한으로 새로운 "APP_NAME" 인스턴스를 실행합니다. "APP_NAME"가 관리자 권한으로 실행중인 다른 프로그램을 관리할 수 있도록 허락합니다.\n\n"APP_NAME"를 관리자 권한으로 실행하기 위해서는 UAC 프롬프트를 승낙해야합니다.",
 /* elevation_aborted  */ L"높히기 취소",
 /* autosave           */ L"설정을 저장하고 즉시 반영됩니다.",

 /* input tab */
 /* mouse */
 /* box                */ L"마우스 동작",
 /* lmb                */ L"왼쪽 버튼:",
 /* mmb                */ L"가운데 버튼:",
 /* rmb                */ L"오른쪽 버튼:",
 /* mb4                */ L"추가버튼 4:",
 /* mb5                */ L"추라버튼 5:",
 /* scroll             */ L"스크롤 휠:",
 /* lowerwithmmb       */ L"(&L) 제목 표시줄을 가운데 클릭 함으로써, 창을 줄입니다.",

 /* actions */
 /* move               */ L"창 이동",
 /* resize             */ L"창 사이즈 변경",
 /* close              */ L"창 닫기",
 /* minimize           */ L"창 최소화",
 /* lower              */ L"창 줄이기",
 /* alwaysontop        */ L"항상 위에",
 /* center             */ L"화면의 중앙",
 /* nothing            */ L"아무것도",
 /* alttab             */ L"Alt+Tab",
 /* volume             */ L"소리",
 /* transparency       */ L"투명도",

 /* hotkeys */
 /* box                */ L"단축키",
 /* leftalt            */ L"(&e) 왼쪽 Alt 키",
 /* rightalt           */ L"(&R) 오른쪽 Alt 키",
 /* leftwinkey         */ L"(&W) 왼쪽 Windows 키",
 /* rightwinkey        */ L"(&i) 오른쪽 Windows 키",
 /* leftctrl           */ L"(&C) 왼쪽 Ctrl 키",
 /* rightctrl          */ L"(&t) 오른쪽 Ctrl 키",
 /* more               */ L"ini 파일을 수정해서 단축키를 변경하거나 추가할 수 있습니다.\n창들을 붙이기(스냅) 위해서 Shift 키를 사용하세요.",

 /* blacklist tab */
 /* box                */ L"블랙리스트 설정",
 /* processblacklist   */ L"프로세스 블랙리스트:",
 /* blacklist          */ L"블랙리스트:",
 /* snaplist           */ L"스냅리스트:",
 /* explanation        */ L"<a href=\"http://code.google.com/p/altdrag/wiki/Blacklist\">위키</a>에서 블랙리스트의 설명을 보실 수 있습니다.",
 /* findwindow_box     */ L"창 찾기",
 /* _explanation       */ L"아이콘을 클릭해서 창을 블랙리스트 혹은 스냅리스트에 등록할 수 있습니다.",

 /* advanced tab */
 /* box                */ L"고급 설정",
 /* hookwindows        */ L"(&E) 일반적으로 창을 움직일때 스냅기능을 활성화합니다.\n자동으로 스냅기능이 작동합니다.",
 /* hookwindows_warn   */ L"다른 프로세스를 후킹하여 사용하는 것이므로, 완전히 안전하지는 않다는걸 염두에 두십시오. 게임을 할때 게임가드와 충돌이 날 수 있습니다.\n\n정말 활성화 하시겠습니까?",
 /* checkonstartup     */ L"(&u) 자동 업데이트 체크",
 /* beta               */ L"(&b) 베타 버젼 체크",
 /* checknow           */ L"(&C) 지금 업데이트 체크",
 /* ini                */ APP_NAME L"의 설정은 "APP_NAME".ini 에 저장됩니다. 이 파일을 직접 수정해야만 사용가능한 몇가지의 기능들이 존재합니다.",
 /* openini            */ L"Open &ini file",

 /* about tab */
 /* box                */ L""APP_NAME" 에 대하여",
 /* version            */ L"버젼 "APP_VERSION,
 /* author             */ L"Created by Stefan Sundin",
 /* license            */ APP_NAME L" is free and open source software!\nFeel free to redistribute!",
 /* donate             */ L"&Donate",
 /* translation_credit */ L"Jeyraof translates it to korean",

 /* === misc === */
 /* unhook_error       */ L""APP_NAME"을 비활성화시킨 에러가 있습니다. 이 에러는 "APP_NAME"의 훅들이 이미 비활성화 된경우 자주 발생합니다.\n\n만약 이 에러가 처음이라면, 무시하고 "APP_NAME"를 이용하시면 됩니다.\n\n만약 반복적으로 에러가 발생한다면, 위키에서 해결 방법을 읽으실 수 있습니다. (해당 문서: 'AltDrag mysteriously stops working' on the About page).",
};
