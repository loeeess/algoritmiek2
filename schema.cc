// Implementatie van klasse Schema
// Opdracht 2 gemaakt door Lochyin Cheng (s3383040) en Loes Crama (s3335798)

#include <iostream>
#include <fstream>
#include "standaard.h"
#include "schema.h"
using namespace std;

//*************************************************************************

// Constructor
Schema::Schema() {

}  // default constructor

//*************************************************************************

// Constructor met gegeven aantal spelers
Schema::Schema(int nwNrSpelers) {
  // Check of gegeven aantal spelers geldig is. Zo niet, verander naar
  // dichtstbijzijnde valide aantal.
  // Aantal spelers moet tussen 4 en 20 liggen
  if (integerInBereik(nwNrSpelers, 4, MaxNrSpelers)) {
    nrSpelers = nwNrSpelers;
  } else if (nwNrSpelers < 4) {
    nrSpelers = 4;
    cout << "Te weinig spelers: aantal spelers verandert naar 4" << endl;
  } else {
    nrSpelers = MaxNrSpelers;
    cout << "Te veel spelers: aantal spelers verandert naar 20" << endl;
  }
  // Aantal spelers % 4 moet gelijk zijn aan 0 of 1
  if (nrSpelers % 4 == 2) {
    nrSpelers--;
    cout << "Ongeldig aantal spelers: aantal spelers verlaagt naar " 
         << nrSpelers << endl;
  } else if (nrSpelers % 4 == 3) {
    nrSpelers++;
    cout << "Ongeldig aantal spelers: aantal spelers verhoogt naar " 
         << nrSpelers << endl;
  }

  // Bereken een paar handige variabelen obv aantal spelers
  aantRondes = (nrSpelers % 4 == 0) ? nrSpelers - 1 : nrSpelers;
  // Gem. aant. rondes tussen hoe vaak 2 spelers elkaar zien
  randScore = aantRondes / 3.0; 
  cout << "randscore: " << randScore << endl;
  spelersPRonde = nrSpelers - nrSpelers % 4;
}  // constructor met parameter

//*************************************************************************

// Lees deelschema uit file genaamd invoerNaam
bool Schema::leesInDeelschema(const char* invoerNaam) {
  ifstream fin;
  int aantalRondes;

  fin.open(invoerNaam);
  // Check of bestand bestaat
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
  // Bereken enkele handige variabelen obv aantal spelers
  aantRondes = (nrSpelers % 4 == 0) ? nrSpelers - 1 : nrSpelers;
  randScore = aantRondes / 3.0;
  spelersPRonde = nrSpelers - nrSpelers % 4;

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

  return (hulpSchemaCorrect(aantalRondes)) ? true : false;
}  // leesInDeelschema

//*************************************************************************

// Print schema in terminal
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

// Bepaal een schema met backtracking en neem de eerste die je vindt
// Wrapper functie voor bepaalSchemaBTRecur
bool Schema::bepaalSchemaBT(int schema[MaxGrootteSchema], 
                            long long &aantalDeelschemas) {
  // Vul eerste ronde met 0 ... n(-1) om schemasymmetrie te voorkomen
  vulEersteRonde();
  // Reset de schema(waardes) en matrices
  aantalDeelschemas = 0;
  resetSchema(schema);

  return bepaalSchemaBTRecur(schema, aantalDeelschemas);
}  //  bepaalSchemaBT

//*************************************************************************

// Bepaal een schema met backtracking en neem de eerste die je vindt
bool Schema::bepaalSchemaBTRecur(int schema[MaxGrootteSchema],
                                 long long &aantalDeelschemas) {
  // Stop als (deel)schema incorrect is en backtrack
  if (!schemaCorrect()) {
    return false;
  } else if (schemaCompleet()) {
    return true;
  } else {
    // Wanneer een nieuwe ronde begint, zijn alle spelers weer vrij
    if (schemaGrootte % spelersPRonde == 0) {
      nieuweRondeSpelers();
    }
    // Probeer alle spelers uit
    for (int s = 0; s < nrSpelers; s++) {
      // Ga alleen verder met een speler als deze niet leidt tot symmetrie
      // en als deze speler nog niet geplaatst is
      if (!symmetrie(s, schema) && spelerVrij(s)) {
        // Voeg speler toe aan schema en houdt dit bij in de matrices
        aantalDeelschemas++;
        schema[schemaGrootte] = s;
        schemaGrootte++;
        updateMatrix(schema);
        if (bepaalSchemaBTRecur(schema, aantalDeelschemas)) {
          return true;
        }
        // Haal speler weer uit schema en houdt dit bij in de matrices
        undoMatrix(schema);
        schemaGrootte--;
        maakSpelerVrij(s);
      }
    }
    return false;
  }
} // bepaalSchemaBTRecur

//*************************************************************************

// Bepaal schema met backtracking en neem schema met laagste waarde
// Wrapper functie voor bepaalMinSchemaBTRecur
bool Schema::bepaalMinSchema(int schema[MaxGrootteSchema],
                             long long &aantalDeelschemas, bool bouwWaardeOp) { 
  // Vul eerste ronde met 0 ... n(-1) om schemasymmetrie te voorkomen
  vulEersteRonde();
  // Reset de schema(waardes) en matrices
  aantalDeelschemas = 0;
  resetSchema(schema);
  waarde = schemaWaarde(schema);

  bool gelukt = bepaalMinSchemaRecur(schema, aantalDeelschemas, bouwWaardeOp);
  // Sla definitieve waardes op
  schemaGrootte = finalGrootte;
  kopieerSchema(schema, minSchema);
  cout << "Minwaarde: " << minWaarde << endl;
  return gelukt;

}  // bepaalMinSchema
  
//*************************************************************************

// Bepaal schema met backtracking en neem schema met laagste waarde
// Als bouwWaardeOp true is, backtrack ook als deelschema een grotere waarde
// heeft als minwaarde
bool Schema::bepaalMinSchemaRecur(int schema[MaxGrootteSchema],
                              long long &aantalDeelschemas, bool bouwWaardeOp) {
  if (!schemaCorrect()) {
    return false;
  } else if (schemaCompleet()) {
    if (!bouwWaardeOp) {
      waarde = schemaWaarde(schema);
    }
    if (waarde < minWaarde) {
      minWaarde = waarde;
      kopieerSchema(minSchema, schema);
      finalGrootte = schemaGrootte;
      // cout << "Nieuwe minWaarde: " << minWaarde << endl;
    }

    return true;
  } else {
    bool schemaGevonden = false;
    if (schemaGrootte % spelersPRonde == 0) {
      nieuweRondeSpelers();
    }
    for (int s = 0; s < nrSpelers; s++) {
      if (!symmetrie(s, schema) && spelerVrij(s)) {
        aantalDeelschemas++;
        schema[schemaGrootte] = s;
        schemaGrootte++;
        updateMatrix(schema);
        double deelWaarde;
        if (bouwWaardeOp) {
          deelWaarde = updateRondeMatrix(schema, false);
          waarde += deelWaarde;
        }
        if ((!bouwWaardeOp || waarde < minWaarde) && 
        bepaalMinSchemaRecur(schema, aantalDeelschemas, bouwWaardeOp)) { 
          schemaGevonden = true;
        }
        undoMatrix(schema);
        if (bouwWaardeOp) {
          waarde -= deelWaarde;
          updateRondeMatrix(schema, true);
        }
        schemaGrootte--;
        maakSpelerVrij(s);
      }
    }
    return schemaGevonden;
  }
}

//*************************************************************************

// Bepaal schema zonder terug te gaan
// Neem telkens een vrije speler die een zo laag mogelijke waarde oplevert
// en als het kan niet direct tot een invalide schema leidt.
void Schema::bepaalSchemaGretig(int schema[MaxGrootteSchema]) {
  // Reset schema(waardes) en matrices
  resetSchema(schema);
  // Houdt bij welke spelers nog niet gebruikt zijn in een ronde
  vector<int> valideSpelers; 
  while (schemaGrootte < aantRondes * spelersPRonde) {
    if (schemaGrootte % spelersPRonde == 0) {
      valideSpelers.clear();
      for (int s = 0; s < nrSpelers; s++) {
        valideSpelers.push_back(s);
      }
    }
    // Houdt beste spelers en waardes bij (ook voor incorrecte schemas)
    double minDeelWaarde = 1000000000; double minFouteDeelWaarde = 100000000;
    int besteSpeler = -1; int besteFouteSpeler = -1;
    bool correcteSpelerBestaat = false;
    // Kies alleen uit valide spelers
    for (int s : valideSpelers) {
      schema[schemaGrootte] = s;
      schemaGrootte++;
      updateMatrix(schema);
      int deelWaarde = updateRondeMatrix(schema, false);
      if (schemaCorrect()) {
        correcteSpelerBestaat = true;
        if (deelWaarde < minDeelWaarde) {
          minDeelWaarde = deelWaarde;
          besteSpeler = s;
        }
      } else {
        if (deelWaarde < minFouteDeelWaarde) {
          minFouteDeelWaarde = deelWaarde;
          besteFouteSpeler = s;
        }
      }
      undoMatrix(schema);
      updateRondeMatrix(schema, true);
      schemaGrootte--;
    }
    // Voeg beste speler toe
    schema[schemaGrootte] = (correcteSpelerBestaat) ? besteSpeler : besteFouteSpeler;
    schemaGrootte++;
    updateMatrix(schema);
    updateRondeMatrix(schema, false);
    // Haal speler weg uit lijst met vrije spelers
    for (int i = 0; (unsigned) i < valideSpelers.size(); i++) {
      if ((correcteSpelerBestaat && valideSpelers[i] == besteSpeler) || 
      (!correcteSpelerBestaat && valideSpelers[i] == besteFouteSpeler)) {
        valideSpelers.erase(valideSpelers.begin() + i);
      }
    }
  }
}  // bepaalSchemaGretig

//*************************************************************************

// Check of huidig schema een compleet schema is mbv de matrices
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

// Check of huidig schema nog een correct schema is mbv de matrices
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

// Check of een vector v een bepaalde waarde bevat
bool Schema::inVector(vector<int> v, int value) {
  for (int i = 0; (unsigned) i < v.size(); i++) {
    if (v[i] == value) {
      return true;
    }
  }
  return false;
}  // inVector

//*************************************************************************

// Kopieer deelschema naar gegeven schema en update de voor en tegen matrices
void Schema::leesInDeelschema(int schema[MaxGrootteSchema]) {
  if (hulpSchema.size() > 0) {
    for (int i = 0; (unsigned) i < hulpSchema.size(); i++) {
      schema[i] = hulpSchema[i];
      if ((i + 1) % 4 == 1) {
        updateMatrix(hulpSchema[i], hulpSchema[i+1], hulpSchema[i+2], 
                     hulpSchema[i+3]);
      }
    }
  }
}  // leesInDeelschema

//*************************************************************************

// Houdt in voor / tegen matrix bij dat de laatste schemawaarde wordt verwijderd
void Schema::undoMatrix(int schema[MaxGrootteSchema]) {
  // Tafelgrootte 4
  if (schemaGrootte % 4 == 0) {
    int s1 = schema[schemaGrootte - 4];
    int s2 = schema[schemaGrootte - 3];
    int s3 = schema[schemaGrootte - 2];
    int s4 = schema[schemaGrootte - 1];
    voorMatrix[s4][s2]--; voorMatrix[s2][s4]--;
    tegenMatrix[s4][s1]--; tegenMatrix[s1][s4]--;
    tegenMatrix[s4][s3]--; tegenMatrix[s3][s4]--;
  // Tafelgrootte 2
  } else if (schemaGrootte % 4 == 2) {
    int s1 = schema[schemaGrootte - 2];
    int s2 = schema[schemaGrootte - 1];
    tegenMatrix[s1][s2]--; tegenMatrix[s2][s1]--;
  // Tavelgrootte 3
  } else if (schemaGrootte % 4 == 3) {
    int s1 = schema[schemaGrootte - 3];
    int s2 = schema[schemaGrootte - 2];
    int s3 = schema[schemaGrootte - 1];
    voorMatrix[s1][s3]--; voorMatrix[s3][s1]--;
    tegenMatrix[s3][s2]--; tegenMatrix[s2][s3]--;
  }
}

//*************************************************************************

// Houdt in voor / tegen matrix bij dat een nieuwe schemawaarde is toegevoegd
void Schema::updateMatrix(int schema[MaxGrootteSchema]) {
  // Tafelgrootte 4
  if (schemaGrootte % 4 == 0) {
    int s1 = schema[schemaGrootte - 4];
    int s2 = schema[schemaGrootte - 3];
    int s3 = schema[schemaGrootte - 2];
    int s4 = schema[schemaGrootte - 1];
    voorMatrix[s4][s2]++; voorMatrix[s2][s4]++;
    tegenMatrix[s4][s1]++; tegenMatrix[s1][s4]++;
    tegenMatrix[s4][s3]++; tegenMatrix[s3][s4]++;
  // Tafelgrootte 2
  } else if (schemaGrootte % 4 == 2) {
    int s1 = schema[schemaGrootte - 2];
    int s2 = schema[schemaGrootte - 1];
    tegenMatrix[s1][s2]++; tegenMatrix[s2][s1]++;
  // Tafelgrootte 3
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

// Houdt in voor / tegen matrix bij dat een nieuwe tafel is toegevoegd
void Schema::updateMatrix(int s1, int s2, int s3, int s4) {
  voorMatrix[s1][s3]++; voorMatrix[s3][s1]++;
  voorMatrix[s2][s4]++; voorMatrix[s4][s2]++;
  tegenMatrix[s1][s2]++; tegenMatrix[s2][s1]++;
  tegenMatrix[s1][s4]++; tegenMatrix[s4][s1]++;
  tegenMatrix[s2][s3]++; tegenMatrix[s3][s2]++;
  tegenMatrix[s3][s4]++; tegenMatrix[s4][s3]++;
} // updateMatrix

//*************************************************************************

// Print de voor / tegen matrix in de terminal
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

// Print de ronde matrix in de terminal
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

// Reset schema(waardes) en matrices
void Schema::resetSchema(int schema[MaxGrootteSchema]) {
  minWaarde = 1000000000;
  waarde = 0;
  schemaGrootte = 0;
  for (int i = 0; i < nrSpelers; i++) {
    for (int j = 0; j < nrSpelers; j++) {
      voorMatrix[i][j] = 0;
      tegenMatrix[i][j] = 0;
      for (int z = 0; z < MaxNrSpelers; z++) {
        rondeMatrix[i][j][z] = -1;
      }
    }
  }

  // Lees deelschema opnieuw in als die bestaat
  if (hulpSchema.size() != 0) {
    leesInDeelschema(schema);
    schemaGrootte = hulpSchema.size();
  }             
}

//*************************************************************************

// Houdt in rondeMatrix bij in welke rondes spelers elkaar hebben gezien
// Bereken deelscore / -waarde van het toevoegen van een schemawaarde
// Nog niet bekend welke spelers elkaar hebben gezien in welke ronde
double Schema::updateRondeMatrix(int schema[MaxGrootteSchema], bool undo) {
  double score = 0; // Houdt deelscore bij
  // Bereken huidige ronde
  int ronde = (schemaGrootte - schemaGrootte % spelersPRonde) / spelersPRonde; 
  if (schemaGrootte % spelersPRonde == 0) {
    ronde--;
  }

  int s = schema[schemaGrootte - 1]; // Nieuwste speler
  // Update matrix obv tafelgrootte
  if (schemaGrootte % 4 > 1 || schemaGrootte % 4 == 0) {
    score += updateRondeMatrix(s, schema[schemaGrootte - 2], ronde, undo);
  }
  if (schemaGrootte % 4 == 3 || schemaGrootte % 4 == 0) {
    score += updateRondeMatrix(s, schema[schemaGrootte - 3], ronde, undo);
  }
  if (schemaGrootte % 4 == 0) {
    score += updateRondeMatrix(s, schema[schemaGrootte - 4], ronde, undo);
  }
  return score;
}

//*************************************************************************

// Houdt in rondeMatrix bij in welke rondes spelers elkaar hebben gezien
// Bereken deelscore / -waarde van het toevoegen van een schemawaarde
// Bereken welke spelers elkaar hebben gezien in welke ronde
double Schema::updateRondeMatrix(int s1, int s2, int ronde, bool undo) {
  int r = vrijInRondeMatrix(s1, s2); // Krijg eerste vrije plek in de matrix
  // Zet rondenummer als waarde, -1 als er een waarde uit de schema is gehaald
  if (!undo) {
    rondeMatrix[s1][s2][r] = ronde;
    rondeMatrix[s2][s1][r] = ronde;
  } else {
    rondeMatrix[s1][s2][r-1] = -1;
    rondeMatrix[s2][s1][r-1] = -1;    
  }
  // Bereken deelscore als spelers elkaar meerdere keren hebben gezien
  if (!undo && r != 0) {
    return deelScore(rondeMatrix[s1][s2][r] - rondeMatrix[s1][s2][r-1]);
  }
  return 0;
}

//*************************************************************************

// Geef eerste vrije plek in de derde dimensie van de ronde matrix
// maw: geef hoevaak de geven twee spelers elkaar al hebben gezien
int Schema::vrijInRondeMatrix(int s1, int s2) {
  int i = 0;
  while (rondeMatrix[s1][s2][i] != -1 && i < MaxNrSpelers) {
    i++;
  }
  return i;
}

//*************************************************************************

// Bereken de deelscore obv x (aant rondes tussen 2x dat 2 spelers elkaar zien)
double Schema::deelScore(int x) {
  double s = randScore - x;
  return s * s;
}

//*************************************************************************


// Kopieer schema a2 naar a1
void Schema::kopieerSchema(int a1[MaxGrootteSchema], int a2[MaxGrootteSchema]) {
  for (int i = 0; i < schemaGrootte; i++) {
    a1[i] = a2[i];
  }
}

//*************************************************************************

// Bereken gehele schema waarde
double Schema::schemaWaarde(int schema[MaxGrootteSchema]) {
  // Reset rondeMatrix
  for (int i = 0; i < nrSpelers; i++) {
    for (int j = 0; j < nrSpelers; j++) {
      for (int z = 0; z < MaxNrSpelers; z++) {
        rondeMatrix[i][j][z] = -1;
      }
    }
  }

  double deelwaarde = 0;
  int grootte = schemaGrootte; // onthoudt originele schemagrootte
  // Bouw schema en waarde 1 voor 1 op
  for (int i = 2; i <= grootte; i++) {
    schemaGrootte = i;
    deelwaarde += updateRondeMatrix(schema, false);
  }
  return deelwaarde;
}

//*************************************************************************

// Voeg een nieuwe ronde (vector) toe met alle mogelijke / vrije spelers
void Schema::nieuweRondeSpelers() {
  vector<int> v;
  for (int i = 0; i < nrSpelers; i++) {
    v.push_back(i);
  }
  vrijeSpelers.push_back(v);
}

//*************************************************************************

// Check of speler een vrije speler is en dus nog niet is gebruikt in deze ronde
bool Schema::spelerVrij(int s) {
  int i = 0;
  int r = vrijeSpelers.size() - 1;
  while ((unsigned) i < vrijeSpelers[r].size() && vrijeSpelers[r][i] != s) {
    i++;
  }
  if ((unsigned) i < vrijeSpelers[r].size()) {
    vrijeSpelers[r].erase(vrijeSpelers[r].begin() + i);
    return true;
  }
  return false;
}

//*************************************************************************

// Voeg speler s toe aan lijst met vrije spelers
void Schema::maakSpelerVrij(int s) {
  int r = vrijeSpelers.size() - 1;
  if (r >= 0) {
    // Check of je terug moet gaan naar vorige ronde
    if (schemaGrootte % spelersPRonde == spelersPRonde - 1 && 
    vrijeSpelers[r].size() > 1) {
      vrijeSpelers.pop_back();
      r = vrijeSpelers.size() - 1;
    }
    vrijeSpelers[r].push_back(s);
  }
}

//*************************************************************************

// Vul eerste ronde met spelers 0 ... n(-1) om schemasymmetrie te voorkomen
// Alleen als er geen deelschema is gegeven
void Schema::vulEersteRonde() {
  if (hulpSchema.size() == 0) {
    for (int s = 0; s < spelersPRonde; s++) {
      hulpSchema.push_back(s);
    }
    schemaGrootte = hulpSchema.size();
  }
}

//*************************************************************************

// Check of je een symmetrisch schema aan het bouwen bent
bool Schema::symmetrie(int s, int schema[MaxGrootteSchema]) {
  // Check tafel symmetrie: s1 < s3, s2 < s4, s1 < s2
  if (schemaGrootte % 4 == 1 && schema[schemaGrootte - 1] >= s) {
    return true;
  }
  if (schemaGrootte % 4 >= 2 && schema[schemaGrootte - 2] >= s) {
    return true;
  }

  // Check ronde symmetrie: 1e speler van een tafel moet groter zijn dan de 
  // eerste speler van de vorige tafel. Eerste speler van een ronde kan 
  // 0 zijn of 1 als 0 moet wachten
  int spelerInRonde = schemaGrootte % spelersPRonde;
  if (spelerInRonde % 4 == 0 && !isGroterDanVorigeTafel(s, schema)) {
    return true;
  }

  // Check hele ronde symmetrie: 1e speler van deze ronde mag niet kleiner zijn
  // dan 1e speler vorige ronde
  if (schemaGrootte - (signed) hulpSchema.size() >= spelersPRonde && 
  schemaGrootte % spelersPRonde == 0 &&
  schema[schemaGrootte - spelersPRonde] > s) {
    return true;
  }

  return false;
}

//*************************************************************************

// Check of eerste speler van een tafel groter is dan de eerste speler van
// de vorige tafel
bool Schema::isGroterDanVorigeTafel(int s, int schema[MaxGrootteSchema]) {
  // Eerste speler van de ronde kan alleen 0 zijn als er geen spelers wachten
  if (nrSpelers % 4 == 0 && schemaGrootte % spelersPRonde == 0 && s == 0) {
    return true;
  }
  // Eerste speler van de ronde kan 0 of 1 zijn als er een speler wacht
  if (nrSpelers % 4 == 1 && schemaGrootte % spelersPRonde == 0 && s <= 1) {
    return true;
  }
  // Eerste speler van de tafel moet groter zijn dan eerste speler vorige tafel
  if (schemaGrootte % spelersPRonde >= 4 && s > schema[schemaGrootte - 4]) {
    return true;
  }

  return false;
}

//*************************************************************************

// Check of waardes in hulp- / deelschema correct zijn
bool Schema::hulpSchemaCorrect(int aantalRondes) {
  vector<int> ronde; // Houdt bij welke spelers al zijn geweest in een ronde
  for (int i = 0; (unsigned) i < hulpSchema.size(); i++) {

    // Check of waardes een spelernummer zijn
    if (!integerInBereik(hulpSchema[i], 0, nrSpelers - 1)) {
      cout << "Error: spelernummer buiten bereik" << endl;
      hulpSchema.clear();
      return false;
    }

    // Check of iedere speler max 1x is toegewezen
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
}

//*************************************************************************