#include "HQCG.h"
// STEP 2 : Modify the cases number
//|---------------------------------------------|
	int minCaseNumber = 1, maxCaseNumber = 7;   //Test Cases Number
//|---------------------------------------------|

void solve() {
//STEP 1 : std Code copy here and replace "cin" and "cout" with generator
//|---------------------------------------------|
	long long n, k, m;
	double t;
	n = intRand(8e2, 1e3);
	k = intRand(8e2, 1e3);
	t = intRand(1, 10);
	t /= 10;
	m = intRand(1, 1e2);
	iprint(n); iprint(k); iprint(t), iprint(m);
	n *= k;
	long long bad = n * t;
	n -= bad;
	oprint(n / m); oprint(n % m);
//|---------------------------------------------|
}
int main() {
// STEP 3 : Modify the cases name
//|---------------------------------------------|
	init(minCaseNumber, maxCaseNumber, "apple");
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
