#include <bits/stdc++.h>
using namespace std;
int n;
int main() {
	cin >> n;
	vector<string> a(n, string(n, '.'));
	for (int i = 0; i <= n >> 1; i++) {
		a[i][n / 2 - i] = a[i][n / 2 + i] = a[n - i - 1][n / 2 - i] = a[n - i - 1][n / 2 + i] = '#';
	}
	for (auto& x : a) cout << x << '\n';
	return 0;
}
