#include <bits/stdc++.h>
using namespace std;
long long n, k, x[1000001], y[1000001], a, b;
int main() {
	cin >> n >> k;
	for (int i = 1; i <= k; i++) {
		cin >> x[i] >> y[i];
	}
	sort(x + 1, x + k + 1), sort(y + 1, y + k + 1);
	for (int i = 1; i <= k; i++) {
		a += x[i] != x[i - 1];
		b += y[i] != y[i - 1];
	}
	cout << n * (a + b) - a * b;
	return 0;
}
