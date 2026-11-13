#include <bits/stdc++.h>
using namespace std;
long long n, m, q, x;
int main() {
	cin >> n >> m >> q;
	vector<int> ans;
	while (q--) {
		x = (n % 10) * (n % 10) % 10;
		n = stoi(to_string(x) + to_string(n / 10));
		ans.push_back(n);
		if (n == m) {
			for (auto& x : ans) cout << x << endl;
			return 0;
		}
	}
	cout << -1;
	return 0;
}
