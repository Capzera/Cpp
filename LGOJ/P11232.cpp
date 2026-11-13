#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 5;
struct node {
	int l, r;
	bool operator < (const struct node& rhs) const {
		return r < rhs.r;
	}
} vec[N];
long long n, m, L, V, T, d, v, a, p[N];
void solve() {
	int ans1 = 0, ans2 = 0;
	cin >> n >> m >> L >> V;
	for (int i = 1; i <= n; i++) {
		cin >> d >> v >> a;
		// 初始距离， 初始速度， 加速度
		if (a == 0) {
			if (v > V) vec[i] = {d, L};
		} else if (a > 0) {
			
		}
		if (a < 0) {
			
		}
		
	}
	for (int i = 1; i <= m; i++) cin >> p[i];
	sort(vec + 1, vec + n + 1);
	
	for (int i = 1, lst = -1; i <= n; i++) {
		if (vec[i].l + vec[i].r == 0) continue;
		int j = upper_bound(p + 1, p + m + 1, vec[i].r) - p - 1;
		if (p[j] >= vec[i].l) ans1++;
		if (vec[i].l <= lst && vec[i].r >= lst) continue;
		lst = p[j];
		ans2++;
	}
	cout << ans1 << " " << ans2 << "\n";
}
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> T;
	while (T--) solve();
	return 0;
}

