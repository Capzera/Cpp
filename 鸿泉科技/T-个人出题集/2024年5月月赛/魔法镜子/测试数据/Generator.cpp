#include "HQCG.h"
// STEP 2 : Modify the cases number
//|---------------------------------------------|
	int minCaseNumber = 8, maxCaseNumber = 10;   //Test Cases Number
//|---------------------------------------------|

void solve() {
//STEP 1 : std Code copy here and replace "cin" and "cout" with generator
//|---------------------------------------------|
	int n = intRand(8000, 10000), k = intRand(n / 5, n / 4);
	string s = stringRand(n, 1, 0 ,0,0);
	iprint(n);
	iprint(k, '\n');
	iprint(s);
	for (int i = 0; i < n && k; i++) {
		if (s[i] != '6' && s[i] != '0' && s[i] != '3' && s[i] != '1') continue;
		if (s[i] == '6') s[i] = '9';
		if (s[i] == '1') s[i] = '7';
		if (s[i] == '0' || s[i] == '3') s[i] = '8';
		k--;
	}
	oprint(s);
//|---------------------------------------------|
}
int main() {
// STEP 3 : Modify the cases name
//|---------------------------------------------|
	init(minCaseNumber, maxCaseNumber, "mirror");
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
