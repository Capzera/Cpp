#include "HQCG.h"
// STEP 2 : Modify the cases number
//|---------------------------------------------|
	int minCaseNumber = 4, maxCaseNumber = 10;   //Test Cases Number
//|---------------------------------------------|
long long a[2005][2005];
void solve() {
//STEP 1 : std Code copy here and replace "cin" and "cout" with generator
//|---------------------------------------------|
	for (int i = 0; i < 2005; i++) {
		memset(a[i], 0, sizeof(a[i]));
	}
	long long n, m, q, x, r1, r2, c1, c2;
	n = intRand(900, 1000);
	m = intRand(900, 1000);
	q = intRand(9e4, 1e5);
	iprint(n), iprint(m), iprint(q, '\n');
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			x = intRand(1, 1e6, 0.5);
			iprint(x);
			a[i][j] = a[i - 1][j] + a[i][j - 1] - a[i - 1][j - 1] + x;
		}
		iprint();
	}
	while (q--) {
		r1 = intRand(1, n / 8);
		r2 = intRand(n * 4 / 5, n);
		c1 = intRand(1, m / 8);
		c2 = intRand(m * 4 / 5, m);
		iprint(r1), iprint(c1), iprint(r2), iprint(c2, '\n');
		oprint(a[r2][c2] - a[r2][c1 - 1] - a[r1 - 1][c2] + a[r1 - 1][c1 - 1], '\n');
	}
//|---------------------------------------------|
}
int main() {
// STEP 3 : Modify the cases name
//|---------------------------------------------|
	init(minCaseNumber, maxCaseNumber, "2dpresum");
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
