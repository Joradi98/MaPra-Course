#include <iostream>
#include "unit.h"
#include "stdlib.h"
#include <map>


//MARK: GLOBALE VARIABLEN

std::map<double,double> fValues; 			//Speichert die bereits berechneten Werte von f
double minimalSchrittWeite = INFINITY;		//Speicher den Wert, den wir bei der adaptiven Methode brauchten, um das Integral auszurechnen.
double getFValue(double x) {				//Getter-Methode für die f-Werte, denn wir können sie vorher aus dem Speicher ziehen
	if ( fValues.count(x) == 0 ) {
		fValues[x] = f(x);
	}
	return fValues[x];
}


/**
Integral mit der adaptiven Methode benutzen. (rekursiv auf der linken und rechten Hälfte, im Basisfall die Simpsonregel, Fehlerschaetzer mittel |Trapez-Mittelwert|).
Korrektheit für konvexe/konkave Funktionen bewiesen.
*/
double adaptivesIntegral(double a, double b, double eps) {
	
	//Basisfall der Rekursion: Genuigkeit reicht schon aus.
	double mittelpunkt = (a+b)/2.0;
	double mittelpunktsRegel = (b-a)*getFValue( mittelpunkt );
	double trapezRegel = (b-a)*(0.5*getFValue(a)+0.5*getFValue(b));

	//Update minimale Schrittweite
	if ( b-a < minimalSchrittWeite) {
		minimalSchrittWeite = b-a;
	}


	if ( fabs(trapezRegel - mittelpunktsRegel) < eps ) {
		//return simpson
		double simpsonRegel = (b-a)*(0.5*getFValue(a)+0.5*getFValue(b));
		return simpsonRegel;
	}

	//Ansonsten: In Teilintervalle aufteilen:
	return adaptivesIntegral(a,mittelpunkt,eps/2.0) + adaptivesIntegral(mittelpunkt, b, eps/2.0);
	
}

/**
Integral mit der summierten Trapezformel berechnen. Dabei wird die globale Variable INFINITY benutzt => Vorher MUSS adaptivesIntegral() aufgerufen worden sein.
*/
double summiertesTrapezIntegral(double a, double b, double eps) {
	double h = minimalSchrittWeite;
	double n = (b-a)/h;
	double sum = 0;
	
	sum += 0.5*h*getFValue(a);
	sum += 0.5*h*getFValue(b);
	for (int j = 1; j < n-1; j++) {
		sum += h*getFValue(a+j*h);
	}

	return sum;
}



/**
Loesungs-Programm: Nach der Beispielnummer fragen und dann nach den zwei Methoden das Integral berechnen.
*/
int main(int argc, char *argv[]) {
	
	int example = 0;
	std::cout << "Welches Beispiel darf's sein? [1-6]: ";
	std::cin >> example;
	if ( example > AnzahlBeispiele ) {
		std::cout << "So viele Beispiele gibt es gar nicht!" << std::endl;
		return -1;
	}
	
	double a, b, eps;
	
	Start( example, a, b, eps, true, 10000);
	
	double I = adaptivesIntegral(a, b, eps);
	Ergebnis(I);
	I = summiertesTrapezIntegral(a, b, eps);
	Ergebnis(I);

		
}
