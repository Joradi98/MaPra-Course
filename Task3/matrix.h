/***********************************************
*  Name        : matrix.h                      *
*  Verwendung  : Header zu Matrixklasse        *
***********************************************/

#ifndef _MATRIX_H                         // matrix.h nicht doppelt benutzen
#define _MATRIX_H

#include <iostream>
#include <vector>
#include <string>


class Matrix
{
  private:
    size_t Zeil, Spalt; // Dimensionen der Matrix
    std::vector< std::vector<double> > mat;  // Verschachtelte std::vector. Effizienter wäre wahrscheinlich ein 2d-Array vom Typ double
  public:
    explicit Matrix (size_t m=1, size_t n=1); // Matrix mit m Zeilen und n Spalten anlegen. Default 1, damit Felder von Matrizen angelegt werden koennen
    ~Matrix () {};                        // Destruktor

    double& operator () (size_t, size_t);         // Zugriff auf Einträge der Matrix
    double  operator () (size_t, size_t) const;   // Zugriff auf falls Matrix const

    Matrix& ReDim   (size_t, size_t);                   // neue Dimensionen festlegen
    size_t  Zeilen  () const {  }  // Zeilen
    size_t  Spalten  () const {}  // Spalten

    static void MatrixFehler (const std::string& str);   // Fehlermeldung ausgeben

    friend std::istream& operator >> (std::istream&, Vektor&);       // Eingabe
    friend std::ostream& operator << (std::ostream&, const Vektor&); // Ausgabe

};

#endif
