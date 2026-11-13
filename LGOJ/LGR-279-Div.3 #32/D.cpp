#include <bits/stdc++.h>
using namespace std;
const int N = 1010;
inline int read() {
	int ans = 0, f = 1;
	char ch = getchar();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar();
	}
	return ans * f;
}

struct node {
	int l, r, id;
	bool operator < (const struct node& rhs) const {
		return r < rhs.r;
	}
} a[N];

int t, n, ans, dp[401][401], cnt[401][401];
vector<node> sel;
bool check(node x, node y) {
	if (x.r < y.l || y.r < x.l) return 1;
	if (x.l == y.l || x.l == y.r || x.r == y.l || x.r == y.r) return 1;
	return 0;
}
void dfs(int i) {
	if (i > n) {
		ans = max(ans, (int)sel.size());
		return;
	}
	bool f = 1;
	for (auto& p : sel) {
		if (!check(p, a[i])) {
			f = 0;
			break;
		}
	}
	if (f) {
		sel.push_back(a[i]);
		dfs(i + 1);
		sel.pop_back();
	}
	dfs(i + 1);
}

void solve() {
	n = read();
	ans = 0;
	memset(dp, 0, sizeof(dp));
	memset(cnt, 0, sizeof(cnt));
	bool flagA = 1;
	unordered_map<int, int> mp;
	for (int i = 1; i <= n; i++) {
		a[i].l = read(), a[i].r = read();
		mp[a[i].l]++, mp[a[i].r]++;
	}
	for (int i = 1; i <= n; i++) {
		for (int j = i + 1; j <= n; j++) {
			if (a[i].l == a[j].l || a[i].l == a[j].r || a[i].r == a[j].l || a[i].r  == a[j].r) flagA = 0;
		}
	}
	if (n <= 10) {
		sel.clear();
		dfs(1);
		cout << ans << '\n';
		return;
	}
	if (n <= 200) {
		int mx = 0;
		for (int i = 1; i <= n; i++) {
			cnt[a[i].l][a[i].r]++;
			mx = max(mx, a[i].r);
		}
		for (int len = 1; len <= mx; len++) {
			for (int i = 1; i + len - 1 <= mx; i++) {
				int j = i + len - 1;
				for (int k = i; k < j; k++) {
					dp[i][j] = max(dp[i][j], dp[i][k] + dp[k + 1][j]);
				}
				for (int k = i + 1; k < j; k++) {
					dp[i][j] = max(dp[i][j], dp[i][k] + dp[k][j] - cnt[k][k]);
				}
				if (cnt[i][j] > 0) {
					int val = 0;
					for (int k = i; k <= j; k++) {
						int sum = 0;
						for (int x = i; x <= k; x++) {
							if (x < j) sum += cnt[i][x];
						}
						for (int y = k; y <= j; y++) {
							if (y > i) sum += cnt[y][j];
						}
						val = max(val, sum);
					}
					dp[i][j] = max(dp[i][j], cnt[i][j] + val);
				}
			}
		}
		cout << dp[1][mx] << '\n';
		return;
	}
	if (flagA) {
		sort(a + 1, a + n + 1);
		for (int i = 1, lst = -1; i <= n; i++) {
			if (a[i].l > lst) {
				ans++;
				lst = a[i].r;
			}
		}
		cout << ans << '\n';
		return;
	}
	for (auto&& [_, x] : mp) {
		ans = max(ans, x);
	}
	cout << ans << '\n';
	return;
}

int main() {
	t = read();
	while (t--) solve();
	return 0;
}
/*
n <= 200, O(n^3)
考虑区间dp
离散化坐标到2n, 

设dp[i][j] 表示位置[i, j] 刚好碰上的区间最大个数
dp[i][j] 在转移过程中
	1) 不选择横跨[i, j] dp[i][j] = max(dp[i][j], dp[i][k] + dp[k + 1][j])
	2) 如果要选择区间[i, j]，则内部只允许选择端点为i，或者端点为j的区间
		dp[i][j] = max(dp[i][j], cnt[i][j] + max(left, right))
*/
