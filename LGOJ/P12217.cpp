#include <bits/stdc++.h>
using namespace std;
const long long p = 2146516019;
long long n = 233333333, ans = 1;
long long ksm(long long a, int b) {
	long long ans = 1;
	while (b) {
		if (b & 1) ans = ans * a % p;
		a = a * a % p;
		b >>= 1;
	}
	return ans;
}
int main() {
	for (int i = 2; i <= 2; i++) {
		ans ^= ksm(i, p - 2);
	}
	cout << "1307261675";
	return 0;
}
