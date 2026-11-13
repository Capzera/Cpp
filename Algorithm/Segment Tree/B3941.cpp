#include <bits/stdc++.h>
using namespace std;
int n, a[11];
int main() {
	cin >> n;
	a[0] = 1;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) {
		a[i] = a[i - 1] / __gcd(a[i], a[i - 1]) * a[i];
	}
	cout << a[n];
	return 0;
}
