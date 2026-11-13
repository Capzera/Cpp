#include <bits/stdc++.h>
#define int unsigned long long
using namespace std;
int t = 10, a, b;
int f(int a, int b) {
	if (a % b == 0) return a;
	return a / b * b + f(b, a % b);
}
signed main() {
	while (t--) {
		cin >> a >> b;
		cout << f(a, b) << '\n';
	}
	return 0;
}
