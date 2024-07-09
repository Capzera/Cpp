#include "random.h"
// STEP 2 : Modify the cases number
//|---------------------------------------------|
	int minCaseNumber = 1, maxCaseNumber = 1; //Test Cases Number
//|---------------------------------------------|
void solve(int number) {
	caseNumber = number;
//STEP 1 : std Code copy here and replace "cin" and "cout" with generator
//|---------------------------------------------|
	string s = highIntRand("12345678","87654321");
	iprint(s);
//|---------------------------------------------|
}
int main() {
// STEP 3 : Modify the cases name
//|---------------------------------------------|
	init(minCaseNumber, maxCaseNumber, "TEST");
	InputFileSuffix = "in";
	OutputFileSuffix = "out";
//|---------------------------------------------|
	for (int i = minCaseNumber; i <= maxCaseNumber; i++) {
		solve(i);
	}
	return 0; 
} 
