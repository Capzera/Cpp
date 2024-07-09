#include "HQCG.h"
// STEP 2 : Modify the cases number
//|---------------------------------------------|
int minCaseNumber = 9, maxCaseNumber = 9;   //Test Cases Number
//|---------------------------------------------|
int a[200005];
void solve() {
//STEP 1 : std Code copy here and replace "cin" and "cout" with generator
//|---------------------------------------------|

	long long n, k, l, r, maxAns = 0, sum = 0, ans, slide = 0;
	n = intRand(9e4 + 1e5, 2e5);
	k = intRand(n * 3 / 4, n * 4 / 5);
	int ansL = intRand(1, n - k);
	int ansR = ansL + k;
	iprint(n);
	iprint(k, '\n');
	for (int i = 1, j = 1; i <= n; i++) {
		if (i >= ansL && i <= ansR) a[i] = intRand(100, 200);
		else a[i] = intRand(1, 10);
		if (i == ansR) a[i] += intRand(7e7, 8e7);
		iprint(a[i]);
		slide += a[i];
		if (i > k) {
			if (slide > maxAns) {
				maxAns = slide;
				l = j, r = i;
				ans = l;
				sum = 0;
				for (; l < r; l++) {
					sum += a[l];
					if (sum < a[l + 1]) {
						ans = l + 1;
					}
				}
			}
			slide -= a[j++];

		}
	}
	oprint(maxAns);
	oprint(ans);

//|---------------------------------------------|
}
int main() {
// STEP 3 : Modify the cases name
//|---------------------------------------------|
	init(minCaseNumber, maxCaseNumber, "");
	InputFileSuffix = "in";
	OutputFileSuffix = "out";
//|---------------------------------------------|
	for (int i = minCaseNumber; i <= maxCaseNumber; i++) {
		caseNumber = i;
		solve();
		filePrint();
	}
	return 0;
}
