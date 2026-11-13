#include <bits/stdc++.h>
using namespace std;
struct food {
	int score, type;
	bool operator < (const struct food& rhs) const {
		return score > rhs.score;
	}
} a[205];
long long n, m, k, ans, cnt[201];
int main() {
	cin >> n >> m >> k;
	for (int i = 1; i <= k; i++) cin >> cnt[i];
	for (int i = 1; i <= n; i++) cin >> a[i].score >> a[i].type;
	sort(a + 1,a + n + 1);		
	for (int i = 1; i <= n; i++) {
		if (cnt[a[i].type] == 0) continue; // 判断当前食材同类是否有剩余
		cnt[a[i].type]--; // 先扣除上限
		ans += a[i].score; // 增加贡献
		m--; // 目标 --
		if (m == 0) break; // 目标已经不剩了，直接退出
	}
	cout << ans;
	return 0;
}
