/*******************************************************
*  Name       : IterativeSolver.cpp                    *
*  Verwendung : Implementation von Iterationsverfahren *
********************************************************/

#include "matrix.h"
#include "vektor.h"
#include "unit.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>


/*
Helper function: Appends to file
*/
void clearFile(std::string fileName) {
  //std::ifstream ifs1 ("strings.txt", std::ifstream::in);
  std::ofstream myfile;
  myfile.open (fileName, std::ofstream::out | std::ofstream::trunc);
  myfile.close();

}


/*
Helper function: Appends to file
*/
void append(std::string fileName, Vektor x) {
  //std::ifstream ifs1 ("strings.txt", std::ifstream::in);
  std::ofstream myfile;
  myfile.open (fileName, std::ofstream::out | std::ofstream::app);
  myfile << x << std::endl;
  myfile.close();
}


Vektor jacobi(Matrix A, Vektor x0, Vektor b, double tol, unsigned int maxIter, unsigned int &benoetigteIterationen)
{
  unsigned int i, k, l;
  double sum;
  Vektor oldVektor = x0;
  Vektor newVector = x0;
  Vektor residuum(maxIter); //Residuum als Vektor realisieren.
  std::string logFile = "jacobi.txt";
  
  // Algorithmus durchfuehren bis gewuenschte Genuaigkeit oder max. Anzahl der Iterationen 
  for (i = 0; i < maxIter; i++) {
    //Iterationsschritt i
    
    //Abbruchbedingung ueberpruefen.
    residuum(i) = (b - A*newVector).Norm2()/b.Norm2();
    if ( residuum(i) < tol ) {
      break;
    }
    
    append(logFile, oldVektor);

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
    
    
    
    //Alten Vektor auf den neuen setzen, um nachste Iteration vorzubereiten
    oldVektor = newVector;
  }
  
  // Rueckgabe
  clearFile(logFile);
  append(logFile, residuum);
  benoetigteIterationen = i;
  return newVector;
}


Vektor gauss_seidel(Matrix A, Vektor x0, Vektor b, double tol, unsigned int maxIter, unsigned int &benoetigteIterationen)
{
  unsigned int i, k, l;
  double sum1, sum2;
  Vektor oldVektor = x0;
  Vektor newVector = x0;
  Vektor residuum(maxIter); //Residuum als Vektor realisieren.
  std::string logFile = "gauss_seidel.txt";


  
  // Algorithmus durchfuehren bis gewuenschte Genuaigkeit oder max. Anzahl der Iterationen 
  for (i = 0; i < maxIter; i++) {
    //Iterationsschritt i
    
    //Abbruchbedingung ueberpruefen.
    residuum(i) = (b - A*newVector).Norm2()/b.Norm2();
    if ( residuum(i) < tol ) {
      break;
    }
    
    for (k = 0; k < x0.Laenge(); k++) {
      //Komponente k des neuen Vektors berechnen
      
      //Linke Summe aus der Vorschrift des GS-Verfahrens berechnen
      sum1 = 0;
      for (l = 0; l +1 <= k; l++) {
        sum1 += A(k,l)*newVector(l);
      }
      
      //Rechte Summe aus der Vorschrift des GS-Verfahrens berechnen
      sum2 = 0;
      for (l = k+1; l < x0.Laenge(); l++) {
        sum2 += A(k,l)*oldVektor(l);
      }
      
      //Komponente des neuen Vektors nach Formel berechnen
      newVector(k) = 1.0/A(k,k) * ( b(k) - sum1 - sum2 );
    }
    
  
    
    //Alten Vektor auf den neuen setzen, um nachste Iteration vorzubereiten
    oldVektor = newVector;
  }
  
  // Rueckgabe
  clearFile(logFile);
  append(logFile, residuum);
  benoetigteIterationen = i;
  return newVector;
}



Vektor conjugate_gradients(Matrix A, Vektor x0, Vektor b, double tol, unsigned int maxIter, unsigned int &benoetigteIterationen)
{
  unsigned int i;
  double alpha, gamma_old, gammma_new;
  Vektor x_old = x0;
  Vektor x_new = x0;
  Vektor q(x0.Laenge()), p_old(x0.Laenge()), p_new(x0.Laenge()), r_old(x0.Laenge()), r_new(x0.Laenge());
  Vektor residuum(maxIter); //Residuum als Vektor realisieren.
  std::string logFile = "conjugate_gradients.txt";

  
  //Initialisierung
  p_old = b - A*x0;
  r_old = p_old;
  
  gamma_old = dot(r_old, r_old);
  
  // Algorithmus durchfuehren bis gewuenschte Genuaigkeit oder max. Anzahl der Iterationen 
  for (i = 0; i < maxIter; i++) {
    //Iterationsschritt i

    //Abbruchbedingung ueberpruefen.
    residuum(i) = (r_old).Norm2()/b.Norm2();
    if ( residuum(i) < tol ) {
      break;
    }

    q = A * p_old;
    alpha = gamma_old /dot(q,p_old) ;
    x_new = x_old + alpha*p_old;
    r_new = r_old - alpha*q;
    gammma_new = dot(r_new, r_new);
    p_new = r_new + (gammma_new/gamma_old) * p_old;
        
    //VNaechsten Schritt vorbereiten
    x_old = x_new;
    gamma_old = gammma_new;
    p_old = p_new;
    r_old = r_new;
  }
  
  // Rueckgabe
  clearFile(logFile);
  append(logFile, residuum);
  benoetigteIterationen = i;
  return x_new;
  
}

/*
Helper method: Determine if a string displays an integer
*/
bool isInteger(std::string s){
  using namespace std;
  for (unsigned int i = 0; i < s.size(); i++){
    if(!isdigit(s[i])) return false;
  }
  return true;
}


int main(int argc, char* argv[]) 
{
  std::cout << "Willkommen zur Aufgabe 3. Hier werden iterative Loesungsverfahren für lineare Gleichungssysteme implementiert." << std::endl;
  std::cout << "Welches Beispiel moechtest du testen? ";

  
  std::string command;    
  std::cin >> command;

  if(isInteger(command)){
    
    int i = stoi(command); //Stoi avaiable in c++11
    int maxIter = 0;
    unsigned int anzahlIter;
    Matrix A;
    Vektor x0;
    Vektor b;
    double tol = 0;


    Start(i, A, x0, b, tol, maxIter);
    Vektor result(x0.Laenge());
    result = jacobi(A, x0, b, tol, maxIter, anzahlIter);
    Ergebnis(result, anzahlIter, 0); // Jacobi entspricht 0
    
    Start(i, A, x0, b, tol, maxIter);
    result = gauss_seidel(A, x0, b, tol, maxIter, anzahlIter);
    Ergebnis(result, anzahlIter, 1); // GaussSeidel entspricht 1 

    Start(i, A, x0, b, tol, maxIter);
    result = conjugate_gradients(A, x0, b, tol, maxIter, anzahlIter);
    Ergebnis(result, anzahlIter, 2); // CG entspricht 2
     
    
  } else {
    std::cout << "Das ist keine gueltige Zahl. Programmabbruch." << std::endl;
  }
  
  
  //TODO: Relatives Residuum in Datei schreiben, gnuplot?
  // Vielleicht TODO: Matrix ueberpruefen, sodass keine Nullen auf Diagonale stehen
  
  

}

