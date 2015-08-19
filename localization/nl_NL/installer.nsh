; AltDrag - nl-NL localization by Merijn Bosma (bosma@xs4all.nl)
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.

!insertmacro MUI_LANGUAGE "Dutch"
LangString L10N_LANG ${LANG_DUTCH} ""

LangString L10N_UPGRADE_TITLE     0 "Al geïnstalleerd"
LangString L10N_UPGRADE_SUBTITLE  0 "Kies hoe u {APP_NAME} wilt installeren."
LangString L10N_UPGRADE_HEADER    0 "${APP_NAME} is al geïnstalleerd op dit systeem. Kies wat u wilt doen en klik op Volgende om door te gaan."
LangString L10N_UPGRADE_UPGRADE   0 "&Upgrade ${APP_NAME} naar ${APP_VERSION}."
LangString L10N_UPGRADE_INI       0 "Uw bestaande instellingen zullen gekopieerd worden naar ${APP_NAME}-old.ini."
LangString L10N_UPGRADE_INSTALL   0 "&Installeer op een nieuwe locatie."
LangString L10N_UPGRADE_UNINSTALL 0 "${APP_NAME} &deinstalleren."

LangString L10N_ALTSHIFT_TITLE    0 "Sneltoets"
LangString L10N_ALTSHIFT_SUBTITLE 0 "De sneltoets Alt + Shift conflicteerd met ${APP_NAME}."
LangString L10N_ALTSHIFT_HEADER   0 "De installatie heeft gedetecteerd dat de Windows sneltoets om de huidige toetsenbordindeling te schakelen Alt + Shift is.$\n$\nAls u ${APP_NAME} gebruikt kunt u Shift gebruiken tijdens het slepen om vensters aan elkaar te koppelen. Dit betekend dat het zeer waarschijnlijk is dat u Alt + Shift in zal drukken, dezelfde combinatie die uw toetsenbordindeling schakelt. ${APP_NAME} zal altijd proberen het foutief schakelen van de toetsenbord indeling te voorkomen, maar dit zal niet lukken als u Shift indrukt vóórdat u het venster gaat slepen.$\n$\nU kunt de sneltoets waarmee u de toetsenbordindeling schakelt veranderen door op deze knop te drukken. Druk op Volgende om door te gaan."
LangString L10N_ALTSHIFT_BUTTON   0 "&Open toetsenbordinstellingen"

LangString L10N_HOOKTIMEOUT_TITLE          0 "Register instellingen"
LangString L10N_HOOKTIMEOUT_SUBTITLE       0 "Optionele instelling om te voorkomen dat ${APP_NAME} onverwachts stopt."
LangString L10N_HOOKTIMEOUT_HEADER         0 "In Windows 7 heeft Microsoft een optie ingebouwd, die toetsenbord- en muishaken uitschakeld als deze te lang niet reageren. Helaas kan deze optie zich onterecht misdragen, zeker als u de pc regelmatig in slaap- of sluimerstand zet, of vergrendeld.$\n$\nWanneer dit gebeurd zult u merken dat ${APP_NAME} zonder waarschuwing stopt met werken, en dat u handmatig {$APP_NAME} zult moeten uitschakelen en weer inschakelen, zodat het weer gaat werken.$\n$\nEr is een register instelling die ervoor zorgt dat Windows langer wacht voordat toetsenbord- en muishaken worden uitgeschakeld, die u aan- en uit kunt zetten met de knoppen hieronder. Deze register instellingen zijn optioneel."
LangString L10N_HOOKTIMEOUT_APPLYBUTTON    0 "Register instellingen &inschakelen"
LangString L10N_HOOKTIMEOUT_REVERTBUTTON   0 "Register instelling &uitschakelen"
LangString L10N_HOOKTIMEOUT_ALREADYAPPLIED 0 "De register instelling is al uitgevoerd."
LangString L10N_HOOKTIMEOUT_FOOTER         0 "De wijzigingen zullen van kracht worden hebben als u opnieuw inlogt."
