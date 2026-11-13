#include <bits/stdc++.h>
using namespace std;
long long n, ans;

int phi(long long n) {
	long long ans = n, m = sqrt(n);
	for (int i = 2; i <= m; i++) {
		if (n % i) continue;
		while (n % i == 0) n /= i;
		ans -= ans / i;
	}
	if (n > 1) ans -= ans / n;
	return ans;
}

int main() {
	cin >> n;
	vector<long long> fac;
	for (long long i = 1; i * i <= n; i++) {
		if (n % i) continue;
		fac.push_back(i);
		ans += i * phi(n / i);
		if (i * i != n) ans += n / i * phi(i);
	}
	cout << ans;
	return 0;
}
