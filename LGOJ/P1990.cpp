#include <bits/stdc++.h>
using namespace std;
int a[1000010], n;
int main() {
	cin >> n;
	a[0] = 1, a[1] = 1, a[2] = 2;
	for (int i = 3; i <= n; i++) {
		a[i] = (2 * a[i - 1] + a[i - 3]) % 10000;
	}
	cout << a[n];
	return 0;
}
/*
11 121 12 22 2023
n = 1  1
n = 2  2
n = 3  5
n = 4  5
*/
