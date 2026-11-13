#include <bits/stdc++.h>
using namespace std;
long long n, m, num, j = 1, s, a[20], v[20], pa[20], dp[1 << 16], sum[1 << 16];
int find(int x) {
	return pa[x] == x ? x : pa[x] = find(pa[x]);
}
void merge(int x, int y) {
	x = find(x), y = find(y);
	pa[y] = x;
}
int main() {
	cin >> n >> m;
	memset(dp, 0x3f, sizeof(dp)); dp[0] = 0;
	for (int i = 1; i <= n; i++) cin >> a[i];
	iota(pa + 1, pa + n + 1, 1);
	cin >> s;cin.ignore();
	while (s--) {
		string str; getline(cin, str); stringstream buf(str);
		vector<int> vec;
		while (buf >> num) vec.push_back(num);
		for (int i = 1; i < vec.size(); i++) merge(vec[0], vec[i]);
	}
	for (int i = 1; i <= n; i++) {
		if (pa[i] == i) continue;
		a[pa[i]] += a[i];
		a[i] = 0;
	}
	for (int i = 1; i <= n; i++) {
		if (!a[i]) continue;
		a[j++] = a[i];
	}
	n = j - 1;
	for (int i = 1; i < 1 << n; i++)
		for (int j = 0; j < n; j++)
			if (i >> j & 1) sum[i] += a[j + 1];
	for (int i = 1; i < 1 << n; i++) {
		for (int j = i; j; j = j - 1 & i) {
			if (sum[j] > m) continue;
			dp[i] = min(dp[i], dp[i ^ j] + 1);
		}
	}
	cout << dp[(1 << n) - 1];
	return 0;
}
