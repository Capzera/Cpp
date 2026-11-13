#include <bits/stdc++.h>
using namespace std;
const int N = 1e3 + 10, M = 1e6 + 10;
struct node {
	int w, c;
	bool operator < (const struct node& rhs) const {
		return w < rhs.w;
	}
	bool operator < (const 结构体名& rhs) const {
		return 左手边的量 < rhs.量; // 按量从小到大升序
		return 左手边的量 > rhs.量; // 按量从大到小升序
	}
} a[N];
int n, v, m, dp[M];
bool cmp(node x, node y) {
	return x.w < y.w;
}
int main() {
	cin >> n >> v >> m;
	for (int i = 1; i <= n; i++) {
		cin >> a[i].w >> a[i].c;
	}
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) {
		for (int j = m; j >= a[i].w; j--) {
			dp[j] = max(dp[j], dp[j - a[i].w] + a[i].c);
			if (dp[j] >= v) {
				cout << a[i].w;
				return 0;
			}
		}
	}
	cout << "No Solution!" << endl;
	return 0;
}
