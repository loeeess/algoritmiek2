// Defintie van standaard functies.

#ifndef Standaard2HVar  // om te voorkomen dat dit .h bestand meerdere keren
#define Standaard2HVar  // wordt ge-include 

// Controleer of variabele met naam `variabele' een waarde `waarde' heeft
// die tussen (inclusief) minWaarde en maxWaarde in ligt.
// Zo nee, geef een passende foutmelding.
//
// Voorbeeld van aanroep:
//   if (integerInBereik ("teller", teller, 0, 1000))
//     ...
bool integerInBereik2 (const char *variabele, int waarde,
                      int minWaarde, int maxWaarde);

// Controleer of waarde `waarde' tussen (inclusief) minWaarde en maxWaarde
// in ligt.
// Geef GEEN foutmelding als het niet zo is.
bool integerInBereik2 (int waarde, int minWaarde, int maxWaarde);

// Genereer een random geheel getal r waarvoor  min <= r <= max.
// Pre: min <= max;
int randomGetal2 (int min, int max);

// Bereken kwadraat van x
double square2 (double x);

// Bereken kwadraat van x
int intSquare2 (int x);

#endif

