#include "HQCG.h"
// STEP 2 : Modify the cases number
//|---------------------------------------------|
	int minCaseNumber = 6, maxCaseNumber = 10;   //Test Cases Number
//|---------------------------------------------|

void solve() {
//STEP 1 : std Code copy here and replace "cin" and "cout" with generator
//|---------------------------------------------|
	long long n, pro = 1, sum = 0;
	n = intRand(15, 17);
	string s;
	for (int i = 0; i < n; i++) {
		int ppp = intRand(1, 2);
		if (ppp == 1) s  += '8';
		else s += '9';
	}
	n = stoll(s);
	iprint(n);
	while (n) {
		pro *= n % 10;
		sum += n % 10;
		n /= 10;
	}
	oprint(pro - sum);
//|---------------------------------------------|
}
int main() {
// STEP 3 : Modify the cases name
//|---------------------------------------------|
	init(minCaseNumber, maxCaseNumber, "sub");
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
