#include "HQCG.h"
// STEP 2 : Modify the cases number
//|---------------------------------------------|
	int minCaseNumber = 1, maxCaseNumber = 10;   //Test Cases Number
//|---------------------------------------------|

void solve() {
//STEP 1 : std Code copy here and replace "cin" and "cout" with generator
//|---------------------------------------------|
	int n, k;
	n = intRand(1, 100);
	k = intRand(0, 100 - n);
	iprint(n), iprint(k);
	n += k;
	oprint(n);
	if (n > 89) oprint('A');
	else if (n > 74) oprint('B');
	else if (n > 59) oprint('C');
	else oprint('D');
//|---------------------------------------------|
}
int main() {
// STEP 3 : Modify the cases name
//|---------------------------------------------|
	init(minCaseNumber, maxCaseNumber, "score");
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
