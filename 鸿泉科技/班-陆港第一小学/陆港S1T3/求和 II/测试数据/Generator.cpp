#include "HQCG.h"
// STEP 2 : Modify the cases number
//|---------------------------------------------|
	int minCaseNumber = 8, maxCaseNumber = 8;   //Test Cases Number
//|---------------------------------------------|

void solve() {
//STEP 1 : std Code copy here and replace "cin" and "cout" with generator
//|---------------------------------------------|
	int n, m;
	n = intRand(1, 1e6);
	m = intRand(1, 1e6);
	iprint(n);
	iprint(m);
	long long s1 = 0, s2 = 0;
	for (int i = 1; i <= n; i++) s1 += i;
	for (int i = 1; i <= m; i++) s2 += i;
	oprint(s1 - s2);
//|---------------------------------------------|
}
int main() {
// STEP 3 : Modify the cases name
//|---------------------------------------------|
	init(minCaseNumber, maxCaseNumber, "sum");
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
