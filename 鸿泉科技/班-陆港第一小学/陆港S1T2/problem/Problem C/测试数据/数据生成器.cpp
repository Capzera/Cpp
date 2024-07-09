#include "random.h"
// STEP 2 : Modify the cases number
//|---------------------------------------------|
	int minCaseNumber = 9, maxCaseNumber = 10; //Test Cases Number
//|---------------------------------------------|
void solve(int number) {
	caseNumber = number;
	initcase();
//STEP 1 : std Code copy here and replace "cin" and "cout" with generator
//|---------------------------------------------|
	int n, x, ans = -1, mx = INT_MIN;
	n = intRand(1e5, 3e5);	
	iprint(n, '\n');
	for (int i = 1; i <= n; i++) {
		x = intRand(1, 1e6, 1);
		iprint(x);
		if (x >= mx) {
			mx = x;
			ans = i;
		}
	}
	oprint(ans);
//|---------------------------------------------|
}
int main() {
// STEP 3 : Modify the cases name
//|---------------------------------------------|
	init(minCaseNumber, maxCaseNumber, "index");
	InputFileSuffix = "in";
	OutputFileSuffix = "out";
//|---------------------------------------------|
	for (int i = minCaseNumber; i <= maxCaseNumber; i++) {
		solve(i);
	}
	return 0; 
} 
