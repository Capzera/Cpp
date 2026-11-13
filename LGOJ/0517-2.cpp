#include <bits/stdc++.h>
using namespace std;
const int p1 = 131, m1 = 998244353;
const int N = 1e4 + 10, M = 1e5 + 10;
string s;
int n;
long long p[M], h[M];
set<long long> ans;
int main() {
	p[0] = 1;
	for (int i = 1; i <= 100000; i++) {
		p[i] = p[i - 1] * p1 % m1;
	}
	cin >> n;
	while (n--) {
		cin >> s;
		int m = s.size();
		memset(h, 0, sizeof(h));
		for (int i = 0; i < m; i++) {
			h[i + 1] = (h[i] * p1 + s[i]) % m1; 
		}
		ans.insert(h[m]);
	}
	cout << ans.size();
	return 0;
}
