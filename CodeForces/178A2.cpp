#include <bits/stdc++.h>
using namespace std;
long long n, x, a[100010], sum;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i < n; i++) {
		long long p = 1;
		while (i + (p << 1) <= n) {
			p <<= 1;
		}
		a[i + p] += a[i];
		sum += a[i];
		cout << sum << endl;
	}
	return 0;
}
