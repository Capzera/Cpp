#include <bits/stdc++.h>
using namespace std;
struct Intervals {
	int start, end;
	bool operator < (const struct Intervals& rhs) const {
		return start < rhs.start; // 区间合并按起始排序，区间并列按结束排序
	}
} a[10001];
int n, L;
int main() {
	cin >> n >> L;
	for (int i = 1; i <= n; i++) cin >> a[i].start >> a[i].end;
	sort(a + 1, a + n + 1); // 按起始排序
	int mx = 0, ans = 0; // mx表示木板最远位置，ans表示已经用了多少模板
	for (int i = 1; i <= n; i++) {
		mx = max(mx, a[i].start); // 计算当前应从mx开始铺起
		int num = (a[i].end - mx + L - 1) / L; // 计算距离/长度 = 个数，最少多少个木板
		ans += num; 
		mx += num * L;
	}
	cout << ans;
	return 0;
}
