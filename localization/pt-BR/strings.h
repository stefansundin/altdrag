/*
	AltDrag - pt-BR localization by Jucá Costa (virgilino@gmail.com)

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

struct strings pt_BR = {
 /* === translation info === */
 /* code               */ L"pt-BR",
 /* lang_english       */ L"Brazilian Portuguese",
 /* lang               */ L"Português (Brasil)",
 /* author             */ L"Jucá Costa",

 /* === app === */
 /* tray_enabled       */ APP_NAME,
 /* tray_disabled      */ APP_NAME " (desativado)",
 { /* menu */
   /* enable           */ L"Ativar",
   /* disable          */ L"Desativar",
   /* hide             */ L"Ocultar ícone",
   /* update           */ L"Atualização disponível!",
   /* config           */ L"Configuração",
   /* about            */ L"Sobre",
   /* exit             */ L"Sair",
 },
 { /* update */
   /* update_balloon   */ L"Nova versão disponível!",
   /* update_dialog    */ L"Uma nova versão está disponível. Deseja ir à página de download?",
   /* update_nonew     */ L"Não há atualizações disponíveis.",
 },

 /* === config === */
 /* title              */ L"Configuração do " APP_NAME,
 { /* tabs */
   /* general          */ L"Geral",
   /* input            */ L"Teclado e mouse",
   /* blacklist        */ L"Lista negra",
   /* advanced         */ L"Avançado",
   /* about            */ L"Sobre",
 },
 { /* general tab */
   /* box              */ L"Configurações gerais",
   /* autofocus        */ L"A&tivar janelas ao arrastar. Você também pode\npressionar Ctrl para ativar janelas.",
   /* aero             */ L"&Imitar o Aero Snap",
   /* inactivescroll   */ L"&Rolar janelas inativas",
   /* mdi              */ L"Suporte a &MDI, interface de múltiplos documentos",
   /* autosnap         */ L"Alinhar automaticamente a:",
   /* autosnap0        */ L"Desativado",
   /* autosnap1        */ L"Bordas da tela",
   /* autosnap2        */ L"+ exterior das janelas",
   /* autosnap3        */ L"+ interior das janelas",
   /* language         */ L"Idioma:",
   /* autostart_box    */ L"Início automático",
   /* autostart        */ L"&Iniciar o "APP_NAME" quando fizer logon",
   /* autostart_hide   */ L"&Ocultar ícone da área de notificação",
   /* autostart_elevate*/ L"&Elevar a privilégios de administrador",
   /*       elevate_tip*/ L"Note que a janela do Controle de Conta de Usuário aparecerá sempre que você fizer logon, a não ser que o Controle de Conta de Usuário esteja desativado completamente.",
   /* elevate          */ L"E&levar",
   /* elevated         */ L"Elevado",
   /* elevate_tip      */ L"Isto criará uma nova instância do "APP_NAME" que possuirá privilégios de administrador, permitindo que o "APP_NAME" manipule outros programas que possuam privilégios de administrador.\n\nVocê deverá permitir o "APP_NAME" na janela de confirmação do Controle de Conta de Usuário do Windows, para que o "APP_NAME" seja executado com privilégios de administrador.",
   /* elevation_aborted*/ L"Elevação cancelada.",
   /* autosave         */ L"Nota: as configurações são salvas e aplicadas instantaneamente.",
 },
 { /* input tab */
   { /* mouse */
     /* box            */ L"Ações do mouse",
     /* lmb            */ L"Botão esquerdo:",
     /* mmb            */ L"Botão do meio:",
     /* rmb            */ L"Botão direito:",
     /* mb4            */ L"Botão 4:",
     /* mb5            */ L"Botão 5:",
     /* scroll         */ L"Roda do mouse:",
     /* lowerwithmmb   */ L"&Enviar janela para trás ao clicar na barra de título",
   },
   { /* actions */
     /* move           */ L"Mover",
     /* resize         */ L"Redimensionar",
     /* close          */ L"Fechar",
     /* minimize       */ L"Minimizar",
     /* lower          */ L"Enviar para trás",
     /* alwaysontop    */ L"Sempre visível",
     /* center         */ L"Centralizar",
     /* nothing        */ L"Nada",
     /* alttab         */ L"Alt+Tab",
     /* volume         */ L"Volume",
     /* transparency   */ L"Transparência",
   },
   { /* hotkeys */
     /* box            */ L"Teclas de atalho",
     /* leftalt        */ L"Alt &esquerdo",
     /* rightalt       */ L"Alt &direito",
     /* leftwinkey     */ L"&Windows esquerdo",
     /* rightwinkey    */ L"W&indows direito",
     /* leftctrl       */ L"&Ctrl esquerdo",
     /* rightctrl      */ L"C&trl direito",
     /* more           */ L"Adicione mais teclas editando o arquivo ini.\nUtilize a tecla Shift para alinhar as janelas umas às outras.",
   },
 },
 { /* blacklist tab */
   /* box              */ L"Configurações da lista negra",
   /* processblacklist */ L"Processos a ignorar:",
   /* blacklist        */ L"Janelas a ignorar:",
   /* snaplist         */ L"Janelas a alinhar:",
   /* explanation      */ L"Leia <a href=\"http://code.google.com/p/altdrag/wiki/Blacklist\">a wiki</a> (em inglês) na nossa página para uma explicação detalhada de como a lista negra funciona.",
   /* findwindow_box   */ L"Identificar janela",
   /* _explanation     */ L"Utilize esta ferramenta para identificar a classe de uma janela, a fim de adicionar a uma das listas acima.",
 },
 { /* advanced tab */
   /* box              */ L"Configurações avançadas",
   /* hookwindows      */ L"A&tivar alinhamento quando mover normalmente uma janela. Funciona em conjunto com o alinhamento automático.",
   /* hookwindows_warn */ L"Note que isto não é totalmente seguro, pois faz injeções (hooks) em outros processos. Pode ser arriscado utilizar com jogos que tenham proteção anti-cheat.",
   /* checkonstartup   */ L"Verificar se há novas at&ualizações automaticamente",
   /* beta             */ L"Verificar versões &beta",
   /* checknow         */ L"&Verificar agora",
   /* ini              */ L"As configurações do "APP_NAME" são salvas no arquivo "APP_NAME".ini. Certas configurações só podem ser alteradas através da modificação manual desse arquivo.",
   /* openini          */ L"Abrir arquivo &ini",
 },
 { /* about tab */
   /* box              */ L"Sobre o "APP_NAME,
   /* version          */ L"Versão "APP_VERSION,
   /* author           */ L"Criado por Stefan Sundin",
   /* license          */ APP_NAME L" é um software grátis e de código aberto.\nDivulgue-o aos seus amigos!",
   /* donate           */ L"&Doar",
   /*translation_credit*/ L"Créditos das traduções",
 },

 /* === misc === */
 /* unhook_error       */ L"Houve um erro ao desabilitar o "APP_NAME". Isto foi provavelmente causado pelo Windows ter alterados as injeções (hooks) do "APP_NAME".\n\nSe é a primeira vez que isto acontece, você pode ignorar esta mensagem com segurança e continuar a usar o "APP_NAME".\n\nSe isto acontece repetidamente, você pode ler em nossa wiki (em inglês) como evitar a repetição deste erro (procure por \""APP_NAME" mysteriously stops working\" na página \"About\").",
};
