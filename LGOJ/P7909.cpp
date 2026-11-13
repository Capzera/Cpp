#include <bits/stdc++.h>
using namespace std;
long long n, l, r;
int main() {
	cin >> n >> l >> r;
	if (l / n == r / n) return cout << r % n, 0;
	cout << n - 1;
	return 0;
}

