#include <iostream>

#include "unit.h"

using namespace std;


/*
Healper function for Heap-Sort:
*/
// To heapify a subtree rooted with node i which is
// an index in arr[]. n is size of heap
void heapify(unsigned int *&array, size_t length, int i)
{
	int largest = i;  // Initialize largest as root
	int l = 2*i + 1;  // left = 2*i + 1
	int r = 2*i + 2;  // right = 2*i + 2
 
	// If left child is larger than root
	if (l < length && array[l] > array[largest])
		largest = l;
 
	// If right child is larger than largest so far
	if (r < length && array[r] > array[largest])
		largest = r;
 
	// If largest is not root
	if (largest != i)
	{
		tausche(array, i, largest);

		// Recursively heapify the affected sub-tree
		heapify(array, length, largest);
	}
}
 


/*
Heap-Sort
*/
void heapSort(unsigned int *&array, int n)
{
	// Build heap
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(array, n, i);
 
	// One by one extract an element from heap
	for (int i=n-1; i>=0; i--) {
		// Move current root to end
		tausche(array, 0, i);
 
		// call max heapify on the reduced heap
		heapify(array, i, 0);
	}
}



/*
Helper function for Merge-Sort:
	Merges two sorted sub-arrays into a combined sorted array
*/
// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(unsigned int *&array, int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 =  r - m;
 
	// Temporary Arrays for storing the left and right arrays (each of them is already sorted)
	int L[n1], R[n2];
 
	// Copy data 
	for (i = 0; i < n1; i++)
		L[i] = array[l + i];
	for (j = 0; j < n2; j++)
		R[j] = array[m + 1+ j];
 
	// Start the merging process of L and R back into the real
	i = 0; // Index for the left sub-array
	j = 0; // Index for the right sub-array
	k = l; // Merging starts at index l of the original array
	while (i < n1 && j < n2) {
		// Compare the first elements of L and R subarrays and copy the smaller one to the original array
		if (L[i] <= R[j]) {
			array[k] = L[i];
			i++; // Advance the index of the left-subarray cause we have just taken an element from it
		} else {
			array[k] = R[j];
			j++; // Advance the index of the right-subarray cause we have just taken an element from it
		}
		// Whichever sub-array we took an element from, advance the index of the original array
		k++;
	}
 
	// Copy the remaining elements of L[], if there are any. 
	while (i < n1) {
		array[k] = L[i];
		i++;
		k++;
	}
 
	// Copy the remaining elements of R[], if there  are any. This may be the case, when the array is already completely sorted [1,2],[3,4]
	while (j < n2) {
		array[k] = R[j];
		j++;
		k++;
	}
	
}





/*
Merge-Sort:
	l is for left index and r is right index of the sub-array of arr to be sorted
*/
void mergeSort(unsigned int *&array, int l, int r) {
	if (l < r) {
		// Same as (l+r)/2, but avoids overflow for
		// large l and h
		int m = l+(r-l)/2;
 
		// Sort first and second halves
		mergeSort(array, l, m);
		mergeSort(array, m+1, r);
 
		// Now merge these subarrays in the right order
		merge(array, l, m, r);
	}
}




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
		
		start(2, length, array);
		mergeSort(array, 0,length-1);
		cout << "Merge Sort: ";
		printArray(array, length);

		
		start(2, length, array);
		heapSort(array, length);
		cout << "Heap Sort: ";
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
