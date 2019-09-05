//------------------------------------------------------------
//
// " ! " - is a line break in a textbox = needs a space either side
//
// Create Italian text strings
void as_setItalianText()
//------------------------------------------------------------
{
	gui_addKeyAndText("scrollBoxIntroText",		"  ^ ^       Benvenuto in Paradosso. ! ! Premere [LEFT CTRL KEY] per giocare o attendere ulteriori istruzioni! Usa i [TASTI FRECCIA] per navigare nei menu! Premere [ENTER] per effettuare una scelta! ! ^ Libera il cargo dei robot distruggendoli con laser gemelli o trasferendo il controllo su di essi. ! ! Il controllo avviene solo tramite tastiera, come segue:! ! In ogni momento la tastiera muove il robot e tenendo premuto il fuoco consentirà l'uso di ascensori e console. ! ! Inoltre, premendo il pulsante di accensione senza premere i tasti, verrà preparato il robot corrente per il trasferimento. Il contatto con un altro robot con il tasto di accensione giù avvierà il trasferimento. ! ! ! ^! ! ! ! Una flotta di Mercanti Robo in viaggio verso il sistema Beta Ceti ha riferito di aver inserito un campo inesplorato di asteroidi. Ogni nave trasporta un carico di droidi da battaglia per rafforzare le difese esterne. ! ! Due fari di soccorso sono stati recuperati. Messaggi simili sono stati memorizzati su ciascuno. Le navi erano state bombardate da un potente raggio radionico proveniente da uno degli asteroidi. ! ! Tutti i robot sulle navi, compresi quelli in deposito, sono diventati iper-attivi. Gli equipaggi riferiscono di un attacco da parte di droidi, isolandoli sul ponte. Non possono raggiungere lo shuttle e possono resistere solo per un paio d'ore. ! ! Dato che questi beacon sono stati localizzati due giorni fa, possiamo solo temere il peggio. ! ! Parte della flotta è stata vista per l'ultima volta in direzione dello spazio nemico. Nelle mani dei nemici i droidi possono essere usati contro le nostre forze. ! ! L'attracco sarebbe impossibile, ma possiamo trasmettere a bordo di un prototipo di dispositivo di influenza. ! ^! ! ! Il dispositivo 001 Influence è costituito da un casco che, quando posizionato su un'unità di controllo del robot, può interrompere le normali attività di quel robot per un breve periodo. Il casco ha il proprio alimentatore e alimenta il robot stesso, con capacità potenziate. Il casco utilizza anche un mantello energetico per la protezione dell'ospite. ! ! Il casco è equipaggiato con laser gemelli montati in una corrente. Questi sono a bassa potenza e hanno un tasso di riciclaggio lento. ! ! La maggior parte delle risorse del dispositivo vengono indirizzate verso il mantenimento del controllo del robot host, nel tentativo di riprendere il normale funzionamento. È quindi necessario cambiare spesso il robot host per evitare che il dispositivo si scarichi. Il trasferimento a un nuovo robot richiede che il dispositivo prosciughi la sua energia per poterlo prendere. Il mancato raggiungimento dei risultati di trasferimento nel dispositivo è di nuovo un agente libero. ! ! Ulteriori informazioni in arrivo ...! ^! ! ! Un dispositivo Influence può trasmettere solo determinati dati, vale a dire la sua posizione e la posizione di altri robot nel raggio visivo. Questi dati vengono uniti con i layout della nave noti sul terminale remoto C64. ! ! Ulteriori informazioni sulla nave e sui robot possono essere ottenute accedendo al computer della nave su una console. È disponibile una planimetria su scala ridotta dell'intero mazzo, oltre a un prospetto laterale della nave. ! ! I robot sono rappresentati sullo schermo come un simbolo che mostra un numero di tre cifre. La prima cifra mostrata è quella importante, la classe del robot. Denota anche la forza. ! ! Per saperne di più su un determinato robot, utilizzare il sistema di ispezione del robot su una console. Sono disponibili solo i dati relativi alle unità di una classe inferiore rispetto all'host corrente, in quanto è il nulla osta di sicurezza dell'host utilizzato per accedere alla console. ! ! ! ! ! Ulteriori informazioni in arrivo ...! ^! ! ! ! ! Paradroid programmato da Andrew Braybrook. ! ! ! Un buon consiglio di Steve Turner. ! ! ! Porta SDL di David Berry. ! 2019.! ! ! ! ! Alimentato dalla libreria Allegro5. ! Motore di scripting di AngelScript. ! Fisica di Box2D. ! Filewatcher di James Wynn 2009.! ! ! ! Premi il fuoco per giocare. ! ! ! ! ! ! ^ # ");

	gui_addKeyAndText("gameLeft", "Muovere a sinistra");
	gui_addKeyAndText("gameRight", "Vai a destra");
	gui_addKeyAndText("gameDown", "Abbassati");
	gui_addKeyAndText("gameUp", "Andare avanti");
	gui_addKeyAndText("gameAction", "Esegui un'azione");
	gui_addKeyAndText("gamePause", "Metti in pausa il gioco");
	gui_addKeyAndText("gameEscape", "Fuga");
	gui_addKeyAndText("consoleAction", "Richiama la console");
	gui_addKeyAndText("gameScreenShot", "Fai uno screenshot");

	gui_addKeyAndText("startGameButton", "Inizia il gioco");
	gui_addKeyAndText("optionsButton", "Opzioni");
	gui_addKeyAndText("tutorialButton", "lezione");
	gui_addKeyAndText("briefingButton", "Riunione");
	gui_addKeyAndText("exitGameButton", "Esci dal gioco");
	gui_addKeyAndText("mainScreenLabel", "Seleziona la tua opzione usando il mouse o la tastiera.");
	gui_addKeyAndText("guiOptionsMainLabel", "OPZIONI");
	gui_addKeyAndText("optionVideoLabel", "OPZIONI VIDEO");
	gui_addKeyAndText("optionsVideoCheckFullScreen", "A schermo intero");
	gui_addKeyAndText("guiOptionsMainVideoButton", "Video");
	gui_addKeyAndText("guiOptionsMainAudioButton", "Audio");
	gui_addKeyAndText("guiOptionsMainGraphicsButton", "Grafica");
	gui_addKeyAndText("guiOptionsMainNetworkButton", "Rete");
	gui_addKeyAndText("guiOptionsMainControlsButton", "controlli");

	gui_addKeyAndText("tutMoveLabel", 		"Tutorial - Move");
	gui_addKeyAndText("tutTransferLabel",	"Esercitazione - Modalità di trasferimento");
	gui_addKeyAndText("tutTransGameLabel",	"Tutorial - Trasferisci gioco");
	gui_addKeyAndText("tutLiftLabel",		"Tutorial - Ascensori");
	gui_addKeyAndText("tutTerminalLabel",	"Tutorial - Terminali");
	gui_addKeyAndText("tutHealingLabel",	"Tutorial - Guarigione");
	gui_addKeyAndText("tutTipsLabel",		"Tutorial - consigli di gioco");

	gui_addKeyAndText("tutMoveText",		" Usa le frecce per muoverti. ! ! Premendo il tasto azione si spara nella direzione corrente del movimento. ! ! Tenere premuto il tasto di azione quando non si preme nessun altro tasto per avviare la modalità di trasferimento.");
	gui_addKeyAndText("tutTransText",		" Per prendere il controllo di un altro droide, entra in modalità Trasferimento. ! ! Premi e tieni premuto il tasto Azione senza altri tasti per accedere a questa modalità. Scontrandosi con un altro droide si avvierà la subroutine di trasferimento.");
	gui_addKeyAndText("tutTransGameText",	"Per ottenere il controllo di un altro droide, devi vincere il gioco di trasferimento dei circuiti. Seleziona il lato che utilizzerai. Premere i tasti SU / GIÙ per spostare il token su un circuito adatto. Premi il tasto Azione per selezionare quel circuito. Devi avere più colore prima che scada il tempo per vincere.");
	gui_addKeyAndText("tutLiftText",		"Per passare da un mazzo all'altro, devi usare un ascensore. Un ascensore viene attivato fermandosi su una tessera ascensore e premendo il tasto azione. Utilizzare i tasti di direzione per selezionare un nuovo livello e premere il tasto Azione per selezionarlo.");
	gui_addKeyAndText("tutTerminalText",	"Fermati a un terminale e premi il tasto Azione. Questo ti garantirà l'accesso ai sistemi informatici delle navi. L'accesso al database dei droidi è controllato dal livello di accesso del tuo droide corrente.");
	gui_addKeyAndText("tutHealingText",		"Tutti i droidi hanno un indicatore di salute che gira su di loro. Più lento è questo giro, più basso è il livello di salute del droide. Per riparare i danni, fermati su una tessera di riparazione. Il punteggio viene automaticamente sottratto durante la riparazione.");
	gui_addKeyAndText("tutTipsText",		" - Scontrarsi con un altro droide danneggerà entrambi. ! - Alcuni droidi sono immuni alle armi di disgregazione. ! - Il tuo controllo su un droide si indebolisce con il tempo. ! - Il primo numero su un droide indica la sua classifica.");

	gui_addKeyAndText("cancelButton", "Annulla");
	gui_addKeyAndText("nextButton", "Il prossimo");

	gui_addKeyAndText("networkLost", "Rete persa");
	gui_addKeyAndText("serverLost", "La connessione al server non è attiva.");
	gui_addKeyAndText("pausedTitle", "In pausa");
	gui_addKeyAndText("pausedText", "Gioco in pausa");
}
