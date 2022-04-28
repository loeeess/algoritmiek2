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
    cout << "Te weinig spelers: aantal spelers verandert naar 4" << endl;
  } else {
    nrSpelers = MaxNrSpelers;
    cout << "Te veel spelers: aantal spelers verandert naar 20" << endl;
  }

  if (nrSpelers % 4 == 2) {
    nrSpelers--;
    cout << "Ongeldig aantal spelers: aantal spelers verlaagt naar " 
         << nrSpelers << endl;
  } else if (nrSpelers % 4 == 3) {
    nrSpelers++;
    cout << "Ongeldig aantal spelers: aantal spelers verhoogt naar " 
         << nrSpelers << endl;
  }

  schemaGrootte = 0;
  randScore = (nrSpelers - 1) / 3.0;
  minWaarde = 1000000000;
  
  for (int i = 0; i < nrSpelers; i++) {
    for (int j = 0; j < nrSpelers; j++) {
      voorMatrix[i][j] = 0;
      tegenMatrix[i][j] = 0;
      for (int z = 0; z < 4; z++) {
        scoreMatrix[i][j][z] = -1;
      }
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
  if (!integerInBereik(nrSpelers, 4, MaxNrSpelers) || nrSpelers % 4 == 2 || 
                       nrSpelers % 4 == 3) {
    cout << "Ongeldig speleraantal" << endl;
  return false;
  }

  randScore = (nrSpelers - 1) / 3.0;
  minWaarde = 1000000000;
  for (int i = 0; i < nrSpelers; i++) {
    for (int j = 0; j < nrSpelers; j++) {
      voorMatrix[i][j] = 0;
      tegenMatrix[i][j] = 0;
      for (int z = 0; z < 4; z++) {
        scoreMatrix[i][j][z] = -1;
      }
    }
  }

  // Laadt waardes uit bestand naar vector
  int speler;
  while(!fin.eof()) {
    speler = -100;
    fin >> speler;
    // Vermijden inladen trailing new lines
    if (speler != -100) {
      hulpSchema.push_back(speler);
    }
  }

  // Check of waardes een spelernummer zijn
  vector<int> ronde;
  for (int i = 0; i < hulpSchema.size(); i++) {
    cout << endl;
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
    if ((i + 1) % (hulpSchema.size() / aantalRondes) == 0) {
      ronde.clear();
    }
  }

  return true;

}  // leesInDeelschema

//*************************************************************************

void Schema::drukAfSchema(int schema[MaxGrootteSchema]) {
  for (int i = 0; i < schemaGrootte; i++) {
    int spelersPerRonde = (nrSpelers % 4 == 0) ? nrSpelers : nrSpelers - 1;
    if ((i + 1) % spelersPerRonde == 1) {
      cout << endl << "Ronde " << i / spelersPerRonde << endl;
    }
    cout << " " << schema[i];
  }
  cout << endl;
}  // drukAfSchema

//*************************************************************************

bool Schema::bepaalSchemaBT(int schema[MaxGrootteSchema], 
                            long long &aantalDeelschemas) {
  resetSchema(schema);

  return bepaalSchemaBTRecur(schema, aantalDeelschemas);
}  //  bepaalSchemaBT

//*************************************************************************

bool Schema::bepaalSchemaBTRecur(int schema[MaxGrootteSchema],
                                 long long &aantalDeelschemas) {
  if (!schemaCorrect()) {
    return false;
  } else if (schemaCompleet()) {
    return true;
  } else {
    for (int s = 0; s < nrSpelers; s++) {
      aantalDeelschemas++;
      schema[schemaGrootte] = s;
      schemaGrootte++;
      updateMatrix(schema);
      if (bepaalSchemaBTRecur(schema, aantalDeelschemas)) {
        return true;
      }
      undoMatrix(schema);
      schemaGrootte--;
    }
    return false;
  }
} // bepaalSchemaBTRecur

//*************************************************************************

bool Schema::bepaalMinSchema(int schema[MaxGrootteSchema],
                             long long &aantalDeelschemas, bool bouwWaardeOp) { 
  resetSchema();

  return bepaalMinSchemaRecur(schema, aantalDeelschemas, bouwWaardeOp);

}  // bepaalMinSchema
  
//*************************************************************************

bool Schema::bepaalMinSchemaRecur(int schema[MaxGrootteSchema]
                              long long &aantalDeelschemas, bool bouwWaardeOp) {
  if (!schemaCorrect()) {
    return false;
  } else if (schemaCompleet()) {
    float waarde = updateRondeMatrix(schema, false);
    if (waarde < minWaarde) {
      minWaarde = waarde;
      minSchema = schema;
    }
    return true;
  } else {
    for (int s = 0; s < nrSpelers; s++) {
      aantalDeelschemas++;
      schema[schemaGrootte] = s;
      schemaGrootte++;
      updateMatrix(schema);
      if (bepaalSchemaBTRecur(schema, aantalDeelschemas)) {
        return true;
      }
      undoMatrix(schema);
      schemaGrootte--;
    }
    return false;
  }
}

//*************************************************************************

void Schema::bepaalSchemaGretig(int schema[MaxGrootteSchema]) {

  leesInDeelschema(schema);

}  // bepaalSchemaGretig

//*************************************************************************

bool Schema::schemaCompleet() {
  for (int i = 0; i < nrSpelers; i++) {
    for (int j = 0; j < nrSpelers; j++) {
      if ((voorMatrix[i][j] != 1 && i != j) || 
      (tegenMatrix[i][j] != 2 && i != j)) {
        return false;
      }
    }
  }
  return true;
} // schemaCompleet

//*************************************************************************

bool Schema::schemaCorrect() {
  for (int i = 0; i < nrSpelers; i++) {
    for (int j = 0; j < nrSpelers; j++) {
      if (voorMatrix[i][j] > 1 || tegenMatrix[i][j] > 2 || 
      (i == j && (voorMatrix[i][j] != 0 || tegenMatrix[i][j] != 0))) {
        return false;
      }
    }
  }
  return true;
} // schemaCorrect

//*************************************************************************

bool Schema::inVector(vector<int> v, int value) {
  for (int i = 0; i < v.size(); i++) {
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
      if ((i + 1) % 4 == 1) {
        updateMatrix(hulpSchema[i], hulpSchema[i+1], hulpSchema[i+2], 
                     hulpSchema[i+3]);
      }
    }
  }
}  // leesInDeelschema

//*************************************************************************

void Schema::undoMatrix(int schema[MaxGrootteSchema]) {
  if (schemaGrootte % 4 == 0) {
    int s1 = schema[schemaGrootte - 4];
    int s2 = schema[schemaGrootte - 3];
    int s3 = schema[schemaGrootte - 2];
    int s4 = schema[schemaGrootte - 1];
    voorMatrix[s4][s2]--; voorMatrix[s2][s4]--;
    tegenMatrix[s4][s1]--; tegenMatrix[s1][s4]--;
    tegenMatrix[s4][s3]--; tegenMatrix[s3][s4]--;
  } else if (schemaGrootte % 4 == 2) {
    int s1 = schema[schemaGrootte - 2];
    int s2 = schema[schemaGrootte - 1];
    tegenMatrix[s1][s2]--; tegenMatrix[s2][s1]--;
  } else if (schemaGrootte % 4 == 3) {
    int s1 = schema[schemaGrootte - 3];
    int s2 = schema[schemaGrootte - 2];
    int s3 = schema[schemaGrootte - 1];
    voorMatrix[s1][s3]--; voorMatrix[s3][s1]--;
    tegenMatrix[s3][s2]--; tegenMatrix[s2][s3]--;
  }
}

//*************************************************************************

void Schema::updateMatrix(int schema[MaxGrootteSchema]) {
  if (schemaGrootte % 4 == 0) {
    int s1 = schema[schemaGrootte - 4];
    int s2 = schema[schemaGrootte - 3];
    int s3 = schema[schemaGrootte - 2];
    int s4 = schema[schemaGrootte - 1];
    voorMatrix[s4][s2]++; voorMatrix[s2][s4]++;
    tegenMatrix[s4][s1]++; tegenMatrix[s1][s4]++;
    tegenMatrix[s4][s3]++; tegenMatrix[s3][s4]++;
  } else if (schemaGrootte % 4 == 2) {
    int s1 = schema[schemaGrootte - 2];
    int s2 = schema[schemaGrootte - 1];
    tegenMatrix[s1][s2]++; tegenMatrix[s2][s1]++;
  } else if (schemaGrootte % 4 == 3) {
    int s1 = schema[schemaGrootte - 3];
    int s2 = schema[schemaGrootte - 2];
    int s3 = schema[schemaGrootte - 1];
    voorMatrix[s1][s3]++; voorMatrix[s3][s1]++;
    tegenMatrix[s3][s2]++; tegenMatrix[s2][s3]++;
  }
  // Modulo 1 means start of a new table with 1 player, so no new teammates
  // or opponents
}

//*************************************************************************

void Schema::updateMatrix(int s1, int s2, int s3, int s4) {  
  voorMatrix[s1][s3]++; voorMatrix[s3][s1]++;
  voorMatrix[s2][s4]++; voorMatrix[s4][s2]++;
  tegenMatrix[s1][s2]++; tegenMatrix[s2][s1]++;
  tegenMatrix[s1][s4]++; tegenMatrix[s4][s1]++;
  tegenMatrix[s2][s3]++; tegenMatrix[s3][s2]++;
  tegenMatrix[s3][s4]++; tegenMatrix[s4][s3]++;
} // updateMatrix

//*************************************************************************

void Schema::printMatrices() {
  cout << endl << "VoorMatrix" << endl;
  cout << "   ";
  for (int s = 0; s < nrSpelers; s++) {
    cout << s << " ";
  }
  cout << endl;
  for (int i = 0; i < nrSpelers; i++) {
    cout << i << ": ";
    for (int j = 0; j < nrSpelers; j++) {
      cout << voorMatrix[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl << "TegenMatrix" << endl;
  cout << "   ";
  for (int s = 0; s < nrSpelers; s++) {
    cout << s << " ";
  }
  cout << endl;
  for (int i = 0; i < nrSpelers; i++) {
    cout << i << ": ";
    for (int j = 0; j < nrSpelers; j++) {
      cout << tegenMatrix[i][j] << " ";
    }
    cout << endl;
  }
} // printMatrices

//*************************************************************************

void Schema::resetSchema(int schema[MaxGrootteSchema]) {
  aantalDeelschemas = 0;
  minWaarde = 1000000000;
  schemaGrootte = 0;
  for (int i = 0; i < nrSpelers; i++) {
    for (int j = 0; j < nrSpelers; j++) {
      voorMatrix[i][j] = 0;
      tegenMatrix[i][j] = 0;
      for (int z = 0; z < 4; z++) {
        scoreMatrix[i][j][z] = -1;
      }
    }
  }

  if (hulpSchema.size() != 0) {
    leesInDeelschema(schema);
    schemaGrootte = hulpSchema.size();
    // printMatrices();
  }             

  drukAfSchema(schema);
}

//*************************************************************************

float Schema::updateRondeMatrix(int schema[MaxGrootteSchema], bool undo) {
  float score = 0;
  float hulpRonde = schemaGrootte / (float) (nrSpelers - nrSpelers % 4);
  int ronde = (hulpRonde > (int) hulpRonde) ? hulpRonde++ : hulpRonde;
  int s = schema[schemaGrootte - 1];
  if (schemaGrootte % 4 > 1 || schemaGrootte % 4 == 0) {
    score += updateRondeMatrix(s, schema[schemaGrootte - 2], undo);
  }
  if (schemaGrootte % 4 == 3 || schemaGrootte % 4 == 0) {
    score += updateRondeMatrix(s, schema[schemaGrootte - 3], undo);
  }
  if (schemaGrootte % 4 == 0) {
    score += updateRondeMatrix(s, schema[schemaGrootte - 4], undo);
  }
  return score;
}

//*************************************************************************

float Schema::updateRondeMatrix(int s1, int s2, int ronde, bool undo) {
  int r = vrijInRondeMatrix(s, s2);
  rondeMatrix[s][s2][r] = (undo) ? -1 : ronde;
  rondeMatrix[s2][s][r] = (undo) ? -1 : ronde;
  if (r != 0) {
    return deelScore(rondeMatrix[s][s2][r] - rondeMatrix[s][s2][r-1]);
  }
  return 0;
}

//*************************************************************************

int Schema::vrijInRondeMatrix(int s1, int s2) {
  int i = 0;
  while (rondeMatrix[s1][s2][i] != -1 && i < 3) {
    i++;
  }
  return i;
}

//*************************************************************************

float Schema::deelScore(int x) {
  float s = randScore - x;
  return s * s;
}