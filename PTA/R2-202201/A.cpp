#include <bits/stdc++.h>
using namespace std;
long long n, a[11];
int main() {
	cin >> n;
	a[0] = 1, a[1] = 3;
	for (int i = 2; i <= n; i++) {
		a[i] = 4 * a[i - 1] - a[i - 2];
	}
	cout << a[n];
	return 0;
}
