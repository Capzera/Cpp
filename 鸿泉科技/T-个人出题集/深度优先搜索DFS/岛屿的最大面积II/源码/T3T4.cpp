#include <bits/stdc++.h>
using namespace std;
int n, k, a[17];
long long mx = 1e18;
vector<long long> mul;
void dfs(int nw, int ed, long long m) {
	if (nw > ed) {
		mul.emplace_back(m);
		return;
	}
	if (mx / a[nw] >= m) {
		dfs(nw, ed, m * a[nw]);
	}
	dfs(nw + 1, ed, m);
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	cin >> k;
	dfs(1, n, 1);
	cout << mul.size() << endl;
	sort(mul.begin(), mul.end());
	cout << mul.size() << endl;
	cout << mul[k - 1];
	return 0;
}
/*
16
2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53
755104793
1 2 3 4 5 6 8
*/
