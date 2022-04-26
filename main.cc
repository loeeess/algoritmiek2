// Hoofdprogramma voor oplossing voor tweede programmeeropdracht Algoritmiek,
// voorjaar 2022: Klaverjassen.
//
// Biedt de gebruiker een menustructuur om voor een door hem/haar te bepalen
// aantal spelers:
// * met backtracking een geldig schema te bepalen
// * (met backtracking) een minimaal geldig schema te bepalen, waarbij
//   de waarde van het schema onderweg wordt opgebouwd
// * (met backtracking) een minimaal geldig schema te bepalen, waarbij
//   de waarde van het schema aan het eind wordt berekend
// * met een gretig algoritme een schema te bepalen
//
// Namen + nummers studenten, datum

#include <iostream>
#include <ctime>  // voor clock() en clock_t
#include "constantes.h"
#include "schema.h"
using namespace std;
const int MaxBestandsNaamLengte = 30; // maximale lengte van een bestandsnaam

//*************************************************************************

// Schrijf het menu voor een toernooi op het scherm,
// en vraag een keuze van de gebruiker.
// Retourneer: de keuze van de gebruiker
int keuzeUitMenu ()
{ int keuze;

  cout << endl;
  cout << "1. Een schema bepalen (backtracking)" << endl;
  cout << "2. Een minimaal schema bepalen (backtracking)" << endl;
  cout << "     waarbij waarde van schema onderweg wordt opgebouwd" << endl;
  cout << "3. Een minimaal schema bepalen (backtracking)" << endl;
  cout << "     waarbij waarde van schema aan het eind wordt berekend" << endl;
  cout << "4. Een schema bepalen (gretig)" << endl;
  cout << "5. Stoppen met dit toernooi" << endl;
  cout << endl;
  cout << "Maak een keuze: ";
  cin >> keuze;

  return keuze;

}  // keuzeUitMenu

//*************************************************************************

// Voer de actie uit die hoort bij een keuze 1, 2 of 3 in het menu voor
// instantie s1.
void VerwerkKeuze123 (Schema *s1, int keuze)
{ int schema[MaxGrootteSchema];
  long long aantalDeelschemas;  // aantal deelschemas dat we hebben
                                // opgebouwd
  clock_t t1, t2;
  bool bs = false;  // lukt het om een (minimaal) schema te bepalen

  t1 = clock ();
  cout << endl;
  switch (keuze)
  { case 1:
        cout << "bepaalSchema" << endl;
        bs = s1->bepaalSchemaBT (schema, aantalDeelschemas);
        break;
    case 2:
        cout << "bepaalMinSchema (waarde onderweg opbouwen)" << endl;
        bs = s1->bepaalMinSchema (schema, aantalDeelschemas, true);
        break;
    case 3:
        cout << "bepaalMinSchema (waarde aan eind berekenen)" << endl;
        bs = s1->bepaalMinSchema (schema, aantalDeelschemas, false);
        break;
    default: ;   // komt niet voor
  }
  t2 = clock ();

  if (bs)
    s1 -> drukAfSchema (schema);
  else
  { cout << endl;
    cout << "Helaas, we vonden geen geldig schema";
  }
  cout << endl;
  cout << "We hebben hiervoor " << aantalDeelschemas
       << " deelschemas opgebouwd." << endl;
  cout << "Dit kostte " << (t2-t1) << " clock ticks, ofwel "
   << (((double)(t2-t1))/CLOCKS_PER_SEC) << " seconden." << endl;

}  // verwerkKeuze123

//*************************************************************************

// Bied de gebruiker een menu om schemas te bepalen voor instantie s1:
// een schema (met backtracking), een zo kort mogelijk schema (met
// backtracking), en een schema (met een gretig algoritme).
// Roep vervolgens s1 aan voor het gekozen soort schema.
// Herhaal dit totdat de gebruiker aangeeft te willen stoppen.
void menuVoorInstantie (Schema *s1)
{ int keuze,
      schema[MaxGrootteSchema];

  keuze = 0;
  while (keuze!=5)
  {
    keuze = keuzeUitMenu ();

    switch (keuze)
    { case 1:
      case 2:
      case 3: VerwerkKeuze123 (s1, keuze);
              break;
      case 4: cout << endl;
              cout << "bepaalSchemaGretig" << endl;
              s1 -> bepaalSchemaGretig (schema);
              s1 -> drukAfSchema (schema);
              break;
      case 5: break;
      default: cout << endl;
               cout << "Voer een goede keuze in!" << endl;
    }  // switch

  }  // while

}  // menuVoorInstantie

//*************************************************************************

void hoofdmenu ()
{ Schema *s1;  // pointer, om makkeijk nieuwe objecten te kunnen maken
               // en weer weg te gooien
  int keuze,
      nrSpelers;
  char invoernaam[MaxBestandsNaamLengte+1];

  do
  {
    cout << endl;
    cout << "1. Een nieuw toernooi starten" << endl;
    cout << "2. Een toernooi met deelschema inlezen" << endl;
    cout << "3. Stoppen" << endl;
    cout << endl;
    cout << "Maak een keuze: ";
    cin >> keuze;
    switch (keuze)
    { case 1: cout << endl;
              cout << "Geef het aantal spelers (4.." << MaxNrSpelers
                   << ", en 0 of 1 modulo 4): ";
              cin >> nrSpelers;
              s1 = new Schema (nrSpelers);
              menuVoorInstantie (s1);
              delete s1;  // netjes opruimen
              break;
      case 2: s1 = new Schema ();
              cout << "Geef de naam van het tekstbestand met de invoer: ";
              cin >> invoernaam;
              if (s1 -> leesInDeelschema (invoernaam))
              { //s1 -> drukAf ();
                menuVoorInstantie (s1);
              }
              delete s1;  // netjes opruimen
      case 3: break;
      default: cout << endl;
               cout << "Voer een goede keuze in!" << endl;
    }

  } while (keuze!=3);

}  // hoofdmenu

//*************************************************************************
  
int main ()
{
  hoofdmenu ();

  return 0;

}
