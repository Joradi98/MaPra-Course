#include <cmath>
#include <iostream>
#include <random>


#ifndef _SPIELBRETT_H                         				// nicht doppelt benutzen
#define _SPIELBRETT_H

enum Feld
{ leer, gelb, rot };


class Spielbrett
{
	private:
		size_t Zeil, Spalt; 								// Dimensionen des Spielbretts
		std::vector< std::vector<Feld> > matrix; 
	public:
		explicit Spielbrett (size_t m=1, size_t n=1);
		
		bool spielIstBeendet();
		
		Feld& operator () (size_t, size_t);       			// Zugriff auf Einträge der Matrix
		
		bool setzeStein(int , Feld);					// Gibt zuruekc, ob die Aktion erfolgreich war
		void entferneStein(int);					// Entfernt obersten Stein aus einer Spalte


		double heuristischeBewertung(Feld); 					//Bewertet die aktuelle Verteilung der Steine fuer eine gegebene Farbe

		static void BrettFehler (const std::string& str);   // Fehlermeldung ausgeben
};

#endif
