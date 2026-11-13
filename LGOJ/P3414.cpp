#include <bits/stdc++.h>
using namespace std;
const int p = 6662333;
long long qpow(long long a, long long b) {
	long long ans = 1;
	while (b) {
		if (b & 1) ans = ans * a % p;
		a = a * a % p;
		b >>= 1;
	}
	return ans;
}
long long n;
int main() {
	cin >> n;
	if (!n) return puts("1"), 0;
	cout << qpow(2, n - 1);
	return 0;
}
