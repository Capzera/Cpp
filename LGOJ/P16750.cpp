#include <bits/stdc++.h>
using namespace std;
const int N = 2e6 + 10;
string s;
int t, n, k, idx, sum[N], tr[N][26];
long long ans;
void insert(string s) {
	int p = 0, cnt = 0;
	for (auto& ch : s) {
		int x = ch - 'A';
		if (!tr[p][x]) tr[p][x] = ++idx;
		p = tr[p][x];
		sum[p]++;
	}
}
long long solve() {
	cin >> n >> k;
	for (int i = 0; i <= idx; i++) {
		for (int j = 0; j < 26; j++) tr[i][j] = 0;
		sum[i] = 0;
	}
	ans = idx = 0;
	while (n--) {
		cin >> s;
		insert(s);
	}
	for (int i = 1; i <= idx; i++) ans += sum[i] / k;
	return ans;
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> t;
	for (int i = 1; i <= t; i++) {
		printf("Case #%d: %lld\n", i, solve());
	}
	return 0;
}
