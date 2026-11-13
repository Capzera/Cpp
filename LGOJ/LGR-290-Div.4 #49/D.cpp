#include <bits/stdc++.h>
using namespace std;
long long n, a[61];
int main() {
	cin >> n;
	a[0] = a[1] = 1, a[2] = 2;
	for (int i = 3; i <= n; i++) {
		a[i] = a[i - 1] + a[i - 3];
	}
	cout << a[n];
	return 0;
}
