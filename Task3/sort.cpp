#include <iostream>
#include "unit.h"
#include <stdio.h>
#include <fstream>

using namespace std;


template<typename ElemT>
void swap(std::vector<ElemT>& feld, int i, int j) {
	ElemT at_j = feld[j];
	feld[j] = feld[i];
	feld[i] = at_j;
}


template<typename ElemT>
void einlesen(std::ifstream& ifs, std::vector<ElemT>& feld){
    while(!ifs.eof()){
        ElemT element; 
        ifs >> element;
        if(ifs.eof()) break;
        feld.push_back(element);
        //cout << "student: " << element << endl;
    }
    ifs.close();
}


/*
Helper function for Merge-Sort:
	Merges two sorted sub-arrays into a combined sorted array
	First subarray is arr[l..m]
	Second subarray is arr[m+1..r]
*/
template<typename ElemT>
void merge(vector<ElemT> array, int l, int m, int r)
{
	int i, j, k;
	
	int n1 = m - l + 1;
	int n2 =  r - m;
 
	// Temporary Arrays for storing the left and right arrays (each of them is already sorted)
	vector<ElemT> L = {};
    vector<ElemT> R = {};
 
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
template<typename ElemT>
void mergeSort(vector<ElemT> vector, int l, int r) {
	if (l < r) {
		// Same as (l+r)/2, but avoids overflow for
		// large l and h
		int m = l+(r-l)/2;
 
		// Sort first and second halves
		mergeSort(vector, l, m);
		mergeSort(vector, m+1, r);
 
		// Now merge these subarrays in the right order
		merge(vector, l, m, r);
	}
}




/*
Bubble-Sort (in place)
*/
template<typename ElemT>
void bubbleSort(std::vector<ElemT>& feld) {
	size_t length = feld.size();

	
	for(int i = 0; i != length; i++) {
		// Iterate from the end of the array down to i + 1
		for(int j = length - 1; j > i; j--) {
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
void selectionSort(std::vector<ElemT>& feld) {
	for(int i = 0; i != feld.size(); i++) {
		int minIndex = i;
		// Determine minimum of all following elements
		for(int k = i+1; k != feld.size(); k++) {
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
Helper method: print an vector of ElemT's
*/
template<typename ElemT>
void printVector(std::vector<ElemT>& feld) {
	// Iterate and print values of vector
	for(typename std::vector<ElemT>::iterator it = feld.begin(); it != feld.end(); ++it){
        std::cout << *it << endl;
    }
}

void waitForUserToContinue() {
	getchar();
}


int main(int argc, char *argv[]) {
    cout << "Welcome to Task 2b" << endl;
    cout << "Here you can not only sort unsigned int, but also many other types!" << endl;
    cout << "which sortg algorithm you want to test? (bubble, selection, merge)" << endl;
    /*
    // Create a vector containing integers
	std::vector<int> v = {7, 5, 16, 8};
 
	// Add two more integers to vector
	v.push_back(25);
	v.push_back(13);
	bubbleSort(v);
  
	//printVector(v);
    */
    
    
    std::ifstream ifs1 ("strings.txt", std::ifstream::in);
    std::vector<std::string> v1 = {};
    einlesen(ifs1, v1);
    
    std::ifstream ifs2 ("doubles.txt", std::ifstream::in);
    std::vector<double> v2 = {};
    einlesen(ifs2, v2);
    

    std::ifstream ifs3 ("studenten.txt", std::ifstream::in);
    std::vector<Student> v3 = {};
    einlesen(ifs3, v3);
    
    string sortType;
    cin >> sortType;
    
    
    if(sortType == "bubble"){
        bubbleSort(v1);
        bubbleSort(v2);
        bubbleSort(v3);
    }
    else if(sortType == "selection"){
        selectionSort(v1);
        selectionSort(v2);
        selectionSort(v3);
    }
    else if(sortType == "merge"){
        mergeSort(v1, 0, v1.size() - 1);
        mergeSort(v2, 0, v2.size() - 1);
        mergeSort(v3, 0, v3.size() - 1);
    }
    else {
        cout << "not a valid sorting algorithm. exiting.." << endl;
    }

    
    if(!(ergebnis(v1) || ergebnis(v2) || ergebnis(v3))){
        cout << "everything fine!" << endl;
    }
    else {
        cout << "something wrong." << endl;
    }

	return 0;

}
