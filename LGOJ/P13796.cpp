#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
long long n, fz, fm = 114514, top, a[N], stk[N];
void max(long long nfz, long long nfm) {
	if (nfz * fm > nfm * fz) {
		fz = nfz;
		fm = nfm;
	}
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) {
		while (top && a[i] > a[stk[top]]) top--;
		if (top) {
			int j = stk[top];
			long long A = i - j, B = a[j] - a[i], C = a[j] - a[j + 1];
			max(A * C - B, C);
		}
		stk[++top] = i;
	}
	top = 0;
	for (int i = n; i; i--) {
		while (top && a[i] > a[stk[top]]) top--;
		if (top) {
			int j = stk[top];
			long long A = j - i, B = a[j] - a[i], C = a[j] - a[j - 1];
			max(A * C - B, C);
		}
		stk[++top] = i;
	}
	long long g = __gcd(fz, fm);
	fz /= g, fm /= g;
	cout << fz;
	if (fm > 1) cout << "/" << fm;
	return 0;
}

