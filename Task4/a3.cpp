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
    if(n > Sieb.length()){
    	lint border = static_cast<lint>(sqrt(n));
	for(lint i = 2; i <= border + 1; i++){
	    //std::cout << "::: " << n << ", " << border << ", " <<  i << " mod: " << (n % i)  << std::endl; 
	    if(n % i == 0){
		return i;
	    }
	}    
	return 1;
    }
    if(!Sieb[n]){
        lowestFactor++;
        for(; n % lowestFactor != 0; lowestFactor++){
        } 
    } 
    return lowestFactor;
}


// Miller-Rabin


// Hilfsfunktionen

lint fastpow(lint nBase, lint nExp, lint nMod){
	//std::cout << nBase << ", " << nExp << ", " << nMod << std::endl;
	lint result = 1;
	while(nExp != 0){
		lint currentBit = nExp & 1;
		if(currentBit == 1){
			result = (result * nBase) % nMod;
		}
		//std::cout << "exponent: " << nExp << ", " << result << ", " << nBase << std::endl;
		nExp = nExp >> 1;
		nBase = (nBase * nBase) % nMod;
	}
	//std::cout << result << "WTFFFFFF" << std::endl;
	return result;
}




lint MillerRabinTest(lint n, lint a, lint d, lint r){ 
	return fastpow(a, ((lint) pow(2,r)) * d, n);
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

void get_ds(lint n, lint& d, lint& s){
	s = 1;
	for(lint r = 1;; r++){
		//std::cout << "n: " << n << ", r: " << r << ", ---: " << (n-1) % ((lint) pow(2, r)) << std::endl;
		if(pow(2, r) > n){
			break;
		}
		if((n - 1) % (lint) pow(2, r) == 0){
			s = r;
		}
	} 
	d = (n-1)/pow(2, s);
	return; 
}

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
        //std::cout << "-- testing value: " << testValue << std::endl;
        TestErgebnis(Probe, i, PrimTest(testValue, Sieb)); 
    }   
    
    std::cout << "fastpow(4, 24, 123): " << fastpow(4, 24, 123) << std::endl;
    //std::cout << "gcd(12345, 5245): " << gcd(12345, 5245) << std::endl;
    // Fermat-Test
    
    std::cout << "testint Fermtat...." << std::endl;
    for(unsigned int i = 1; i <= AnzahlBeispiele; i++){
        lint testValue = HoleKandidaten(Fermat, i);
        //std::cout << "-- testing value: " << testValue << std::endl;
        bool saveNoPrime = false;
	for(lint j = 2; j < MaxTestPrimzahl; j++){
	    if(Sieb[j] && testValue % j != 0){
	    	lint result = FermatTest(testValue, j);
            	//std::cout << "fermat result" << result << std::endl;
                //std::cout << "inner: bsp = " << i << " ( = " << testValue << ") , testing: " << j << " with result: " << result << std::endl; 
		TestErgebnis(Fermat, i, result, j);
		if(result != 1){
		     saveNoPrime = true;
		     break;
		}
	    }
	}
	//std::cout << "inner end.. final result:" << std::endl;
	TestErgebnis(Fermat, i, saveNoPrime ? 0 : 1, 0);
	//std::cout << "proceeding to next example..." << std::endl;	
    }
	//std::cout << "test calculation....." << std::endl;
	//std::cout << "fastpow(2, 560, 561): " << fastpow(2, 560, 561) << std::endl;
	// Miller-Rabin-Test
	
	

	std::cout << "testing miller rabin....... max: " << MaxTestPrimzahl << std::endl;
	for(unsigned int i = 1; i < AnzahlBeispiele; i++){
		lint testValue = HoleKandidaten(MillerRabin, i);
		bool maybePrime = true;
		bool proof = false;
		//std::cout << "testing: " << testValue << std::endl;
		for(lint j = 1; j <= MaxTestPrimzahl; j++){
			if(Sieb[j] && gcd(j, testValue) == 1){
				//std::cout << "working with value " << j << "....." << std::endl;
				lint d = 0;
				lint s = 0;
				get_ds(testValue, d, s);
				//std::cout << "s: " << s << " , d: " << d << std::endl;
				if(fastpow(j, d, testValue) == 1){
					proof = true;
				} 
				else {
					for(lint r = 0; r <= s - 1; r++){
						lint result = MillerRabinTest(testValue, j, d, r); 
						//std::cout << "--- r: " << r << ", result: " << result << std::endl;	
						//TestErgebnis(MillerRabin, i, result, j, r);
						if(result == testValue - 1){
							proof = true;
						}
					}
				} 
				//std::cout << "proof after j = " << j << ": " << proof << std::endl;
				maybePrime = maybePrime & proof;
				proof = false;
				
			}
			
		}
		//std::cout << maybePrime << std::endl;
		TestErgebnis(MillerRabin, i, maybePrime ? 1 : 0, 0); 
	}

	//std::cout << fastpow(3, 1500955143, 3001910287) << std::endl;
	return 0;
}

