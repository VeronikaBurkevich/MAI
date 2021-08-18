#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
//#include <ctime>

long long int MAX_DIVIDER = 1000000000000000000;
long long int DIVIDER_N=0;
long long int DIVIDER_M=0;
using namespace std;
int main(){
	long long int n, m;
	cin >> n >> m;
	long long int counter=0;

	//time_t start, end;
	//start = clock();

	while (1) {
		if ((n >= MAX_DIVIDER) && (DIVIDER_N == 0)) {
			DIVIDER_N = MAX_DIVIDER;
		}
		if ((m >= MAX_DIVIDER) && (DIVIDER_M == 0)) {
			DIVIDER_M = MAX_DIVIDER;
		}
		if ((DIVIDER_M != 0) && (DIVIDER_N != 0)) {
			DIVIDER_N /= DIVIDER_M;
			break;
		}
		MAX_DIVIDER /= 10;
	}
	if (m==1){
		while (DIVIDER_N!=0){
			counter+=n/DIVIDER_N-(DIVIDER_M-1);
			DIVIDER_M *= 10;
			DIVIDER_N /= 10;
		}
	}
	else{
		while (DIVIDER_N != 0) {
			counter += ((n/DIVIDER_N)-((DIVIDER_M/m)*m))/m;
			if (DIVIDER_M%m==0){
				++counter;
			}
			DIVIDER_M *= 10;
			DIVIDER_N /= 10;
		}
	}
	if (n%m == 0) {
		if(counter){
		--counter;
		}
	}
	
	//end = clock();
	cout << counter << endl;
	//cout << "time: " << end << "-" << start << endl;

}