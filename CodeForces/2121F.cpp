#include <bits/stdc++.h>
using namespace std;
long long n, s, x, a[200010];
long long get(int mx) {
	long long ans = 0;
	for (int i = 1, j = 1; i <= n; i = j) {
		if (a[j] > mx) { // 以大于mx的元素作为分段点，找到分段的起点
			j++;
			continue;
		}
		while (j <= n && a[j] <= mx) j++; // 定分段的终点,此时j比段要多1
		long long tot = 0;
		map<long long, int> mp;
		mp[0] = 1;
		for (int x = i; x < j; x++) {
			tot += a[x];
			ans += mp[tot - s];
			mp[tot]++;
		} // 套模版，计算[i, j - 1]之间和为s的个数
	}
	return ans;
}
void solve() {
	cin >> n >> s >> x;
	for (int i = 1; i <= n; i++) cin >> a[i];
	cout << get(x) - get(x - 1) << endl;
}

int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cout.tie(nullptr)->sync_with_stdio(0);
	int t;
	cin >> t;
	while (t--) solve();
	return 0;
}
/*
1 0 3 3 6 7 8 1 1
1 0 2 0 2 7 8 0 0
9
1 0 0
0
1 -2 -1
-2
3 -1 -1
-1 1 -1
6 -3 -2
-1 -1 -1 -2 -1 -1
8 3 2
2 2 -1 -2 3 -1 2 2
9 6 3
1 2 3 1 2 3 1 2 3
13 7 3
0 -1 3 3 3 -2 1 2 2 3 -1 0 3
2 -2 -1
-2 -1
2 -2 -1
-1 -2

1
0
2
0
2
7
8
0
0

*/
