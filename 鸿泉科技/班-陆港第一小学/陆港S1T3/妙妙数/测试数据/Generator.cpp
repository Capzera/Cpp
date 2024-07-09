#include "HQCG.h"
// STEP 2 : Modify the cases number
//|---------------------------------------------|
	int minCaseNumber = 1, maxCaseNumber = 8;   //Test Cases Number
//|---------------------------------------------|
int a[200010];
void solve() {
//STEP 1 : std Code copy here and replace "cin" and "cout" with generator
//|---------------------------------------------|
	int n, k;
	k = intRand(1, 1e4);
	n = intRand(k, 1e4);
	iprint(n); iprint(k, '\n');
	int m = (n - k) / 2;
	for (int i = 1; i <= n; i++) a[i] = intRand(1, 1e6), iprint(a[i]);
	oprint(a[k + m]);
//|---------------------------------------------|
}
int main() {
// STEP 3 : Modify the cases name
//|---------------------------------------------|
	init(minCaseNumber, maxCaseNumber, "good");
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
