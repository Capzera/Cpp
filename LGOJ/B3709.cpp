#include <bits/stdc++.h>
using namespace std;
int n, fac = 1;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) fac *= i;
	cout << 2 * (fac - n) << " " << 2 * (fac - n * 2) << " " << 2;
	return 0;
}
