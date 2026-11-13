#include <bits/stdc++.h>
using namespace std;
long long n, k, p, q;
int main() {
	cin >> n >> k >> p >> q;
	if (k * q <= n * p) return puts("0"), 0;
	cout << ceil(1.0 * k * q / p) - n;
	return 0;
}
