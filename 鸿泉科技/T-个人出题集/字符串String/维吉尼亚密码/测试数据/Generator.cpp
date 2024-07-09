#include "HQCG.h"
// STEP 2 : Modify the cases number
//|---------------------------------------------|
	int minCaseNumber = 1, maxCaseNumber = 10;   //Test Cases Number
//|---------------------------------------------|

void solve() {
//STEP 1 : std Code copy here and replace "cin" and "cout" with generator
//|---------------------------------------------|
	string s1, s2;// n0100
	s1 = stringRand(intRand(1, 1000), 0, 1, 0, 0);
	s2 = stringRand(intRand(1, s1.size() / 2), 0, 1, 0, 0);
	iprint(s1, '\n');
	iprint(s2, '\n');
	int n = s1.size(), m = s2.size();
	for (int i = 0, j = 0; i < n; i++, j = (j + 1) % m) {
		s1[i] = (s1[i] - 'a' + s2[j] - 'a') % 26 + 'a';
	}
	oprint(s1);
//|---------------------------------------------|
}
int main() {
// STEP 3 : Modify the cases name
//|---------------------------------------------|
	init(minCaseNumber, maxCaseNumber, "virginia");
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
