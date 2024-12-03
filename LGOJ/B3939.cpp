#include <bits/stdc++.h>
using namespace std;
int l, r, a[101];
bool isPrime(int x) {
	for (int i = 2; i * i <= x; i++) {
		if (x % i == 0) return 0;
	}
	return 1;
}
int main() {
	cin >> l >> r;
	for (int i = l; i <= r; i++) {
		int j = i % 10 * 10 + i / 10;
		if (isPrime(i) && isPrime(j)) cout << i << endl;
	}
	return 0;
}
