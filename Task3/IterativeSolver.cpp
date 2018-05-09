/***********************************************
*  Name       : iterativeSolve.cpp             *
*  Verwendung : Implementation von Iterationsverfahren                   *
***********************************************/

#include "matrix.h"
#include "vektor.h"
#include "unit.h"
#include <cmath>
#include <cstdlib>


int main(int argc, char* argv[]) 
{
  std::cout << "Willkommen zur Aufgabe 3. Hier werden iterative Loesungsverfahren für lineare Gleichungssysteme implementiert." << std::endl;

  int i, maxIter = 0;
  Matrix A;
  Vektor x0;
  Vektor b;

  double tol = 0;
  
  for (i=1; i <= AnzahlBeispiele; i++) {
    Start(i, A, x0, b, tol, maxIter);

    //TODO: Nummer des Beispiels als Kommandozeilenparameter uebergeben
    //TODO: LGS loesen
    //TODO: Relatives Residuum in Datei schreiben, gnuplot?
    //TODO: Ergebnis aufrufen

  }
  

}

