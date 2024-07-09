#include <bits/stdc++.h>
using namespace std;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	long long n, k, m;
	double t;
	cin >> n >> k >> t >> m;
	n *= k;
	long long bad = n * t;
	n -= bad;
	cout << n / m << " " << n % m;
	return 0;
}
