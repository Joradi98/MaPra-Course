/***********************************************
*  Name       : iterativeSolve.cpp             *
*  Verwendung : Implementation von Iterationsverfahren                   *
***********************************************/

#include "matrix.h"
#include "vektor.h"
#include "unit.h"
#include <cmath>
#include <cstdlib>


Vektor jacobi(Matrix A, Vektor x0, Vektor b, double tol, unsigned int maxIter, unsigned int &benoetigteIterationen)
{
  unsigned int i, k, l;
  double sum;
  Vektor oldVektor = x0;
  Vektor newVector = x0;
  
  
  //Eingabe ueberpruefen: Auf Diagonale darf keine Null stehen!
  
  
  // Algorithmus durchfuehren bis gewuenschte Genuaigkeit oder max. Anzahl der Iterationen 
  for (i = 0; i < maxIter; i++) {
    //Iterationsschritt i
    
    for (k = 0; k < x0.Laenge(); k++) {
      //Komponente k des neuen Vektors berechnen
      
      //Summe aus der Vorschrift des Jacobi-Verfahrens berechnen
      sum = 0;
      for (l = 0; l < x0.Laenge(); l++) {
        if (k != l)
          sum += A(k,l)*oldVektor(l);
      }
      
      //Komponente des neuen Vektors nach Formel berechnen
      newVector(k) = 1.0/A(k,k) * ( b(k) - sum );
      
    }
    
    //Abbruchbedingung ueberpruefen.
    if ( fabs( (b - A*newVector).Norm2()/b.Norm2()) < tol ) {
      i++;
      break;
    }
    
    
    //Alten Vektor auf den neuen setzen, um nachste Iteration vorzubereiten
    oldVektor = newVector;

  }
  
  // Rueckgabe
  benoetigteIterationen = i;
  return newVector;
  
  
}

int main(int argc, char* argv[]) 
{
  std::cout << "Willkommen zur Aufgabe 3. Hier werden iterative Loesungsverfahren für lineare Gleichungssysteme implementiert." << std::endl;

  int i, maxIter = 0;
  unsigned int anzahlIter;
  Matrix A;
  Vektor x0;
  Vektor b;

  double tol = 0;
  
  for (i=1; i <= AnzahlBeispiele; i++) {
    Start(i, A, x0, b, tol, maxIter);

    Vektor result(x0.Laenge());
    result = jacobi(A, x0, b, tol, maxIter, anzahlIter);
    Ergebnis(result, anzahlIter, 0);
    
    //TODO: Nummer des Beispiels als Kommandozeilenparameter uebergeben
    //TODO: LGS loesen
    //TODO: Relatives Residuum in Datei schreiben, gnuplot?
    //TODO: Ergebnis aufrufen

  }
  

}

