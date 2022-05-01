// Implementatie van klasse Schema
// Opdracht 2 gemaakt door Lochyin Cheng (s3383040) en Loes Crama (s3335798)

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
  aantRondes = (nrSpelers % 4 == 0) ? nrSpelers - 1 : nrSpelers;
  randScore = aantRondes / 3.0;
  minWaarde = 1000000000;
  
  for (int i = 0; i < nrSpelers; i++) {
    for (int j = 0; j < nrSpelers; j++) {
      voorMatrix[i][j] = 0;
      tegenMatrix[i][j] = 0;
      for (int z = 0; z < 4; z++) {
        rondeMatrix[i][j][z] = -1;
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

  aantRondes = (nrSpelers % 4 == 0) ? nrSpelers - 1 : nrSpelers;
  randScore = aantRondes / 3.0;
  minWaarde = 1000000000;
  for (int i = 0; i < nrSpelers; i++) {
    for (int j = 0; j < nrSpelers; j++) {
      voorMatrix[i][j] = 0;
      tegenMatrix[i][j] = 0;
      for (int z = 0; z < 4; z++) {
        rondeMatrix[i][j][z] = -1;
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
    } else if ((i + 1) % 4 == 1) {
      cout << " |";
    }
    cout << " " << schema[i];
  }
  cout << endl << endl;
}  // drukAfSchema

//*************************************************************************

bool Schema::bepaalSchemaBT(int schema[MaxGrootteSchema], 
                            long long &aantalDeelschemas) {
  aantalDeelschemas = 0;
  resetSchema(schema);

  return bepaalSchemaBTRecur(schema, aantalDeelschemas);
}  //  bepaalSchemaBT

//*************************************************************************

bool Schema::bepaalSchemaBTRecur(int schema[MaxGrootteSchema],
                                 long long &aantalDeelschemas) {
  if (!schemaCorrect()) {
    cout << "incorrect schema" << endl;
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
  aantalDeelschemas = 0;
  resetSchema(schema);
  waarde = schemaWaarde(schema);
  printRondeMatrix();
  printMatrices();
  bool gelukt = bepaalMinSchemaRecur(schema, aantalDeelschemas, bouwWaardeOp);
  schemaGrootte = finalGrootte;
  kopieerSchema(schema, minSchema);
  cout << "Minwaarde: " << minWaarde << endl;
  return gelukt;

}  // bepaalMinSchema
  
//*************************************************************************

bool Schema::bepaalMinSchemaRecur(int schema[MaxGrootteSchema],
                              long long &aantalDeelschemas, bool bouwWaardeOp) {
  cout << "Enter recursie" << endl;
  cout << "min waarde: " << minWaarde << endl;
  cout << "waarde: " << waarde << endl;
  if (!schemaCorrect()) {
    cout << "Schema niet correct" << endl;
    return false;
  } else if (schemaCompleet()) {
    if (!bouwWaardeOp) {
      waarde = schemaWaarde(schema);
      cout << "Waarde na schemaWaarde: " << waarde << endl;
    }
    if (waarde < minWaarde) {
      minWaarde = waarde;
      kopieerSchema(minSchema, schema);
      finalGrootte = schemaGrootte;
      drukAfSchema(minSchema);
      cout << "Nieuwe minWaarde: " << minWaarde << endl;
    }
    return true;
  } else {
    bool schemaGevonden = false;
    for (int s = 0; s < nrSpelers; s++) {
      cout << "speler: " << s << endl;
      aantalDeelschemas++;
      schema[schemaGrootte] = s;
      schemaGrootte++;
      updateMatrix(schema);
      float deelWaarde;
      if (bouwWaardeOp) {
        printRondeMatrix();
        deelWaarde = updateRondeMatrix(schema, false);
        printRondeMatrix();
        cout << "Deelwaarde+: " << deelWaarde << endl;
        waarde += deelWaarde;
      }
      if ((!bouwWaardeOp || waarde < minWaarde) && 
      bepaalMinSchemaRecur(schema, aantalDeelschemas, bouwWaardeOp)) { 
        cout << "Recur called" << endl;
        schemaGevonden = true;
      }
      undoMatrix(schema);
      if (bouwWaardeOp) {
        waarde -= deelWaarde;
        int x = updateRondeMatrix(schema, true);
        cout << "Deelwaarde-: " << x << endl;
        printRondeMatrix();
      }
      schemaGrootte--;
    }
    return schemaGevonden;
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
  // Modulo 1 betekent start van een nieuwe tafel met 1 speler, 
  // dus geen nieuwe teams of tegenstanders
}

//*************************************************************************

void Schema::updateMatrix(int s1, int s2, int s3, int s4) {
  cout << s1 << s2 << s3 << s4 << endl;
  printMatrices();
  voorMatrix[s1][s3]++; voorMatrix[s3][s1]++;
  voorMatrix[s2][s4]++; voorMatrix[s4][s2]++;
  tegenMatrix[s1][s2]++; tegenMatrix[s2][s1]++;
  tegenMatrix[s1][s4]++; tegenMatrix[s4][s1]++;
  tegenMatrix[s2][s3]++; tegenMatrix[s3][s2]++;
  tegenMatrix[s3][s4]++; tegenMatrix[s4][s3]++;
  printMatrices();
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

void Schema::printRondeMatrix() {
  cout << endl << "Ronde Matrix" << endl;
  cout << "   ";
  for (int s = 0; s < nrSpelers; s++) {
    cout << s << " ";
  }
  cout << endl;
  for (int i = 0; i < nrSpelers; i++) {
    cout << i << ": ";
    for (int j = 0; j < nrSpelers; j++) {
      for (int k = 0; k < 4; k++) {
        cout << rondeMatrix[i][j][k] << " ";
      }
      cout << "| ";
    }
    cout << endl;
  }
}

//*************************************************************************

void Schema::resetSchema(int schema[MaxGrootteSchema]) {
  minWaarde = 1000000000;
  waarde = 0;
  schemaGrootte = 0;
  for (int i = 0; i < nrSpelers; i++) {
    for (int j = 0; j < nrSpelers; j++) {
      voorMatrix[i][j] = 0;
      tegenMatrix[i][j] = 0;
      for (int z = 0; z < 4; z++) {
        rondeMatrix[i][j][z] = -1;
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
  cout << "In Update Ronde Matrix" << endl;
  cout << "Schemagrootte: " << schemaGrootte << endl;
  float score = 0;
  // Bereken ronde
  int ronde = (schemaGrootte - schemaGrootte % 4) / 4;
  if (schemaGrootte % 4 == 0) {
    ronde--;
  }
  cout << "Ronde: " << ronde << endl;

  int s = schema[schemaGrootte - 1];
  if (schemaGrootte % 4 > 1 || schemaGrootte % 4 == 0) {
    cout << "1+ players" << endl;
    score += updateRondeMatrix(s, schema[schemaGrootte - 2], ronde, undo);
    cout << "Score: " << score << endl;
  }
  if (schemaGrootte % 4 == 3 || schemaGrootte % 4 == 0) {
    cout << "2+ players" << endl;
    score += updateRondeMatrix(s, schema[schemaGrootte - 3], ronde, undo);
    cout << "Score: " << score << endl;
  }
  if (schemaGrootte % 4 == 0) {
    cout << "3+ players" << endl;
    score += updateRondeMatrix(s, schema[schemaGrootte - 4], ronde, undo);
    cout << "Score: " << score << endl;
  }
  return score;
}

//*************************************************************************

float Schema::updateRondeMatrix(int s1, int s2, int ronde, bool undo) {
  int r = vrijInRondeMatrix(s1, s2);
  cout << "Vrije ronde in matrix: " << r << endl;
  if (!undo) {
    rondeMatrix[s1][s2][r] = ronde;
    rondeMatrix[s2][s1][r] = ronde;
  } else {
    rondeMatrix[s1][s2][r-1] = -1;
    rondeMatrix[s2][s1][r-1] = -1;    
  }
  if (!undo && r != 0) {
    return deelScore(rondeMatrix[s1][s2][r] - rondeMatrix[s1][s2][r-1]);
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

//*************************************************************************

void Schema::kopieerSchema(int a1[MaxGrootteSchema], int a2[MaxGrootteSchema]) {
  for (int i = 0; i < schemaGrootte; i++) {
    a1[i] = a2[i];
  }
}

//*************************************************************************

float Schema::schemaWaarde(int schema[MaxGrootteSchema]) {
  float deelwaarde = 0;
  int grootte = schemaGrootte;
  for (int i = 2; i <= grootte; i++) {
    schemaGrootte = i;
   deelwaarde += updateRondeMatrix(schema, false);
  }
  return deelwaarde;
}