#include <bits/stdc++.h>
using namespace std;
int pa[200001], n, x, cnt, ans = INT_MAX;
inline int find(int x) {
	cnt++;
	return pa[x] == x ? x : find(pa[x]);
}
int main() {
	cin >> n;
	iota(pa, pa + n + 1, 0);
	for (int i = 1; i <= n;cnt = 0, i++) {
		cin >> x;
		find(x) == i ? ans = min(ans, cnt) : pa[i] = x;
	}
	cout << ans;
	return 0;
}
