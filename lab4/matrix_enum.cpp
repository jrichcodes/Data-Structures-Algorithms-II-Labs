//File: matrix_enum.cpp
//Name: Josephine Rich 
//Date: 2/27/2022

//Description: This program takes in the 3 arguments W, E, and (x or h). There
//are exactly W elements with X's and E elements with E's in a square W matrix.
//This program does a two-level enumeration to find all the possible matrcies.
//X or h is just the printing mode to print all the matrices. 

#include <iostream>
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <sstream>
#include <string>
using namespace std;

class Matrix {
	public:
		int W;
		int E;
		char P; /*'x' or 'h' */
		vector <int> Perm; /*Permutation of 0 .. (W-1), for the 'X' elements*/
		vector <int> Non_X; /*row/col id of each of the non-X elements*/
		vector <int> E_ID;  /*row/col id of the E elements*/
		void Print();
		void Permute(int index); /*recursive permuting method*/
		void Choose(int index, int numE);  /*recursive n-choose-k method*/
		int calculate(int exponent); /*calculates 2^i for the h print method*/
};

int Matrix::calculate(int exponent) {
	int result = 1;
	
	while(exponent != 0) {
		result *= 2;
		--exponent;
	}

	return result;
}


void Matrix::Permute(int index) {
	size_t i;
	int temp, num, j;
	
	if(index == (int)Perm.size()) { /*base case: there are no more elements to permuate*/

		for(i=0; i < Perm.size(); i++) { /*looping thourgh each possible permuationi for the 'X' elements*/
			for(j=0; j < W; j++){ /*looping thourgh each element of each permuation*/
				if(j != Perm[i]) { /*finding the non_X elements and pushing onto the vector*/
					num = i * W + j;
					Non_X.push_back(num);
				}
			}
		}

		/*finished finding all the non_x elements of the permutation*/
		Choose(0, E); /*calling choose to do the second level of enumeration*/
		Non_X.clear();

		return;
	}

	for(i = index; i < Perm.size(); i++) {
		temp = Perm[i];        /*swap Perm[index] with Perm[i]*/
		Perm[i] = Perm[index];
		Perm[index] = temp;

		Permute(index+1);

		temp = Perm[i];      /*swap back*/
		Perm[i] = Perm[index];
		Perm[index] = temp;
	}
}

void Matrix::Choose(int index, int numE) {

	if(numE == 0) { /*first base case: no more E's to choose*/
		Print();
		return;
	}

	if(numE > (int)Non_X.size() - index) return; /*second base case: there are fewer E's left to add than there are open spots to put them*/
												/*therefore, it's impossible to finish -- this helps with run time*/
	E_ID.push_back(Non_X[index]); /*putting the open spot on the matrix onto the E_ID vector*/
	
	Choose(index+1, numE-1); /*recursively call choose*/
	E_ID.pop_back(); /*taking the open spot off of the E_ID vector*/

	Choose(index+1, numE);

}

void Matrix::Print() {
	int i, j;
	int num, count = 0;
	vector<int>::const_iterator eit;

	if(P == 'x') {
		for(i=0; i < W; i++) {
			for(j=0; j < W; j++) {
				if(j == Perm[i]) cout << "X"; /*checking where to put Xs*/	
				else {
					num = (i * W) + j;
					eit = find (E_ID.begin(), E_ID.end(), num); /*checking where to put Es*/
					if(eit != E_ID.end()) cout << "E";
					else cout << ".";
				}
			}
			cout << endl;
		}
		cout << endl;
	}

	if(P == 'h') {
		for(i=0; i < W; i++) {
			for(j=0; j < W; j++) {
				num = (i * W) + j;
				eit = find (E_ID.begin(), E_ID.end(), num); /*calculating the hex rep of each line of the matrix*/
				if((j == Perm[i]) || (eit != E_ID.end())) count += calculate(j);
				else count += 0;
			}
			cout << hex << count << endl;
			count = 0;
		}
		cout << endl;
	}
}

int main (int argc, char *argv[]) {
	
	Matrix m;
	string letter;
	int i;

	if(argc != 4) printf("No enough commandline arguemnts -> W E (x or h)\n"); /*error checking input*/
	
	m.W = atoi(argv[1]);
	m.E = atoi(argv[2]);
	m.P = *argv[3];

	for(i=0; i < m.W; i++) {
		m.Perm.push_back(i);
	}

	m.Permute(0);


	return 0;
}
