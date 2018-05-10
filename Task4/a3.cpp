//------------------------------------------------------------------------------
// Mapra WS 2000, A. Voss, A3: Primzahltests
//------------------------------------------------------------------------------

#include <iostream>
#include <cmath>

#include "a3.h"

//------------------------------------------------------------------------------
// bitte mit Leben fuellen:
//------------------------------------------------------------------------------

// Probe

lint PrimTest(lint n, const CSieb& Sieb){ 
    lint lowestFactor = 1;
    if(!Sieb[n]){
        lowestFactor++;
        for(; n % lowestFactor != 0; lowestFactor++){
        } 
    } 
    return lowestFactor;
}

// Fermat

lint FermatTest(lint n, lint a)
{ return 0; }

// Miller-Rabin

lint MillerRabinTest(lint n, lint a, lint d, lint r)
{ return 0; }

// Hilfsfunktionen

lint fastpow(lint nBase, lint nExp, lint nMod)
{ return 0; }

lint gcd(lint a, lint b)
{ return 0; }

void get_ds(lint n, lint& d, lint& s)
{ return; }

//------------------------------------------------------------------------------
// Hauptprogramm
//------------------------------------------------------------------------------

int main()
{
	CSieb Sieb;
    for(lint i = 0; i < Sieb.length(); i++){
        Sieb[i] = true;
    }
    Sieb[0] = false; 
    Sieb[1] = false;
    std::cout << "calculating sieve.." << std::endl;
    for(lint i = 0; i < Sieb.length(); i++){
        for(; i < Sieb.length() && !Sieb[i]; i++){}
        //std::cout << "-- filling multiples of " << i << ".." << std::endl;
        for(lint j = 2;;j++){
            if(i * j >= Sieb.length()) break;
            Sieb[i * j] = false;
        }
    }
    std::cout << "sieve ready." << std::endl;
	// ... Ihre Berechnung des Siebs

	SiebTest(Sieb);

	// PrimTest
    std::cout << PrimTest(9, Sieb) << std::endl;    

	// Fermat-Test

	// Miller-Rabin-Test

	return 0;
}

