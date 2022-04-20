// Implementatie van klasse Schema

#include <iostream>
#include <fstream>
#include "standaard.h"
#include "schema.h"
using namespace std;

//*************************************************************************

Schema::Schema() {

// TODO: implementeer zo nodig deze constructor

}  // default constructor

//*************************************************************************

Schema::Schema(int nwNrSpelers) {
  if (integerInBereik(nwNrSpelers, 4, MaxNrSpelers)) {
    nrSpelers = nwNrSpelers;
  } else if (nwNrSpelers < 4) {
    nrSpelers = 4;
  } else {
    nrSpelers = MaxNrSpelers;
  }
}  // constructor met parameter

//*************************************************************************

bool Schema::leesInDeelschema(const char* invoerNaam) {
  ifstream fin;
  int aantalRondes;

  fin.open(invoerNaam);

  if (!fin) {
    cerr << "Kan het bestand niet openen" << endl;
  return false;
  }
  fin >> nrSpelers >> aantalRondes;
  // Check of het aantal spelers binnen de grenzen (4 tot 20) valt
  if (!integerInBereik(nrSpelers, 4, MaxNrSpelers)) {
    cout << "Ongeldig speleraantal" << endl;
  return false;
  }

  // Laadt waardes uit bestand naar vector
  int speler;
  while(!fin.eof()) {
    fin >> speler;
    hulpSchema.push_back(speler);
  }

  // Check of waardes een spelernummer zijn
  vector<int> ronde;
  for (int i : hulpSchema) {
    if ((i + 1) % (hulpSchema.size() / aantalRondes) == 0) {
      ronde.clear();
    }
    if (!integerInBereik(hulpSchema[i], 0, nrSpelers - 1)) {
      return false;
    }
    if (inVector(ronde, hulpSchema[i])) {
      return false;
    }
    ronde.push_back(hulpSchema[i]);
  }

  // TODO, check of het aantal rondes bruikbaar is
  // check of alle getallen in het deelschema geldige spelers voorstellen en samen
  //   een geldig deelschema vormen. HIERBIJ HOEFT NIET OP SYMMETRIE
  //   GECONTROLEERD TE WORDEN.
  // TODO: testen

  return true;

}  // leesInDeelschema

//*************************************************************************

void Schema::drukAfSchema(int schema[MaxGrootteSchema]) {
  for (int i = 0; i < sizeof(schema)/sizeof(schema[0]); i++) {
    if ((i + 1) % nrSpelers == 0) {
      cout << endl << "Ronde " << i / nrSpelers << endl;
    }
    cout << " " << schema[i];
  }
}  // drukAfSchema

//*************************************************************************

bool Schema::bepaalSchemaBT(int schema[MaxGrootteSchema],
                           long long &aantalDeelschemas) { 

// TODO: implementeer deze memberfunctie

  return false;

}  //  bepaalSchemaBT

//*************************************************************************

bool Schema::bepaalMinSchema(int schema[MaxGrootteSchema],
                          long long &aantalDeelschemas, bool bouwWaardeOp) { 

// TODO: implementeer deze memberfunctie

  return false;

}  // bepaalMinSchema
  
//*************************************************************************

void Schema::bepaalSchemaGretig(int schema[MaxGrootteSchema]) {

// TODO: implementeer deze memberfunctie

}  // bepaalSchemaGretig

//*************************************************************************

bool Schema::inVector(vector<int> v, int value) {
  for (int i : v) {
    if (v[i] == value) {
      return true;
    }
  }
  return false;
}