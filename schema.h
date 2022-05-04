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

  //Boven elke functie moet een commentaarblokje komen met daarin een korte beschrijving
//van wat de functie doet. Noem daarin tevens de gebruikte parameters: geef hun betekenis
///en geef aan hoe ze eventueel veranderd worden door de functie. Geef bij memberfuncties
//ook aan wat deze met de membervariabelen van het object doen. Let verder op de layout
//(consequent inspringen) en op het overige commentaar bij de programmacode (zinvol en
//kort).

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
    // toegevoegd aan schema[]. 
    // Parameters:
    //   schema[MaxGrootteSchema]: schema dat recursief wordt aangevuld
    //   aantalDeelschemas: het aantal deelschemas dat recursief wordt opgeteld
    // Returns:
    //   true, als het schema compleet is,
    //   false, als het schema niet compleet en correct is
    bool bepaalSchemaBTRecur(int schema[MaxGrootteSchema], 
                             long long &aantalDeelschemas);

                               
    bool bepaalMinSchemaRecur(int schema[MaxGrootteSchema], 
                              long long &aantalDeelschemas, bool bouwWaardeOp);
    bool schemaCompleet();
    bool schemaCorrect();
    float updateRondeMatrix(int schema[MaxGrootteSchema], bool undo);
    float updateRondeMatrix(int s1, int s2, int ronde, bool undo);
    void updateMatrix(int schema[MaxGrootteSchema]);
    void updateMatrix(int s1, int s2, int s3, int s4);
    void undoMatrix(int schema[MaxGrootteSchema]);
    void printMatrices();
    void resetSchema(int schema[MaxGrootteSchema]);
    int vrijInRondeMatrix(int s1, int s2);
    float deelScore(int x);
    void kopieerSchema(int a1[MaxGrootteSchema], int a2[MaxGrootteSchema]);
    float schemaWaarde(int schema[MaxGrootteSchema]);
    void printRondeMatrix();
    void nieuweRondeSpelers();
    bool spelerVrij(int s);
    void maakSpelerVrij(int s);
    void vulEersteRonde();
    bool symmetrie(int s, int schema[MaxGrootteSchema]);
    bool isKleinsteVrijeSpeler(int s);

    int nrSpelers; // aantal spelers bij dit schema
    int schemaGrootte;
    float randScore;
    float waarde;
    float minWaarde;
    int finalGrootte;
    int aantRondes;
    int spelersPRonde;
    // Matrices voor bijhouden teamg
    int voorMatrix[MaxNrSpelers][MaxNrSpelers];
    int tegenMatrix[MaxNrSpelers][MaxNrSpelers];
    int rondeMatrix[MaxNrSpelers][MaxNrSpelers][4];

//  private membervariabelen
    vector<int> hulpSchema;
    vector< vector<int> > vrijeSpelers;
    int minSchema[MaxGrootteSchema];
};

#endif

