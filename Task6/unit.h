/*******************************************************************
*  Name       : unit.h                                             *
*  Verwendung : Schnittstelle zu Praktikumsumgebung (MAPRA),       *
*               Adaptive Quadratur                                 *
*  Autor      : Karl-Heinz Brakhage, Kolja Brix, IGPM RWTH Aachen  *
*  Datum      : April 2007                                         *
*******************************************************************/

// Verwendete Header einbinden

#include <cmath>
#include <iostream>

const double Pi = 4*std::atan(1.0);

// Externe Variablen aus der Praktikumsumgebung

extern const int AnzahlBeispiele;

// Die Funktion "f" stellt die Funktion zur Verfuegung, deren Integral
// bestimmt werden soll.
double f ( double x );

// Die folgenden Funktionen sorgen fuer die Beurteilung und
// eventuell die grafische Ausgabe der Ergebnisse.
// Fuer eine genauere Beschreibung siehe Aufgabenblatt.

void Start    ( int Bsp, double &a, double &b, double &epsilon,
                bool Grafik=true, int Pause=300 );

bool Ergebnis ( double I );
