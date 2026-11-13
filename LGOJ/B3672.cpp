#include <bits/stdc++.h>
using namespace std;
long long n, m, q;
string s;
int main() {
	cin >> n >> q;
	vector<string> a(n);
	for (auto& x : a) cin >> x;
	while (q--) {
		int cnt = 0;
		cin >> m >> s;
		for (auto& x : a)  cnt += x.size() >= m && x.substr(x.size() - m) == s;
		cout << cnt << endl;
	}
	return 0;
}
