#include <iostream>

#include "unit.h"

using namespace std;



/*
Bubble-Sort (in place)
*/
void bubbleSort(unsigned int *&array, size_t length) {

	// Iterate over all elements
	int i, j, swap;
	for (i = 0 ; i < ( length - 1 ); i++) {
		
		// Iterate from the end of the array down to i + 1
		for (j = length - 1 ; j > i; j--) {
			if (array[j-1] > array[j])	{
 				// Swap the elements at i and j
				tausche(array, j-1, j);
			}
		}
	}
	
}



/*
Helper method
*/
bool isInteger(string s){
	for (int i = 0; i < s.size(); i++){
		if(!isdigit(s[i])) return false;
	}
	return true;
}


/*
Helper method
*/
void printArray(unsigned int* array, size_t length) {
	int i;
	for (i = 0; i < length - 1; i++) {
		cout << array[i] << ", ";
	}
	cout << array[length - 1] << endl;
}


int main(int argc, char *argv[]) {
	cout << "What would you like to do?" << endl;
	cout << "  manual: manually specify coefficients" << endl;
	cout << "  1-" << 5 << ":   test n-th polynomial from unit.o" << endl;
	cout << "  all:    test all polynomials from unit.o" << endl;
	string command;    
	cin >> command;
	double a, b, c, d, e;
	if(command == "manual"){
		cout << "specify values for the coefficients:" << endl;
		cout << "a: "; cin >> a;
		cout << "b: "; cin >> b;
		cout << "c: "; cin >> c;
		cout << "d: "; cin >> c;
		cout << "e: "; cin >> c;

	}
	else if(command == "all"){

		unsigned int *array = new unsigned int[10];
		array[0] = 3;
		array[1] = 22;
		array[2] = 3;
		array[3] = 45;
		array[4] = 100;
		array[5] = 1;
		
		bubbleSort(array, 6);
		printArray(array, 6);
		delete[] array;
	}
	else if(isInteger(command)){
		cout << "Specified an integer" << endl;
	}
	else {
		cout << "unknown command. exiting.." << endl;
	}
	return 0;

}
