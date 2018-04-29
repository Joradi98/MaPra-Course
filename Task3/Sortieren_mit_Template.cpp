#include <iostream>
#include "unit.h"
#include <stdio.h>

using namespace std;


template<typename ElemT>
void swap(std::vector<ElemT>& feld, int i, int j) {
	ElemT at_j = feld[j];
	feld[j] = feld[i];
	feld[i] = at_j;
}


/*
Helper function for Merge-Sort:
	Merges two sorted sub-arrays into a combined sorted array
	First subarray is arr[l..m]
	Second subarray is arr[m+1..r]
*/
void merge(unsigned int *&array, int l, int m, int r)
{
	/*int i, j, k;
	
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
	}*/
	
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
template<typename ElemT>
void bubbleSort(std::vector<ElemT>& feld) {
	typename std::vector<ElemT>::size_type minIndex, i, j;
	size_t length = feld.size();

	
	for(i = 0; i != length; i++) {
		// Iterate from the end of the array down to i + 1
		for(j = length - 1; j > i; j--) {
			if (feld[j] < feld[j-1]) {
				swap(feld, j-1, j);
			}
		}
	}

}


/*
Selection-Sort
*/
template<typename ElemT>
void selectionSort(std::vector<ElemT>& feld, size_t length) {
	
	typename std::vector<ElemT>::size_type minIndex, i, k;
	// Iterate over all elements
	for(i = 0; i != feld.size(); i++) {
		minIndex = i;
		// Determine minimum of all following elements
		for(k = i+1; k != feld.size(); k++) {
			if (feld[k] < feld[minIndex]) minIndex = k;
		}
		
		// Then swap the current element with the minimum Element
		swap(feld, i, minIndex);
		
	}	    
}




/*
Helper method: Determine if a string displays an integer
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
template<typename ElemT>
void printVector(std::vector<ElemT>& feld) {
	// Iterate and print values of vector
	for(int element : feld) {
		std::cout << element << ", ";
	}
	std::cout << endl;
}

void waitForUserToContinue() {
	getchar();
}

int main(int argc, char *argv[]) {
    cout << "Welcome to Task 2b" << endl;
    cout << "Here you can not only sort unsigned int, but also many other types!" << endl;

    
    // Create a vector containing integers
	std::vector<int> v = {7, 5, 16, 8};
 
	// Add two more integers to vector
	v.push_back(25);
	v.push_back(13);
	bubbleSort(v);
  
	printVector(v);

	return 0;

}
