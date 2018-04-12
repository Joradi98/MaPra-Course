/***********************************************************
*  Name       : meina1.cpp                                 *
*  Verwendung : Loesung der ersten Praktikumsaufgabe,      *
*               Nullstellen eines quadratischen Polynoms   *
*  Autor      :                                            *
*  Datum      :                                            *
*  Sprache    : C++                                        *
***********************************************************/

// Einbinden der Praktikums-Umgebung. Falls Sie die Ein-/Ausgabe zuerst
// nicht ueber die Praktikumsumgebung machen wollen, sollten Sie auch noch
// #include <iostream> einbinden.

#include "unit.h"
#include <iostream>
#include <complex>

using namespace std;

// ===== Hauptprogramm =====

// represents a Polynomial f(x) = ax^2 + bx + c with a, b, c in R
class Polynomial {
    public:
        Polynomial(double a, double b, double c);
        bool isConstant();
        bool isLinear();	
        bool isZeroPolynomial();
        double a, b, c;
};

Polynomial::Polynomial(double a, double b, double c){
    this -> a = a;
    this -> b = b;
    this -> c = c;
}

bool Polynomial::isConstant(){
    return a == 0 && b == 0;
}

bool Polynomial::isLinear(){
    return a == 0;
}

bool Polynomial::isZeroPolynomial(){
    return isConstant() && c == 0;
}

void handleZeroPolynomial(Polynomial &poly){
    if(poly.isZeroPolynomial()){
        cout << "Infinite many zeros." << endl;
        Ergebnis(Unendlich);
    }
    else {
        cout << "no zeros." << endl;
        Ergebnis(0);
    }
}

void handleLinearPolynomial(Polynomial &poly){
    // there is only one zero
    double zero = -poly.c / poly.b;
    cout << "one zero: " << zero << endl;
    // this single zero will always be real
    Ergebnis(1,false,zero);    
}

void handleBZeroPolynomial(Polynomial &poly){
    // polynomial is of type f(x)=ax^2+c
    complex<double> square = complex<double>( -poly.c / poly.a, 0 );
    complex<double> zero1 = sqrt(square);
    cout << "two zeros. zero1: " << zero1 << " and its conjugate" << endl;
    Ergebnis(2, true, zero1.real(), abs( zero1.imag() ));
}

void handleCompleteQuadraticPolynomial(Polynomial &poly){
    double p = poly.b / poly.a;
    double q = poly.c / poly.a;
    double pHalf = p / 2;
	
    complex<double> pqRoot;
    // if p big number, factorise abs(p) to avoid overflow
    // check, whether (p/2)^2 will be bigger than DBL_MAX
    if( pHalf > sqrt(DBL_MAX) ){
        complex<double> square( (pHalf * pHalf) - q, 0 );
        pqRoot = sqrt(square);
    }
    else {
        complex<double> square( (1/4) - (q/p)/p , 0 );
        pqRoot = abs(p) * sqrt(square);
    }
			
    cout << "pq: " << pqRoot << endl;
    complex<double> zero1, zero2;

    // use Vieta to avoid cancellation
    if(p > 0){
        zero2 = complex<double>(-pHalf,0) - pqRoot;
        zero1 = q / zero2;
    }
    else {
        zero1 = complex<double>(-pHalf,0) + pqRoot;
        zero2 = q / zero1;
    }
    cout << "two zeros. zero1: " << zero1 << ", zero2: " << zero2 << "\n";
        
    // check for complex values, as this requires another parameter in Ergebnis()
    if (zero1.imag() != 0) {
        // pass the real and imaginary part to the Ergebnis-function
        Ergebnis(2, true, zero1.real(), abs( zero1.imag() ));
    } 
    else {
        Ergebnis(2, false, zero1.real(), zero2.real());
    }        
}

void calculateZeros(Polynomial &poly){
    cout << "calculating zeros of f: R -> R, f(x) = " << poly.a << "x^2 + " << poly.b << "x + " << poly.c << endl;
    if(poly.isConstant()){
        handleZeroPolynomial(poly);
    }
    else if(poly.isLinear()){
        handleLinearPolynomial(poly);
    }
    else if(poly.b == 0){
        handleBZeroPolynomial(poly);
    }
    else {
        // .. it should be quadratic
        handleCompleteQuadraticPolynomial(poly);
    }
}

bool isInteger(string s){
    for(char c : s){
        if(!isdigit(c)) return false;
    }
    return true;
}

int main(){
    cout << "What would you like to do?" << endl;
    cout << "  manual: manually specify coefficients" << endl;
    cout << "  1-" << AnzahlBeispiele << ":   test n-th polynomial from unit.o" << endl;
    cout << "  all:    test all polynomials from unit.o" << endl;
    string command;    
    cin >> command;
    double a, b, c;
    if(command == "manual"){
        cout << "specify values for the coefficients:" << endl;
        cout << "a: "; cin >> a;
        cout << "b: "; cin >> b;
        cout << "c: "; cin >> c;
        Polynomial p(a, b, c);
        calculateZeros(p);
    }
    else if(command == "all"){
        for(int i = 0; i < AnzahlBeispiele; i++){
            Start(i + 1, a, b, c);
            Polynomial p(a, b, c);
            calculateZeros(p);
        }
    }
    else if(isInteger(command)){
        Start(stoi(command), a, b, c);
        Polynomial p(a, b, c);
        calculateZeros(p);
    }
    else {
        cout << "unknown command. exiting.." << endl;
    }
    return 0;
}
