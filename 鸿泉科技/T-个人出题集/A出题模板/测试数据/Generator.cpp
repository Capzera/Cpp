#include "HQCG.h"
// STEP 2 : Modify the cases number
//|---------------------------------------------|
	int minCaseNumber = 6, maxCaseNumber = 10;   //Test Cases Number
//|---------------------------------------------|
int a[200010];
void solve() {
//STEP 1 : std Code copy here and replace "cin" and "cout" with generator
//|---------------------------------------------|
	int n, k;
	n = mrand(1e5+8e4, 2e5);
	iprint(n);
	for (int i = 0; i < n; i++) {
		a[i] = intRand(1, 1e6, 0.35);
	}
	k = a[intRand(0, n - 1)];
	iprint(k, '\n');
	for (int i = 0; i < n; i++) {
		iprint(a[i]);
	}
	sort(a, a + n);
	int l = 0, r = n - 1;
	while (l <= r) {
		int mid = l + (r - l ) / 2;
		if (a[mid] > k) r = mid - 1;
		if (a[mid] < k) l = mid + 1;
		if (a[mid] == k) {
			oprint(mid);
			return;
		}
	}
	oprint(-1);
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
