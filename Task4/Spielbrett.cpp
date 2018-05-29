#include <cmath>
#include <iostream>
#include <random>
#include "unit.h"
#include "Spielbrett.h"

// =======================
//      Konstruktoren
// =======================


// ----- Konstruktor -----

Spielbrett::Spielbrett(size_t m, size_t n)
{
    Zeil = m;
    Spalt = n;

    //Erzeuge eine Nullzeile mit n leeren Feldern
    std::vector<Feld> nullzeile  ;
    nullzeile.clear();
    nullzeile.resize(n,leer);

    //Fuege m Nullzeilen hinzu
    matrix.clear();
    matrix.resize(m,nullzeile);

}


Feld& Spielbrett::operator () (size_t i, size_t j) {
#ifndef NDEBUG
    if ( Zeil < i ) {
        std::cout << std::endl << "Will Zeile " << i << ", hat aber nur: " << Zeil << std::endl;
        Spielbrett::BrettFehler("Es sind nicht genug Zeilen vorhanden");
    }
    
    if ( Spalt < j) {
        std::cout << std::endl << "Will Spalte " << j << ", hat aber nur: " << Spalt << std::endl;
        Spielbrett::BrettFehler("Es sind nicht genug Spalten vorhanden");
    }
#endif

    return matrix[i][j];
}



void Spielbrett::setzeStein(int Spalte, Feld farbe) {
#ifndef NDEBUG
    if ( this->matrix[1][Spalte] != leer ) {
        std::cout << "Die oberste Zeile ist schon voll" << std::endl;
    }
#endif

    //Stein ganz unten einfuegen
    for (int i = Zeil; i >= 1; i-- ) {
        if (this->matrix[i][Spalte] == leer) {
            this->matrix[i][Spalte] = farbe;
            return;
        }
    }


}





bool Spielbrett::spielIstBeendet() 
{
    
 /*  //Senkrecht
    for (int i) {
        
    }*/

    (*this)(1,1);
    return true;
}


// ==========================
//      Fehlerbehandlung
// ==========================


// ----- Ausgabe der Fehlermeldung "str" -----

void Spielbrett::BrettFehler (const std::string& str)
{
    std::cerr << "\nBrettFehler: " << str << '\n' << std::endl;
    exit(1);
}




