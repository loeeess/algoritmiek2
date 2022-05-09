// Definitie van klasse Schema
// Opdracht 2 gemaakt door Lochyin Cheng (s3383040) en Loes Crama (s3335798)

#ifndef SchemaHVar  // voorkom dat dit bestand meerdere keren
#define SchemaHVar  // ge-include wordt

#include "constantes.h"
#include <vector>
using namespace std;

class Schema
{ public:

    // Default constructor
    Schema();

    // Constructor met parameter
    // Controleer of de parameter nwNrSpelers een bruikbaar aantal spelers
    // is, binnen de grenzen van de opdracht.
    Schema(int nwNrSpelers);

    // Lees een deelschema, bestaande uit een geheel aantal rondes,
    // in uit invoernaam. De invoer bestaat uit
    // * een regel met twee getallen: het aantal spelers en het aantal rondes
    // * voor elke ronde (in hun volgorde in het deelschema, dus ronde 0,
    //   ronde 1, enzovoort) een regel met de spelers die in die ronde zijn
    //   ingedeeld (eveneens in de volgorde van het deelschema
    // Getallen op dezelfde regel worden gescheiden door enkele spaties.
    // Controleer of
    // * het bestand invoernaam te openen is
    // * het aantal spelers bruikbaar is, binnen de grenzen van de opdracht
    // * het aantal rondes bruikbaar is
    // * alle getallen in het deelschema geldige spelers voorstellen en samen
    //   een geldig deelschema vormen. HIERBIJ HOEFT NIET OP SYMMETRIE
    //   GECONTROLEERD TE WORDEN.
    // Retourneer:
    // * true, als alle controles goed uitpakken.
    //   In dat geval is alle ingelezen informatie in het object opgeslagen.
    // * false, anders
    bool leesInDeelschema(const char* invoerNaam);

    // Druk parameter schema overzichtelijk af op het scherm.
    // Pre:
    // * membervariabele nrSpelers heeft een bruikbare waarde
    // * parameter schema bevat een compleet schema dat past bij nrSpelers
    void drukAfSchema(int schema[MaxGrootteSchema]);

    // Bepaal met behulp van backtracking een geldig schema voor het huidige
    // aantal spelers. Het maakt niet uit wat de `waarde' van dit schema is
    // (zie ook bepaalMinSchema).
    // Als er een deelschema van een aantal rondes is ingelezen, bouw
    // dan daarop verder.
    // Retourneer:
    // * true, als het lukt om een schema te bepalen
    // * false, als het niet lukt om een schema te bepalen
    // Pre:
    // * membervariabele nrSpelers heeft een bruikbare waarde
    // Post:
    // * als het lukt om een geldig schema te bepalen, bevat parameter `schema'
    //   zo'n schema. Alle rondes zitten daar dan achter elkaar, beginnend
    //   op positie 0 van het array.
    // * aantalDeelschemas is gelijk aan het aantal deelschemas dat we
    //   hebben gezien bij het bepalen van een schema
    bool bepaalSchemaBT(int schema[MaxGrootteSchema],
                         long long &aantalDeelschemas);

    // Bepaal met behulp van backtracking een geldig schema voor het huidige
    // aantal spelers, met een zo klein mogelijke `waarde'.
    // Als er een deelschema van een aantal rondes is ingelezen, bouw
    // dan daarop verder.
    // Als parameter bouwWaardeOp = true, wordt de waarde van het schema
    // al opgebouwd tijdens het opbouwen van het schema zelf, zodat je kunt
    // backtracken als de waarde van het huidige deelschema al slechter is
    // dan de waarde van het beste complete schema tot nu toe.
    // Als parameter bouwWaardeOp = false, wordt de waarde van het schema
    // pas berekend als er een compleet geldig schema is.
    // Retourneer:
    // * true, als het lukt om een schema te bepalen
    // * false, als het niet lukt om een schema te bepalen
    // Pre:
    // * membervariabele nrSpelers heeft een bruikbare waarde
    // Post:
    // * als het lukt om een geldig schema te bepalen, bevat parameter `schema'
    //   een schema met een minimale `waarde'. Alle rondes zitten daar dan
    //   achter elkaar, beginnend op positie 0 van het array.
    // * aantalDeelschemas is gelijk aan het aantal deelschemas dat we
    //   hebben gezien bij het bepalen van een minimaal schema
    bool bepaalMinSchema(int schema[MaxGrootteSchema],
                         long long &aantalDeelschemas, bool bouwWaardeOp);

    // Bepaal op een gretige manier een schema voor het huidige aantal
    // spelers. Ofwel:
    // * Vul het schema positie voor positie.
    // * Kies bij elke positie een speler die `zo goed mogelijk'
    //   aan de eisen voor een schema voldoet.
    // * Herzie nooit een eerder gemaakte keuze
    // Als er een deelschema van een aantal rondes is ingelezen, bouw
    // dan daarop verder.
    // Als het niet lukt om aan alle eisen voor een geldig schema te voldoen,
    // BEPAAL DAN TOCH EEN COMPLEET SCHEMA, dat `zo goed mogelijk' aan
    // de eisen voldoet.
    // Pre:
    // * membervariabele nrSpelers heeft een bruikbare waarde
    // Post:
    // * parameter `schema' bevat een compleet schema voor het huidige
    //   aantal spelers
    void bepaalSchemaGretig(int schema[MaxGrootteSchema]);

  private:
// Uw eigen private memberfuncties

    // Bepaal of een waarde in een vector voorkomt
    bool inVector(vector<int> v, int value);

    // Lees het deelschema in door het meegeven van een schema
    // Parameters:
    //   schema[MaxGrootteSchema]: schema dat moet worden meegeleverd
    void leesInDeelschema(int schema[MaxGrootteSchema]);

    // Bepaal schema recursief met behulp van backtracking
    // (aangeroepen door bepaalSchemaBT()) 
    // Als het schema niet correct of compleet is, moet het schema verder 
    // worden opgebouwd door middel van backtracking.
    // Voor elke speler wordt er gekeken of er een geldig schema kan worden
    // gemaakt. Wanneer de speler nog beschikbaar is voor de ronde en
    // geen symmetrie veroorzaakt in het al bestaande schema, kan deze worden
    // toegevoegd aan schema[]. Per speler wordt er +1 toegevoegd aan het 
    // aantalDeelschemas en de voor- en tegenMatrix worden geupdated met een 
    // extra voor/tegen beurt voor de spelers. De recursie wordt aangeroepen,
    // om te controleren of het schema nu wel compleet/correct is. Als dit 
    // niet het geval is, worden de beurten uit de voor/tegenMatrix verwijderd
    // en -1 gedaan bij de schemaGrootte. Ook wordt de speler weer teruggezet 
    // in de vrijeSpelers vector. De functie returnt hier false.
    // Parameters:
    //   schema[MaxGrootteSchema]: schema dat recursief wordt aangevuld
    //   aantalDeelschemas: het aantal deelschemas dat recursief wordt opgeteld
    // Returns:
    //   true: als het schema compleet is,
    //   false: als het schema niet compleet en correct is
    bool bepaalSchemaBTRecur(int schema[MaxGrootteSchema], 
                             long long &aantalDeelschemas);

    // Bepaal minimaal schema recursief met behulp van backtracking
    // (aangeroepen door bepaalSchemaBT())
    // Ook hier wordt weer gecontroleerd of het schema correct/compleet is.
    // Als dit niet zo is, wordt er per speler gekeken of er geen symmetrie 
    // plaatsvindt en dat de speler nog vrij is om te worden ingedeeld. 
    // Als dit zo is, kan de speler worden toegevoegd aan het schema en wordt
    // (net zoals bij bepaalSchemaBTRecur) het aantalDeelschemas en schemaGrootte 
    // +1 gedaan en de matrices geupdated. Er wordt een deelwaarde berekend en 
    // opgeteld bij de totale waarde. Als bouwWaardeOp false is en de waarde die er
    // is kleiner is dan de minWaarde op dat moment, wordt de functie recursief 
    // aangeroepen. Wanneer deze true returnt, is het schema klaar. Anders worden
    // de matrices hersteld van de laatste beurt, de deelWaarde van de waarde afgehaald
    // en wordt de speler weer teruggezet in de vrijeSpelers vector. De functie 
    // returnt false, in de vorm van variabele schemaGevonden.
    // Parameters:
    //   schema[MaxGrootteSchema]: schema dat recursief wordt aangevuld
    //   aantalDeelschemas: het aantal deelschemas dat recursief wordt opgeteld
    //   bouwWaardeOp: true voor aanroep met onderweg opbouwen, 
    //      false voor aanroep zonder 
    // Returns:
    //   schemaGevonden = true: als het schema compleet is,
    //   schemaGevonden = false: als het schema niet compleet en correct is                    
    bool bepaalMinSchemaRecur(int schema[MaxGrootteSchema], 
                              long long &aantalDeelschemas, bool bouwWaardeOp);
    
    // Loop door de voor- en tegenMatrix en controleert of de voorMatrix
    // overal een waarde van 1 bevat en tegenMatrix overal een waarde van 
    // 2 bevat. Hier wordt geen rekening gehouden met de diagonalen.
    // Returns:
    //   true: als het schema compleet is
    //   false: als het schema niet compleet is
    bool schemaCompleet();

    // Loop door de voor- en tegenMatrix en controleert of de voorMatrix
    // nergens een waarde hoger dan 1 bevat en tegenMatrix nergens
    // een waarde hoger dan 2 bevat. Hier wordt geen rekening gehouden 
    // met de diagonalen.
    // Returns:
    //   true: als het schema correct is
    //   false: als het schema niet correct is
    bool schemaCorrect();

    // Berekent de score voor het schema dmv de function overloading met
    // updateRondeMatrix(int s1, int s2, int ronde, bool undo). Per speler
    // in de ronde van de matrix wordt de score berekend. Zie onderstaande 
    // functie voor toelichting op berekening.
    // Parameters:
    //    schema[MaxGrootteSchema]: tot nu toe opgebouwde schema
    //    undo: boolean die bepaalt of de deelscore moet worden berekend of niet
    //      in verband met het bepalen van het minimale schema
    // Returns:
    //   score: een double met de berekende score
    double updateRondeMatrix(int schema[MaxGrootteSchema], bool undo);

    // Berekent de score voor het schema dmv de rondeMatrix. Er wordt een 
    // deelscore en deelwaarde berekend van het toevoegen van de schemawaarde.
    // Berekent welke spelers elkaar hebben gezien en in welke ronde dit voorkwam.
    // Parameters:
    //    s1: speler 1
    //    s2: speler 2
    //    ronde: huidige ronde
    //    undo: boolean die bepaalt of de deelscore moet worden berekend of niet
    //      in verband met het bepalen van het minimale schema
    // Returns:
    //   0: als undo false is
    //   deelscore: als undo true is
    double updateRondeMatrix(int s1, int s2, int ronde, bool undo);

    // Vult de voor- en tegenMatrix aan voor elke nieuwe schemawaarde, waarbij
    // rekening wordt gehouden met de verschillende tafelgroottes.
    // Parameters:
    //    schema[MaxGrootteSchema]: tot nu toe opgebouwde schema
    void updateMatrix(int schema[MaxGrootteSchema]);

    // Vult de voor- en tegenMatrix aan voor elke nieuwe tafel
    // Parameters:
    //    s1: speler 1
    //    s2: speler 2
    //    s3: speler 3
    //    s4: speler 4
    void updateMatrix(int s1, int s2, int s3, int s4);

    // Verwijdert de laatste schemawaarde in de voor- en tegenMatrix aan voor 
    // waarbij rekening wordt gehouden met de verschillende tafelgroottes.
    // Parameters:
    //    schema[MaxGrootteSchema]: tot nu toe opgebouwde schema
    void undoMatrix(int schema[MaxGrootteSchema]);

    // Print de voor- en tegenMatrix in de terminal
    void printMatrices();

    // Reset de waardes in de voor- en tegenMatrix op 0 en de waardes in de 
    // rondeMatrix op -1.
    // Parameters:
    //    schema[MaxGrootteSchema]: tot nu toe opgebouwde schema
    void resetSchema(int schema[MaxGrootteSchema]);

    // Zoek naar de eerstvolgende lege plek in de rondeMatrix
    // Parameters:
    //    s1: speler 1
    //    s2: speler 2
    // Returns:
    //    i: de eerstvolgende vrije plek in de rondeMatrix
    int vrijInRondeMatrix(int s1, int s2);

    // Bereken de deelscore met behulp van het aantal rondes 
    // Parameters:
    //    x: het aantal rondes tussen 2x dat 2 spelers elkaar zien
    // Returns:
    //    s * s: de berekende deelscores
    double deelScore(int x);

    // Kopieert een schema naar een ander schema
    // Parameters:
    //    a1: het schema dat je wilt kopieëren
    //    a2: het schema waarin je de kopie wil zetten
    void kopieerSchema(int a1[MaxGrootteSchema], int a2[MaxGrootteSchema]);

    // Berekent de waarde van het schema, waarin de rondeMatrix eerst
    // wordt reset en vervolgens de deelwaarde wordt berekend door de 
    // grootte van het schema.
    // Parameters:
    //    schema[MaxGrootteSchema]: tot nu toe opgebouwde schema
    // Returns:
    //    deelwaarde: de opgetelde waarde van het schema
    double schemaWaarde(int schema[MaxGrootteSchema]);
    
    // Print de rondeMatrix in de terminal
    void printRondeMatrix();
    
    // Voeg een nieuwe ronde toe met daarin alle vrije spelers, ingeladen
    // in een nieuwe vector in de vector vrijeSpelers
    void nieuweRondeSpelers();
    
    // Check of een speler nog niet bezet is in de ronde
    // Parameters:
    //    s: speler om te checken
    // Returns:
    //    true: als de speler nog vrij is
    //    false: als de speler bezet is
    bool spelerVrij(int s);
    
    // Zet de speler terug in de vrijeSpelers vector. Als er een ronde wordt
    // teruggegaan, wordt de grootte van de vector veranderd, zodat de speler
    // kan worden toegevoegd aan de juiste ronde.
    // Parameters:
    //    s: speler om toe te voegen
    void maakSpelerVrij(int s);
    
    // Vul eerste ronde in met 0, 1, 2, ..., nrSpelers, zodat de symmetrie in
    // het schema kan worden voorkomen. Dit is alleen mogelijk als er nog geen
    // deelschema is meegegeven.
    void vulEersteRonde();
    
    // Check of het schema dat wordt gemaakt een vorm van symmetrie bevat.
    // Eerst wordt er gecheckt of een tafel symmetrie bevat. Er wordt gekeken
    // of de eerste spelers in een tafel de kleinste vrije speler is.
    // Dan wordt er gecheckt of er symmetrie plaatsvindt tussen de rondes.
    // De speler in de eerdere tafel moet een kleinere waarde hebben dan de 
    // hieropvolgende tafels voor het voorkmoen van symmetrie.
    // Parameters:
    //    s: speler om te checken
    //    schema[MaxGrootteSchema]: tot nu toe opgebouwde schema
    // Returns:
    //    true: als er symmetrie in het schema aanwezig is
    //    false: als er geen symmetrie in het schema aanwezig is
    bool symmetrie(int s, int schema[MaxGrootteSchema]);
    
    // Check of eerste speler van een tafel groter is dan de eerste speler van
    // de vorige tafel
    // Parameters:
    //     s: speler om te checken
    //     schema[MaxGrootteSchema]: tot nu toe opgebouwde schema
    // Returns:
    //     true: als de speler de groter is 
    ///    false: als de speler niet groter is
    bool isGroterDanVorigeTafel(int s, int schema[MaxGrootteSchema]);
    
    // Check of hulpSchema correct is. Er wordt gecheckt of de waardes in
    // het schema een geldig spelernummer zijn. Ook wordt er checkt of de 
    // spelers niet meer dan 1x zijn toegewezen. 
    // Parameters:
    //    aantalRondes: aantal rondes dat er tot nu toe is
    // Returns: 
    //    true: als het hulpSchema correct is
    //    false als het hulpSchema niet correct is
    bool hulpSchemaCorrect(int aantalRondes);

    // Aantal spelers bij dit schema
    int nrSpelers; 

    // De grootte van het schema tot nu toe
    int schemaGrootte;

    // Gemiddeld aantal rondes waartussen spelers elkaar zien
    double randScore;
    
    // Waarde die wordt bepaald voor het minimale schema
    double waarde;
    
    // Minimale waarde voor het berekenen van de minimale scores 
    double minWaarde;
    
    // De uiteindelijke grootte van het schema
    int finalGrootte;
    
    // Het aantal rondes tot nu toe
    int aantRondes;
    
    // Het aantal spelers dat in een ronde voorkomt
    int spelersPRonde;
    
    // Matrix waarin de teams zijn genoteerd
    int voorMatrix[MaxNrSpelers][MaxNrSpelers]; 
    
    // Matrix waarin de tegenstanders worden genoteerd
    int tegenMatrix[MaxNrSpelers][MaxNrSpelers]; 

    // Matrix waarin de rondes worden genoteerd
    int rondeMatrix[MaxNrSpelers][MaxNrSpelers][MaxNrSpelers];

    // Hulpschema om het volledige schema mee op te bouwen
    vector<int> hulpSchema;

    // De vrije spelers per ronde
    vector< vector<int> > vrijeSpelers;

    // Minimaal schema
    int minSchema[MaxGrootteSchema];
};

#endif

