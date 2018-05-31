#include <cmath>
#include <iostream>
#include <random>
#include "unit.h"
#include "Spielbrett.h"
#include <algorithm>

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



bool Spielbrett::setzeStein(int Spalte, Feld farbe) {

    if ( this->matrix[0][Spalte] != leer ) {
       // std::cout << "Die oberste Zeile ist schon voll" << std::endl;
        return false;
    }

    //Stein ganz unten einfuegen
    for (int i = (Zeil-1); i > 0; i-- ) {
        if ((*this)(i,Spalte) == leer) {
            (*this)(i,Spalte) = farbe;
            return true;
        }
    }

    return false;

}

Feld Spielbrett::getColor(int i){
    if(i == 0) return gelb;
    return rot;
}

int Spielbrett::getColorId(Feld color){
    if(color == gelb) return 0;
    return 1;
}


bool Spielbrett::addTile(int col, int color){
    return setzeStein(col, getColor(color));
}

void Spielbrett::entferneStein(int Spalte) {

    //Stein ganz oben loeschen
    for (int i = 0; i < Zeil; i++ ) {
        if ((*this)(i,Spalte) != leer) {
            (*this)(i,Spalte) = leer;
            return;
        }
    }


}



//Schaut sich die ersten 4 Feldfarben an und vergibt Punkte nach dem Schema der Heuristik
double punkteImBereich(std::vector<Feld> v, Feld farbe) {

    if (v.size() != 4 && farbe == leer) { //Unsinnige Eingabem
        return 0;
    }

    int numberOfRed = std::count(v.begin(), v.end(), rot);
    int numberOfYellow = std::count(v.begin(), v.end(), gelb);
 /*   std::cout << numberOfRed << " Rote und " << numberOfYellow << " Gelbe" << std::endl;
    std::vector<Feld> bereich; //Konstruiere Bereich aus 4 senkrecht aufeinanderdolgenden Feldern
    bereich.push_back(rot);
    std::cout << bereich[0] << std::endl;*/

    if (numberOfRed != 0 && numberOfYellow != 0) {
        return 0;
    }


    double factor = 1.0/42.0;
    if (farbe == rot) {
        return numberOfRed >= 2 ? numberOfRed * factor : 0; //Nur zwei oder mehr werden berücksichtigt
    } else {
        return numberOfYellow >= 2 ? numberOfYellow * factor : 0;
    }


}

bool bereichIstGewonnen(std::vector<Feld> v, Feld farbe) {
    if (v.size() != 4 && farbe == leer) { //Unsinnige Eingabem
        return false;
    }
    int streak = std::count(v.begin(), v.end(), farbe);

    return (streak == 4) ? 1 : 0;
}

double Spielbrett::heuristischeBewertung(Feld farbe) {
    double sum = 0;

    Feld gegenfarbe  = farbe == rot ? gelb : rot;

    //Senkrecht in jeder Spalte
    for (unsigned int spalte = 0; spalte < Spalt; spalte++ ) {
        for (unsigned int i = 0; i < Zeil-3; i++) {
            std::vector<Feld> bereich; //Konstruiere Bereich aus 4 senkrecht aufeinanderdolgenden Feldern
            bereich.push_back((*this)(i,spalte));
            bereich.push_back((*this)(i+1,spalte));
            bereich.push_back((*this)(i+2,spalte));
            bereich.push_back((*this)(i+3,spalte));

            if (bereichIstGewonnen(bereich, farbe)) {return 1;}
            if (bereichIstGewonnen(bereich, gegenfarbe)) {return -1;}

            sum += punkteImBereich(bereich, farbe);
            sum -= punkteImBereich(bereich, gegenfarbe);
        }
    }

    //Waagerecht in jeder Zeile
    for (unsigned int zeile = 0; zeile < Zeil; zeile++ ) {
        for (unsigned int i = 0; i < Spalt-3; i++) {
            std::vector<Feld> bereich; //Konstruiere Bereich aus 4 waagerecht aufeinanderdolgenden Feldern
            bereich.push_back((*this)(zeile,i));
            bereich.push_back((*this)(zeile,i+1));
            bereich.push_back((*this)(zeile,i+2));
            bereich.push_back((*this)(zeile,i+3));

            if (bereichIstGewonnen(bereich, farbe)) {return 1;}
            if (bereichIstGewonnen(bereich, gegenfarbe)) {return -1;}

            sum += punkteImBereich(bereich, farbe);
            sum -= punkteImBereich(bereich, gegenfarbe);
        }
    }

    //Diagonalen mit 4 Elementen: links unten nach rechts oben
    for (unsigned int spalte = 0; spalte < Spalt-3; spalte++ ) {     // In jeder Spalte, sodass nach rechts noch 4 Elemente platz sind
        for (unsigned int i = 3; i < Zeil; i++) {                   // Starte in der 4. Zeile
            std::vector<Feld> bereich; //Konstruiere Bereich aus 4 diagonal aufeinanderdolgenden Feldern
            bereich.push_back((*this)(i,spalte));
            bereich.push_back((*this)(i-1,spalte+1));                // Nach recht oben wandern
            bereich.push_back((*this)(i-2,spalte+2));
            bereich.push_back((*this)(i-3,spalte+3));

            if (bereichIstGewonnen(bereich, farbe)) {return 1;}
            if (bereichIstGewonnen(bereich, gegenfarbe)) {return -1;}

            sum += punkteImBereich(bereich, farbe);
            sum -= punkteImBereich(bereich, gegenfarbe);
        }
    }

    //Diagonalen mit 4 Elementen: links oben nach rechts unten
    for (unsigned int spalte = 3; spalte < Spalt; spalte++ ) {     // In jeder Spalte, sodass nach links noch 4 Elemente platz sind
        for (unsigned int i = 3; i < Zeil; i++) {                   // Starte in der 4. Zeile
            std::vector<Feld> bereich; //Konstruiere Bereich aus 4 diagonal aufeinanderdolgenden Feldern
            bereich.push_back((*this)(i,spalte));
            bereich.push_back((*this)(i-1,spalte-1));                // Nach links oben wandern
            bereich.push_back((*this)(i-2,spalte-2));
            bereich.push_back((*this)(i-3,spalte-3));

            if (bereichIstGewonnen(bereich, farbe)) {return 1;}
            if (bereichIstGewonnen(bereich, gegenfarbe)) {return -1;}
            sum += punkteImBereich(bereich, farbe);
            sum -= punkteImBereich(bereich, gegenfarbe);
        }
    }



   // std::cout << "Bewertung: " << sum << std::endl;

    return sum;
}


bool Spielbrett::spielIstBeendet()
{
    return fabs((*this).heuristischeBewertung(rot)) == 1;
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
