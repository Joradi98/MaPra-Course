#include <iostream>

#include "unit.h"

using namespace std;



/*
Bubble-Sort (in place)
*/
void bubbleSort(unsigned int *&array, size_t length) {

	// Iterate over all elements
	int i, j;
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
Selection-Sort
*/
void selectionSort(unsigned int *&array, size_t length) {
	int i, k, t, minIndex; 
	
	// Iterate over all elements
	for (i = 0 ; i < ( length - 1 ); i++) {
		minIndex = i;
		// Determine minimum of all following elements
		for( k = i+1; k < length; k++) {
			if( array[k] < array[minIndex]) minIndex = k;
		}

		// Then swap the current element with the minimum Elemenr
		tausche(array, i, minIndex);
	}
}


/*
Insertion-Sort
*/
void insertionSort(unsigned int *&array, size_t length) {
	int i, j;

	// Iterate through the array
	for (i = 1 ; i < length; i++) {
		// Now take a look at the sub-array that consists of all elements up to the i-th element
		j = i;
	 
		// "Insert" the i-th element at the right position in that sub-array
		while ( j > 0 && array[j-1] > array[j]) {
			tausche(array, j-1, j);
			j--;
		}
	}


}


/*
Helper method: Determine is a string displays an integer
*/
bool isInteger(string s){
	for (int i = 0; i < s.size(); i++){
		if(!isdigit(s[i])) return false;
	}
	return true;
}


/*
Helper method: print an array of unsigned integers
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

		unsigned int *array = new unsigned int[6];
		size_t length = 6;

		// Use mode 2 in order to receive a random array of specified length
		start(2, length, array);
		bubbleSort(array, length);
		cout << "Bubble Sort: ";
		printArray(array, length);
		
		start(2, length, array);
		selectionSort(array, length);
		cout << "Selection Sort: ";
		printArray(array, length);

		start(2, length, array);
		insertionSort(array, length);
		cout << "Insertion Sort: ";
		printArray(array, length);

		
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
