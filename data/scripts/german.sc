//------------------------------------------------------------
//
// " ! " - is a line break in a textbox = needs a space either side
//
// Create German text strings
void as_setGermanText()
//------------------------------------------------------------
{
	gui_addKeyAndText("scrollBoxIntroText", "  ^ ^       Willkommen bei Paradroid Drücken Sie die [LINKE STRG-TASTE], um die Wiedergabe zu starten, oder warten Sie auf weitere Anweisungen! Verwenden Sie die [PFEILTASTEN], um durch die Menüs zu navigieren! Drücken Sie [ENTER], um eine Auswahl zu treffen! ! ^ Befreien Sie den Frachter von Robotern, indem Sie sie mit Doppellasern zerstören oder die Kontrolle auf sie übertragen. ! ! Die Steuerung erfolgt ausschließlich über die Tastatur wie folgt:! ! Zu jeder Zeit bewegt die Tastatur den Roboter und das Gedrückthalten des Feuers ermöglicht die Verwendung von Aufzügen und Konsolen. ! ! Wenn Sie auf Feuer drücken, ohne dass eine Taste gedrückt wird, wird der aktuelle Roboter für den Transfer vorbereitet. Der Kontakt mit einem anderen Roboter bei gedrücktem Feuerschlüssel leitet die Übertragung ein. ! ! ! ^! ! ! ! Eine Flotte von Robo-Frachtern auf dem Weg zum Beta-Ceti-System meldete das Betreten eines unbekannten Asteroidenfeldes. Jedes Schiff trägt eine Ladung Kampfdroiden, um die Verteidigung der Außenwelt zu stärken. ! ! Zwei Notsender wurden geborgen. Auf jedem wurden ähnliche Nachrichten gespeichert. Die Schiffe waren von einem der Asteroiden mit einem starken Radionikstrahl bombardiert worden. ! ! Alle Roboter auf den Schiffen, einschließlich der im Lager befindlichen, wurden hyperaktiv. Die Besatzungen melden einen Angriff von Droiden, die sie auf der Brücke isolieren. Sie können das Shuttle nicht erreichen und können nur noch ein paar Stunden durchhalten. ! ! Da diese Leuchtfeuer vor zwei Tagen lokalisiert wurden, können wir nur das Schlimmste befürchten. ! ! Ein Teil der Flotte wurde zuletzt auf dem Weg in den feindlichen Raum gesehen. In feindlichen Händen können die Droiden gegen unsere Streitkräfte eingesetzt werden. ! ! Andocken wäre unmöglich, aber wir können an Bord eines Influence Device-Prototyps beamen. ! ^! ! ! Das 001 Influence-Gerät besteht aus einem Helm, der, wenn er über der Steuereinheit eines Roboters platziert wird, die normalen Aktivitäten dieses Roboters für kurze Zeit unterbrechen kann. Der Helm verfügt über eine eigene Stromversorgung und versorgt den Roboter selbst mit einer verbesserten Leistung. Der Helm verwendet auch einen Energieumhang zum Schutz des Wirtes. ! ! Der Helm ist mit zwei Lasern ausgestattet, die im Gleichstrom montiert sind. Diese sind stromsparend und haben eine langsame Recyclingrate. ! ! Die meisten Ressourcen des Geräts werden dazu verwendet, die Kontrolle über den Host-Roboter zu behalten, da dieser versucht, den normalen Betrieb wieder aufzunehmen. Daher muss der Host-Roboter häufig gewechselt werden, um ein Ausbrennen des Geräts zu verhindern. Um auf einen neuen Roboter umzusteigen, muss das Gerät eine Menge Energie verbrauchen, um diese zu übernehmen. Wird keine Übertragung erreicht, ist das Gerät erneut ein freier Agent. ! ! Weitere Informationen eingehend ...! ^! ! ! Ein Influence-Gerät kann nur bestimmte Daten übertragen, nämlich seinen eigenen Standort und den Standort anderer Roboter in Sichtweite. Diese Daten werden mit bekannten Schiffslayouts auf Ihrem C64-Remote-Terminal zusammengeführt. ! ! Zusätzliche Informationen über das Schiff und die Roboter erhalten Sie, indem Sie über eine Konsole auf den Schiffscomputer zugreifen. Ein kleiner Plan des gesamten Decks sowie eine Seitenansicht des Schiffes sind verfügbar. ! ! Roboter werden auf dem Bildschirm als Symbol mit einer dreistelligen Zahl dargestellt. Die erste angezeigte Ziffer ist die wichtige, die Klasse des Roboters. Es bedeutet auch Stärke. ! ! Um mehr über einen bestimmten Roboter zu erfahren, verwenden Sie das Roboterabfragesystem an einer Konsole. Es sind nur Daten zu Einheiten einer niedrigeren Klasse als Ihrem aktuellen Host verfügbar, da die Sicherheitsfreigabe des Hosts für den Zugriff auf die Konsole verwendet wird. ! ! ! ! ! Weitere Informationen eingehend ...! ^! ! ! ! ! Paradroid von Andrew Braybrook programmiert. ! ! ! Guter Rat von Steve Turner. ! ! ! Allegro5 port von David Berry. ! 2019.! ! ! ! ! Unterstützt von der Allegro5 Bibliothek. ! Scripting Engine von AngelScript. ! Physik von Box2D Physics. ! Filewatcher von James Wynn 2009.! ! ! ! Drücken Sie Feuer, um zu spielen. ! ! ! ! ! ! ^ # ");

	gui_addKeyAndText("gameLeft", "Geh nach links");
	gui_addKeyAndText("gameRight", "Nach rechts bewegen");
	gui_addKeyAndText("gameDown", "Sich abwärts bewegen");
	gui_addKeyAndText("gameUp", "Bewegen Sie sich nach oben");
	gui_addKeyAndText("gameAction", "Führen Sie eine Aktion aus");
	gui_addKeyAndText("gamePause", "Unterbrechen Sie das Spiel");
	gui_addKeyAndText("gameEscape", "Flucht");
	gui_addKeyAndText("consoleAction", "Rufen Sie die Konsole auf");
	gui_addKeyAndText("gameScreenShot", "Mach ein Bildschirmfoto");

	gui_addKeyAndText("startGameButton", "Spiel beginnen");
	gui_addKeyAndText("optionsButton", "Optionen");
	gui_addKeyAndText("tutorialButton", "Tutorial");
	gui_addKeyAndText("briefingButton", "Anweisung");
	gui_addKeyAndText("exitGameButton", "Spiel verlassen");
	gui_addKeyAndText("mainScreenLabel", "Wählen Sie Ihre Option mit der Maus oder der Tastatur.");
	gui_addKeyAndText("guiOptionsMainLabel", "OPTIONEN");
	gui_addKeyAndText("optionVideoLabel", "VIDEO-OPTIONEN");
	gui_addKeyAndText("optionsVideoCheckFullScreen", "Vollbildschirm");
	gui_addKeyAndText("guiOptionsMainVideoButton", "Video");
	gui_addKeyAndText("guiOptionsMainAudioButton", "Audio");
	gui_addKeyAndText("guiOptionsMainGraphicsButton", "Grafik");
	gui_addKeyAndText("guiOptionsMainNetworkButton", "Netzwerk");
	gui_addKeyAndText("guiOptionsMainControlsButton", "Kontrollen");

	gui_addKeyAndText("tutMoveLabel", 		"Tutorial - Bewegen");
	gui_addKeyAndText("tutTransferLabel",	"Tutorial - Übertragungsmodus");
	gui_addKeyAndText("tutTransGameLabel",	"Tutorial - Transferspiel");
	gui_addKeyAndText("tutLiftLabel",		"Tutorial - Aufzüge");
	gui_addKeyAndText("tutTerminalLabel",	"Tutorial - Terminals");
	gui_addKeyAndText("tutHealingLabel",	"Tutorial - Heilen");
	gui_addKeyAndText("tutTipsLabel",		"Tutorial - Gameplay-Tipps");

	gui_addKeyAndText("tutMoveText",		" Benutze die Pfeiltasten zur Bewegung. ! ! Durch Drücken der Aktionstaste wird in Ihre aktuelle Bewegungsrichtung geschossen. ! ! Halten Sie die Aktionstaste gedrückt, wenn keine anderen Tasten gedrückt werden, um den Übertragungsmodus zu starten.");
	gui_addKeyAndText("tutTransText",		" Um einen anderen Droiden zu übernehmen, wechseln Sie in den Übertragungsmodus. ! ! Halten Sie die Aktionstaste gedrückt, ohne dass eine andere Taste gedrückt ist, um diesen Modus aufzurufen. Wenn Sie mit einem anderen Droiden kollidieren, wird die Übertragungsunterroutine gestartet.");
	gui_addKeyAndText("tutTransGameText",	"Um die Kontrolle über einen anderen Droiden zu erlangen, müssen Sie das Circuit Transfer-Spiel gewinnen. Wählen Sie die Seite aus, die Sie verwenden möchten. Drücken Sie die UP / DOWN-Tasten, um Ihren Token auf einen geeigneten Schaltkreis zu verschieben. Drücken Sie die Aktionstaste, um diesen Kreis auszuwählen. Sie müssen mehr von Ihrer Farbe haben, bevor die Zeit abläuft, um zu gewinnen.");
	gui_addKeyAndText("tutLiftText",		"Um zwischen den Decks zu wechseln, benutzen Sie einen Lift. Ein Aufzug wird aktiviert, indem Sie über ein Aufzugsplättchen halten und die Aktionstaste drücken. Verwenden Sie die Richtungstasten, um eine neue Ebene auszuwählen, und drücken Sie die Aktionstaste, um sie auszuwählen.");
	gui_addKeyAndText("tutTerminalText",	"Halten Sie an einem Terminal an und drücken Sie die Aktionstaste. Dadurch erhalten Sie Zugriff auf die Computersysteme des Schiffs. Der Zugriff auf die Droiden-Datenbank wird von der Zugriffsebene Ihres aktuellen Droiden gesteuert.");
	gui_addKeyAndText("tutHealingText",		"Alle Droiden haben einen Gesundheitsindikator, der sich auf sie dreht. Je langsamer sich diese dreht, desto geringer ist der Gesundheitszustand des Droiden. Um Schäden zu reparieren, halten Sie über einer Reparaturplatte an. Ihre Punktzahl wird beim Reparieren automatisch abgezogen.");
	gui_addKeyAndText("tutTipsText",		" - Wenn Sie mit einem anderen Droiden kollidieren, werden beide beschädigt. ! - Einige Droiden sind immun gegen die Disruptionswaffen. ! - Ihre Kontrolle über einen Droiden nimmt mit der Zeit ab. ! - Die erste Zahl auf einem Droiden zeigt die Rangfolge an.");

	gui_addKeyAndText("cancelButton", "Stornieren");
	gui_addKeyAndText("nextButton", "Nächster");

	gui_addKeyAndText("networkLost", "Netzwerk verloren");
	gui_addKeyAndText("serverLost", "Die Verbindung zum Server ist unterbrochen.");
	gui_addKeyAndText("pausedTitle", "Angehalten");
	gui_addKeyAndText("pausedText", "Das Spiel pausiert");
}

