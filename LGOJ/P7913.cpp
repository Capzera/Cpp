#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
const int N = 1e5 + 5;
int n, m1, m2, ans, f1[N], f2[N];
struct node {
	int st, ed;
	bool operator < (const struct node& rhs) const {
		if (st != rhs.st) return st < rhs.st;
		return ed < rhs.ed;
	}
} a[N], b[N];
void solve(int m, node *a, int *ans) {
	priority_queue<pii, vector<pii>, greater<pii> > leave;
	priority_queue<int, vector<int>, greater<int> > wait;
	for (int i = 1; i <= n; i++) wait.push(i);
	for (int i = 1; i <= m; i++) {
		while (leave.size() && a[i].st >= leave.top().first) {
			wait.push(leave.top().second); leave.pop();
		}
		if (wait.empty()) continue;
		int j = wait.top(); wait.pop();
		ans[j]++;
		leave.push({a[i].ed, j});
	}
	for (int i = 1; i <= n; i++) ans[i] += ans[i - 1];
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m1 >> m2;
	for (int i = 1; i <= m1; i++) cin >> a[i].st >> a[i].ed;
	for (int i = 1; i <= m2; i++) cin >> b[i].st >> b[i].ed;
	sort(a + 1, a + m1 + 1), sort(b + 1, b + m2 + 1);
	solve(m1, a, f1), solve(m2, b, f2);
	for (int x = 0; x <= n; x++) ans = max(ans, f1[x] + f2[n - x]);
	cout << ans;
	return 0;
}
