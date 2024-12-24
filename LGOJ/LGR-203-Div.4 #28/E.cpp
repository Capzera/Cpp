#include <bits/stdc++.h>
#define int long long
using namespace std;
int n, m, r, k, cnt;
vector<int> ans;
bool isPrime(long long x) {
	if (x < 2) return 0;
	for (int i = 2; i < x; i++) {
		if (x % i == 0) return 0;
	}
	return 1;
}
signed main() {
	cin >> n >> m >> r >> k;
	for (int x = r; x <= n; x += m) {
		if (isPrime(x)) ans.push_back(x);
	}
	sort(ans.begin(), ans.end(), greater<int>());
	ans.resize(k, -1);
	cout << ans[k - 1];
	return 0;
}
/*
x ÷ m = i …… r
m * i + r = x;
i = 0 x = r
 x += m;

*/
