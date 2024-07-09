#include <bits/stdc++.h>
using namespace std;

int main() {
	int n;
	long long a[100];
	cin >> n;
	if (n == 1) {
		cout << 1; return 0;
	}
	if (n == 2) {
		cout << 1; return 0;
	}
	if (n == 3) {
		cout << 1; return 0;
	}
	a[1] = a[2] = a[3] = 1;
	for (int i = 4; i <= n; i++) {
		a[i] = a[i - 1] + a[i - 2] + a[i - 3];
	}
	
	cout << a[n];
	return 0;
}
