#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, k, m;
	double t;
	cin >> n >> k >> t >> m;
	n *= k;
	int bad = n * t;
	n -= bad;
	cout << n / m << " " << n % m;
	return 0;
}
