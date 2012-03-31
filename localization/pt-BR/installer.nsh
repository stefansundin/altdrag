;AltDrag - pt-BR localization by Jucá Costa (virgilino@gmail.com)
;
;This program is free software: you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.

!insertmacro MUI_LANGUAGE "PortugueseBR"
!define LANG ${LANG_PORTUGUESEBR}

LangString L10N_UPGRADE_TITLE     ${LANG} "Já instalado"
LangString L10N_UPGRADE_SUBTITLE  ${LANG} "Escolha o modo de instalação de ${APP_NAME}."
LangString L10N_UPGRADE_HEADER    ${LANG} "${APP_NAME} já está instalado neste sistema. Selecione a operação a ser executada e clique em Próximo para continuar."
LangString L10N_UPGRADE_UPGRADE   ${LANG} "&Atualizar ${APP_NAME} para a versão ${APP_VERSION}."
LangString L10N_UPGRADE_INI       ${LANG} "Suas configurações já existentes serão copiadas para ${APP_NAME}-old.ini."
LangString L10N_UPGRADE_INSTALL   ${LANG} "&Instalar em outra pasta."
LangString L10N_UPGRADE_UNINSTALL ${LANG} "&Desinstalar ${APP_NAME}."
LangString L10N_UPDATE_SECTION    ${LANG} "Verificar se há atualizações antes de instalar"
LangString L10N_UPDATE_DIALOG     ${LANG} "Uma nova versão está disponível.$\nCancelar instalação e ir à página de download?"
LangString L10N_SHORTCUT          ${LANG} "Atalho no menu Iniciar"
LangString L10N_AUTOSTART         ${LANG} "Iniciar com o Windows"
LangString L10N_AUTOSTART_HIDE    ${LANG} "Ocultar ícone da área de notificação"

LangString L10N_ALTSHIFT_TITLE    ${LANG} "Atalho de teclado"
LangString L10N_ALTSHIFT_SUBTITLE ${LANG} "O atalho Alt + Shift entra em conflito com ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   ${LANG} "O instalador detectou que o atalho do Windows para alternar o layout do teclado é Alt + Shift.$\n$\nAo utilizar ${APP_NAME}, você pode pressionar Shift enquanto arrasta uma janela para alinhá-la com outras janelas. Isso significa que é provável que você pressione Alt + Shift, a mesma combinação que alterna o layout do teclado. Ainda que o ${APP_NAME} tente bloquear estas mudanças acidentais, ele poderá não conseguir se você pressionar Shift antes de começar a arrastar uma janela.$\n$\nVocê pode desativar ou mudar o atalho que alterna o layout do teclado pressionado o botão abaxio. Clique em Próximo para continuar."
LangString L10N_ALTSHIFT_BUTTON   ${LANG} "&Alterar teclados..."

!undef LANG
