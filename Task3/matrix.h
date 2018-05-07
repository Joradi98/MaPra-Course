/***********************************************
*  Name        : matrix.h                      *
*  Verwendung  : Header zu Matrixklasse        *
***********************************************/

#ifndef _MATRIX_H                         // matrix.h nicht doppelt benutzen
#define _MATRIX_H

#include "vektor.h"
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
    size_t  Zeilen  () const { return Zeil; }  // Zeilen
    size_t  Spalten  () const { return Spalt; }  // Spalten


    Matrix& operator =  (const Matrix&);  // Zuweisung
    Matrix& operator += (const Matrix&);  // Zuweisungen mit arithm. Operation
    Matrix& operator -= (const Matrix&);
    Matrix& operator *= (const double); // Multiplikation mit einem Faktor

    friend Matrix operator +  (const Matrix&, const Matrix&); // Addition
    friend Matrix operator -  (const Matrix&, const Matrix&); // Subtraktion
    friend Matrix operator -  (const Matrix&);                // Vorzeichen


    friend Matrix operator *  (const double,  const Matrix&); // Vielfache
    friend Matrix operator *  (const Matrix&, const double);
    friend Matrix operator /  (const Matrix&, const double);
    friend Matrix operator *  (const Matrix&, const Matrix&); // Matrixprodukt

    friend Vektor   operator *  (const Matrix&, const Vektor&); // Matrix-Vektor-
    friend Vektor   operator *  (const Vektor&, const Matrix&); // Multiplikation


    static void MatFehler (const std::string& str);   // Fehlermeldung ausgeben

    friend std::istream& operator >> (std::istream&, Matrix&);       // Eingabe
    friend std::ostream& operator << (std::ostream&, const Matrix&); // Ausgabe




};

#endif
