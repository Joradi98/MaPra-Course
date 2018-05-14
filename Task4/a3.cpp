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


// Miller-Rabin

lint MillerRabinTest(lint n, lint a, lint d, lint r)
{ return 0; }

// Hilfsfunktionen

lint fastpow(lint nBase, lint nExp, lint nMod){ 
    lint current = nExp;
    lint result = 1;
    lint lastSquare = 0;
    lint lastSquareI = 0;
    lint mulValue = 0;
    for(int i = 0;; i++){
        std::cout << "current: " << current << std::endl;
        if(current == 0) return result;
        lint currentBit = current & 1;
        if(currentBit == 1){
            lint expValue = (2 << (i - 1));
            std::cout << "current & 1: " << (current & 1) << std::endl;
            std::cout << ".. times: " << expValue << std::endl;
            //use previous value, if there is
            if(i % 2 == 1 && lastSquareI == i - 1){
                mulValue = mulValue * mulValue;
            }
            else {
                mulValue = pow(nBase, expValue);
            }
            lastSquare = mulValue;
            lastSquareI = i;
            std::cout << "|-> mulValue before mod " << nMod << ": " << mulValue << std::endl;
            mulValue = mulValue % nMod;
            std::cout << "|-> mulValue after mod " << nMod << ": " << mulValue << std::endl; 
            result *= mulValue;
            result = result % nMod;
            std::cout << "result: " << result << std::endl;
        }
        current = current >> 1;
    }
    return 0; 
}

lint gcd(lint a, lint b){ 
    lint x;
    if(a == 0) return b;
    if(b == 0) return a;
    while(b != 0){
        x = a % b;
        a = b;
        b = x;
    }
    return a;
}

void get_ds(lint n, lint& d, lint& s)
{ return; }

// Fermat

lint FermatTest(lint n, lint a){ 
    return fastpow(a, n-1, n);
}


//------------------------------------------------------------------------------
// Hauptprogramm
//------------------------------------------------------------------------------

int main()
{
	CSieb Sieb;
    std::cout << "creating Sieb of size " << Sieb.length() << "." << std::endl;
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
    
    std::cout << "testing \"Probe\".. " << AnzahlBeispiele << " for testing found. " << std::endl;
    for(unsigned int i = 1; i <= AnzahlBeispiele; i++){
        lint testValue = HoleKandidaten(Probe, i);
        std::cout << "-- testing value: " << testValue << std::endl;
        if(testValue < Sieb.length()){
            TestErgebnis(Probe, i, PrimTest(testValue, Sieb)); 
        }
    }   
    
    std::cout << "fastpow(4, 24, 123): " << fastpow(4, 24, 123) << std::endl;
	std::cout << "gcd(12345, 5245): " << gcd(12345, 5245) << std::endl;
    // Fermat-Test
    
    for(unsigned int i = 1; i <= AnzahlBeispiele; i++){
        lint testValue = HoleKandidaten(Fermat, i);
        std::cout << "-- testing value: " << testValue << std::endl;
        lint result = FermatTest(1, testValue);
        std::cout << "fermat result" << result << std::endl;
    }

	// Miller-Rabin-Test

	return 0;
}

