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
  schemaGrootte = 0;
  
  for (int i = 0; i < nrSpelers; i++) {
    for (int j = 0; j < nrSpelers; j++) {
      voorMatrix[i][j] = 0;
      tegenMatrix[i][j] = 0;
    }
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

  for (int i = 0; i < nrSpelers; i++) {
    for (int j = 0; j < nrSpelers; j++) {
      voorMatrix[i][j] = 0;
      tegenMatrix[i][j] = 0;
    }
  }

  // Laadt waardes uit bestand naar vector
  int speler;
  schemaGrootte = 0;
  while(!fin.eof()) {
    fin >> speler;
    hulpSchema.push_back(speler);
    schemaGrootte++;
  }

  // Check of waardes een spelernummer zijn
  vector<int> ronde;
  for (int i : hulpSchema) {
    if (aantalRondes != 0 || (i + 1) % (ronde.size() / aantalRondes) == 0) {
      ronde.clear();
    }
    if (!integerInBereik(hulpSchema[i], 0, nrSpelers - 1)) {
      cout << "Error: spelernummer buiten bereik" << endl;
      hulpSchema.clear();
      return false;
    }
    if (inVector(ronde, hulpSchema[i])) {
      cout << "Error: zelfde speler meer dan een keer toegewezen" << endl;
      hulpSchema.clear();
      return false;
    }
    ronde.push_back(hulpSchema[i]);
  }

  return true;

}  // leesInDeelschema

//*************************************************************************

void Schema::drukAfSchema(int schema[MaxGrootteSchema]) {
  for (int i = 0; i < schemaGrootte; i++) {
    if ((i + 1) % nrSpelers == 0) {
      cout << endl << "Ronde " << i / nrSpelers << endl;
    }
    cout << " " << schema[i];
  }
}  // drukAfSchema

//*************************************************************************

bool Schema::bepaalSchemaBT(int schema[MaxGrootteSchema], 
                            long long &aantalDeelschemas) { 
  if (nrSpelers % 4 == 2 || nrSpelers % 4 == 3) {
    return false;
  }
                              
  leesInDeelschema(int schema[MaxGrootteSchema]);
  

  return true;

}  //  bepaalSchemaBT

//*************************************************************************

bool Schema::bepaalMinSchema(int schema[MaxGrootteSchema],
                             long long &aantalDeelschemas, bool bouwWaardeOp) { 
  if (nrSpelers % 4 == 2 || nrSpelers % 4 == 3) {
    return false;
  }
  leesInDeelschema(int schema[MaxGrootteSchema]);

  return true;

}  // bepaalMinSchema
  
//*************************************************************************

void Schema::bepaalSchemaGretig(int schema[MaxGrootteSchema]) {

  leesInDeelschema(int schema[MaxGrootteSchema]);

}  // bepaalSchemaGretig

//*************************************************************************

bool Schema::inVector(vector<int> v, int value) {
  for (int i : v) {
    if (v[i] == value) {
      return true;
    }
  }
  return false;
}  // inVector

//*************************************************************************

void Schema::leesInDeelschema(int schema[MaxGrootteSchema]) {
  if (hulpSchema.size() > 0) {
    for (int i = 0; i < hulpSchema.size(); i++) {
      schema[i] = hulpSchema[i];
      if (i % 4 == 1) {
        updateMatrix(hulpSchema[i], hulpSchema[i+1], hulpSchema[i+2], 
                     hulpSchema[i+3]);
      }
    }
  }
}  // leesInDeelschema

//*************************************************************************

void Schema::updateMatrix(int s1, int s2, int s3, int s4) {
  // if (s1 < s3) {
  //   voorMatrix[s1][s3]++;
  // } else {
  //   voorMatrix[s3][s1]++;
  // }
  // if (s2 < s4) {
  //   voorMatrix[s2][s4]++;
  // } else {
  //   voorMatrix[s4][s2]++;
  // }
  // if (s1 < s2) {
  //   tegenMatrix[s1][s2]++;
  // } else {
  //   tegenMatrix[s2][s1]++;
  // }
  // if (s1 < s4) {
  //   tegenMatrix[s1][s4]++;
  // } else {
  //   tegenMatrix[s4][s1]++;
  // }
  // if (s2 < s3) {
  //   tegenMatrix[s2][s3]++;
  // } else {
  //   tegenMatrix[s3][s2]++;
  // }
  // if (s3 < s4) {
  //   tegenMatrix[s3][s4]++;
  // } else {
  //   tegenMatrix[s4][s3]++;
  // }
  
  voorMatrix[s1][s3]++; voorMatrix[s3][s1]++;
  voorMatrix[s2][s4]++; voorMatrix[s4][s2]++;
  tegenMatrix[s1][s2]++; tegenMatrix[s2][s1]++;
  tegenMatrix[s1][s4]++; tegenMatrix[s4][s1]++;
  tegenMatrix[s2][s3]++; tegenMatrix[s3][s2]++;
  tegenMatrix[s3][s4]++; tegenMatrix[s4][s3]++;
}