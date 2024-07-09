#include "HQCG.h"
// STEP 2 : Modify the cases number
//|---------------------------------------------|
	int minCaseNumber = 1, maxCaseNumber = 1;   //Test Cases Number
//|---------------------------------------------|

void solve() {
//STEP 1 : std Code copy here and replace "cin" and "cout" with generator
//|---------------------------------------------|
	int n;
	long long a[100];
	n = intRand(1, 3);
	iprint(n);
	if (n == 1) {
		oprint(1); 
		return;
	}
	if (n == 2) {
		oprint(1); 
		return;
	}
	if (n == 3) {
		oprint(1);
		return;
	}
	a[1] = a[2] = a[3] = 1;
	for (int i = 4; i <= n; i++) {
		a[i] = a[i - 1] + a[i - 2] + a[i - 3];
	}
	oprint(a[n]);	
//|---------------------------------------------|
}
int main() {
// STEP 3 : Modify the cases name
//|---------------------------------------------|
	init(minCaseNumber, maxCaseNumber, "fib");
	InputFileSuffix = "in";
	OutputFileSuffix = "out";
//|---------------------------------------------|
	for (int i = minCaseNumber; i <= maxCaseNumber; i++) {
		cout << fixed << setprecision(3);
		caseNumber = i;
		solve();
		filePrint();
	}
	return 0; 
} 
