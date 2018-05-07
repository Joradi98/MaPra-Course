/***********************************************
*  Name       : matrix.cpp                     *
*  Verwendung : Matrixklasse                   *
***********************************************/

#include "matrix.h"
#include <iomanip>
#include <cmath>
#include <cstdlib>

// =======================
//      Konstruktoren
// =======================


// ----- Konstruktor -----

Matrix::Matrix(size_t m, size_t n)
{
  #ifndef NDEBUG
    if (m <= 0 || n <= 0)
      Matrix::MatFehler("Nur Matrizen mit positiver Anzahl an Zeilen Und Spalten sind zugelassen!");
  #endif

  this->ReDim(m, n);
}




// ===========================================
//      Eintraege schreiben und lesen
// ===========================================

double& Matrix::operator () (size_t i, size_t j) {
#ifndef NDEBUG
  if ( Zeil < i ) {
    std::cout << std::endl << "Will Zeile " << i << ", hat aber nur: " << Zeil << std::endl;
    Matrix::MatFehler("Es sind nicht genug Zeilen vorhanden");
  }
  
  if ( Spalt < j) {
    std::cout << std::endl << "Will Spalte " << j << ", hat aber nur: " << Spalt << std::endl;
    Matrix::MatFehler("Es sind nicht genug Spalten vorhanden");
  }
#endif

  return mat[i][j];
}

double Matrix::operator () (size_t i, size_t j) const {
#ifndef NDEBUG
  if ( Zeil < i ) {
    std::cout << std::endl << "Const: Will Zeile " << i << ", hat aber nur: " << Zeil << std::endl;
    Matrix::MatFehler("Es sind nicht genug Zeilen vorhanden");
  }
  
  if ( Spalt < j) {
    std::cout << std::endl << "Const: Will Spalte " << j << ", hat aber nur: " << Spalt << std::endl;
    Matrix::MatFehler("Es sind nicht genug Spalten vorhanden");
  }
#endif

  return mat[i][j];
}

// =====================
//      Zuweisungen
// =====================

// ----- Zuweisungsoperator "=" ----

Matrix& Matrix::operator = (const Matrix& x)
{
/*#ifndef NDEBUG
  if ( Zeil != x.Zeilen())
    Matrix::MatFehler("Inkompatible Zeilen fuer 'Matrix = Matrix'");
  if ( Spalt != x.Spalten())
    Matrix::MatFehler("Inkompatible Spalten fuer 'Matrix = Matrix'");
#endif*/

  this->mat=x.mat;

/*  size_t i, j;
  for (i = 0; i < Zeil; i++) {
    for (j = 0; i < Spalt; j++) {
      mat[i][j] = x(i,j);
    }
  }*/


  return *this;
}


// ----- Zuweisungsoperator mit Addition "+=" ----

Matrix& Matrix::operator += (const Matrix& x)
{
  #ifndef NDEBUG
    if (Zeil != x.Zeilen() || Spalt != x.Spalten())
      Matrix::MatFehler("Inkompatible Dimensionen fuer 'Matrix += Matrix'!");
  #endif

  size_t i, j;
  for (i = 0; i < Zeil; i++) {
    for (j = 0; j < Spalt; j++) {
      (*this)(i,j) += x(i,j); 
    }
  }

  
  return *this;

}


// ----- Zuweisungsoperator mit Subtraktion "-=" ----

Matrix& Matrix::operator -= (const Matrix& x)
{
  #ifndef NDEBUG
    if (Zeil != x.Zeilen() || Spalt != x.Spalten())
      Matrix::MatFehler("Inkompatible Dimensionen fuer 'Matrix += Matrix'!");
  #endif

  size_t i, j;
  for (i = 0; i < Zeil; i++) {
    for (j = 0; j < Spalt; j++) {
      (*this)(i,j) -= x(i,j); 
    }
  }

  
  return *this;
}


// ----- Zuweisungsoperator mit Multiplikation "*=" ----

Matrix& Matrix::operator *= (const double c)
{

  size_t i, j;
  for (i = 0; i < Zeil; i++) {
    for (j = 0; j < Spalt; j++) {
      (*this)(i,j) *= c;
    }
  }

  
  return *this;

}


// ----- Zuweisungsoperator mit Multiplikation "/=" ----

Matrix& Matrix::operator /= (const double c)
{

  #ifndef NDEBUG
    if (c == 0.0)
      Matrix::MatFehler("Auch Matrizen duerfen nicht durch Null teilen!");
  #endif

  size_t i, j;
  for (i = 0; i < Zeil; i++) {
    for (j = 0; j < Spalt; j++) {
      (*this)(i,j) /= c;
    }
  }

    
  return *this;


}



// =====================
//      Dimensionen
// =====================

Matrix& Matrix::ReDim (size_t m, size_t n)
{
  #ifndef NDEBUG
    if (m <= 0 || n <= 0)
      Matrix::MatFehler("Nur eine positiver Anzahl an Zeilen Und Spalten ist zugelassen!");
    
  #endif
  
  Zeil = m;
  Spalt = n;
  
  //Erzeuge eine Nullzeile mit n Nullen
  std::vector<double> nullzeile  ;
  nullzeile.clear();
  nullzeile.resize(n,0.0);
  
  //Fuege m Nullzeilen hunzu
  mat.clear();
  mat.resize(m,nullzeile);

  

} 




// ==================================
//      arithmetische Operatoren
// ==================================


// ----- Addition "+" -----

Matrix operator + (const Matrix& x, const Matrix& y)
{
  #ifndef NDEBUG
  if (x.Zeilen() != y.Zeilen() || x.Spalten() != y.Spalten()) {
    std::cout << std::endl << "Will Zeilen " << x.Zeilen() << ", hat aber nur: " << y.Zeilen() << std::endl;
    Matrix::MatFehler("Inkompatible Dimensionen fuer 'Matrix + Matrix'!");
  }
  #endif
  
  Matrix z = x;
  return z += y;
}


// ----- Subtraktion "-" -----

Matrix operator - (const Matrix& x, const Matrix& y)
{
  #ifndef NDEBUG
  if (x.Zeilen() != y.Zeilen() || x.Spalten() != y.Spalten()) {
    std::cout << std::endl << "Will Zeilen " << x.Zeilen() << ", hat aber nur: " << y.Zeilen() << std::endl;
    Matrix::MatFehler("Inkompatible Dimensionen fuer 'Matrix + Matrix'!");
  }
  #endif
  
  Matrix z = x;
  return z -= y;

}


// ----- Vorzeichen wechseln "-" -----

Matrix operator - (const Matrix& x)
{
  Matrix z = 0*x - x; //Nullvektor initialisieren
  return z;
}


// ----- Matrixprodukt -----

Matrix operator * (const Matrix& A, const Matrix& B)
{
  #ifndef NDEBUG
    if (A.Spalten() != B.Zeilen())
      Matrix::MatFehler("Inkompatible Dimensionen fuer 'Matrix*Matrix'!");
  #endif

  Matrix result = Matrix(A.Zeilen(),B.Spalten());
  size_t i, k, j;
  
  for (i = 0; i < A.Zeilen(); i++) {
    for (k = 0; k < B.Spalten(); k++) {
      for (j = 0; j < B.Zeilen(); j++) {
        result(i,k) += A(i,j)*B(j,k);
      }
    }
  }
  return result;
}

// ----- Skalare Vielfache -----

Matrix operator * (const Matrix& x,  double y)
{
  Matrix z = x;
  return z *= y;
}

Matrix operator * (double y,  const Matrix& x)
{
  Matrix z = x;
  return z *= y;

}

Matrix operator / (const Matrix& x,  double y)
{
  #ifndef NDEBUG
    if (y == 0.0)
      Matrix::MatFehler("Auch Matrizen duerfen nicht durch Null teilen!");
  #endif

  Matrix z = x;
  return z /= y;

}

// ----- Matrix mal Vektor -----

Vektor   operator *  (const Matrix& A, const Vektor& x)
{
  #ifndef NDEBUG
    if (A.Spalten() != x.Laenge())
      Matrix::MatFehler("Inkompatible Dimensionen fuer 'Matrix*Vektor'!");
  #endif

  Vektor result = Vektor(A.Zeilen());

  size_t i,j;
  for (i = 0; i < A.Zeilen(); i++) {
    //Berechne den i-ten Eintrag des result-Vektors: i-te Zeile mal Vektor
    for (j = 0; j < A.Spalten(); j++) {
      result(i) += A(i,j)*x(j);
    }
    
    
  }
    
  return result;
} 

// Das wird ein bisschen hacky, um die Tests zu erfuellen. Wieso auch immer dieses Produkt so sinnvol sein sollte...
Vektor   operator *  (const Vektor& x, const Matrix& A)
{
  #ifndef NDEBUG
    if (A.Zeilen() != x.Laenge())
      Matrix::MatFehler("Inkompatible Dimensionen fuer 'Vek*Mal'!");
  #endif

  Vektor result = Vektor(A.Spalten());

  size_t i,j;
  for (i = 0; i < A.Spalten(); i++) {
    //Berechne den i-ten Eintrag des result-Vektors: i-te Zeile mal Vektor
    for (j = 0; j < A.Zeilen(); j++) {
      result(i) += A(j,i)*x(j);
    }
    
    
  }
  return result;
} 


// ==========================
//      Fehlerbehandlung
// ==========================


// ----- Ausgabe der Fehlermeldung "str" -----

void Matrix::MatFehler (const std::string& str)
{
  std::cerr << "\nMatrixFehler: " << str << '\n' << std::endl;
  exit(1);
}

