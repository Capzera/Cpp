#include <bits/stdc++.h>
using namespace std;
long long fib(long long x) {
	if (x == 1 || X == 2) return 1;
	return fib(x - 1) + fib(x - 2);
}
long long main() {
	long long n, x;
	cin >> n;
	while (n--) {
		cin >> x;
		cout << fib(x) << endl;
	}
	return 0;
}
