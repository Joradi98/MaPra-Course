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
}




// ===========================================
//      Eintraege schreiben und lesen
// ===========================================

double& Matrix::operator () (size_t i, size_t j) {
#ifndef MATRIX_DEBUG
#endif

  double k = 42.0;
  return k;
}
double Matrix::operator () (size_t i, size_t j) const {
#ifndef MATRIX_DEBUG
#endif

  double k = 42.0;
  return k;
}

// =====================
//      Zuweisungen
// =====================

// ----- Zuweisungsoperator "=" ----

Matrix& Matrix::operator = (const Matrix& x)
{
#ifndef MATRIX_DEBUG
#endif

  return *this;
}


// ----- Zuweisungsoperator mit Addition "+=" ----

Matrix& Matrix::operator += (const Matrix& x)
{
  // ***** Hier fehlt was *****
  return *this;

}


// ----- Zuweisungsoperator mit Subtraktion "-=" ----

Matrix& Matrix::operator -= (const Matrix& x)
{
  // ***** Hier fehlt was *****
  return *this;
}


// ----- Zuweisungsoperator mit Multiplikation "*=" ----

Matrix& Matrix::operator *= (const double c)
{
  // ***** Hier fehlt was *****
  return *this;

}

// =====================
//      Dimensionen
// =====================

Matrix& Matrix::ReDim (size_t m, size_t n)
{
  
} 




// ==================================
//      arithmetische Operatoren
// ==================================


// ----- Addition "+" -----

Matrix operator + (const Matrix& x, const Matrix& y)
{
#ifndef MATRIX_DEBUG
#endif

  return Matrix();
}


// ----- Subtraktion "-" -----

Matrix operator - (const Matrix& x, const Matrix& y)
{
  // ***** Hier fehlt was *****
  return Matrix();

}


// ----- Vorzeichen wechseln "-" -----

Matrix operator - (const Matrix& x)
{
  // ***** Hier fehlt was *****
  return Matrix();

}


// ----- Matrixprodukt -----

Matrix operator * (const Matrix& x, const Matrix& y)
{
  // ***** Hier fehlt was *****
  return Matrix();

}

// ----- Skalare Vielfache -----

Matrix operator * (const Matrix& x,  double y)
{
  // ***** Hier fehlt was *****
  return Matrix();

}

Matrix operator * (double y,  const Matrix& x)
{
  // ***** Hier fehlt was *****
  return Matrix();

}

Matrix operator / (const Matrix& x,  double y)
{
  // ***** Hier fehlt was *****
  return Matrix();

}

// ----- Matrix mal Vektor -----

Vektor   operator *  (const Matrix&, const Vektor&)
{
  return Vektor();
} 

Vektor   operator *  (const Vektor&, const Matrix&)
{
  return Vektor();
} 


// ==========================
//      Fehlerbehandlung
// ==========================


// ----- Ausgabe der Fehlermeldung "str" -----

void Matrix::MatFehler (const std::string& str)
{
  std::cerr << "\nVektorfehler: " << str << '\n' << std::endl;
  exit(1);
}

