#include <bits/stdc++.h>
using namespace std;
int n, m, p;
int main() {
	cin >> n >> m;
	vector<int> a(n);
	for (auto& x : a) cin >> x;
	while (m--) {
		p = (p - a[p] + n * 5000) % n;
	}
	cout << p + 1;
	return 0;
}
