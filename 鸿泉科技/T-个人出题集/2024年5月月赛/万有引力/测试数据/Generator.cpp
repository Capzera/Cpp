#include "HQCG.h"
// STEP 2 : Modify the cases number
//|---------------------------------------------|
	int minCaseNumber = 1, maxCaseNumber = 1;   //Test Cases Number
//|---------------------------------------------|

void solve() {
//STEP 1 : std Code copy here and replace "cin" and "cout" with generator
//|---------------------------------------------|
	
	int n, mx = -1;
	n = intRand(9e2, 1e3);
	iprint(n, '\n');
	vector<int> a(n);/*
	a[0] = intRand(1, 1e6);
	int k1 = intRand(n / 6, n / 5);
	int k2 = intRand(n / 6, n / 5);
	for (int i = 0; i < k1; i++) a[i] = a[0];
	for (int i = n - k2 + 1; i < n; i++) a[i] = a[0];
	for (int i = k1; i <= n - k2; i++) a[i] = intRand(1, 1e6);*/
	a[0] = intRand(1, 1e3);
	for (int i = 1; i < n; i++) a[i] = a[0];
	iprint(a);
	for (int i = 0; i < n; i++) {
		if (a[i] != a[0]) {
			mx = max(mx, i);
		}
		if (a[n - 1] != a[i]) {
			mx = max(mx, n - 1 - i);
		}
	}
	oprint(mx);

	
//|---------------------------------------------|
}
int main() {
// STEP 3 : Modify the cases name
//|---------------------------------------------|
	init(minCaseNumber, maxCaseNumber, "gravity");
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
