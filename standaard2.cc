// Implementatie van standaard functies.

#include <iostream>
#include <cstdlib>  // voor rand
#include "standaard2.h"
using namespace std;

//*************************************************************************

bool integerInBereik2 (const char *variabele, int waarde,
                      int minWaarde, int maxWaarde)
{
  if (waarde>=minWaarde && waarde<=maxWaarde)
    return true;
  else
  { cout << variabele << "=" << waarde << ", maar moet in [" << minWaarde
         << "," << maxWaarde << "] liggen." << endl;
    return false;
  }

}  // integerInBereik

//*************************************************************************

bool integerInBereik2 (int waarde, int minWaarde, int maxWaarde)
{
  if (waarde>=minWaarde && waarde<=maxWaarde)
    return true;
  else
    return false;

}  // integerInBereik

//*************************************************************************

int randomGetal2 (int min, int max)
{ int bereik,
      r;

  bereik = max - min + 1;

  r = ((rand())%bereik) + min;
  return r;

}  // randomGetal

//*************************************************************************

double square2 (double x)
{
  return x*x;

}  // square

//*************************************************************************

int intSquare2 (int x)
{
  return x*x;

}  // square

