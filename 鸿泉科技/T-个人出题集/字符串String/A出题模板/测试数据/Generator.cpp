#include "HQCG.h"
// STEP 2 : Modify the cases number
//|---------------------------------------------|
	int minCaseNumber = 1, maxCaseNumber = 1;   //Test Cases Number
//|---------------------------------------------|

void solve() {
//STEP 1 : std Code copy here and replace "cin" and "cout" with generator
//|---------------------------------------------|
	
//|---------------------------------------------|
}
int main() {
// STEP 3 : Modify the cases name
//|---------------------------------------------|
	init(minCaseNumber, maxCaseNumber, "test");
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
