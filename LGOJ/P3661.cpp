#include <bits/stdc++.h>
using namespace std;
struct interval {
	int start, end;
	bool operator < (const struct interval& rhs) const {
		return end < rhs.end;
	}
} vec[20001];
int n, m, cnt, ans, a[20001];
int main() {
	cin >> n >> m;
	cnt = n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= m; i++) cin >> vec[i].start >> vec[i].end;
	sort(a + 1, a + n + 1), sort(vec + 1, vec + m + 1);
	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++) {
			if (a[j] < vec[i].start || a[j] > vec[i].end) continue;
			a[j] = -1;
			ans++;
			cnt--;
			break;
		}
		if (!cnt) break;
	}
	cout << ans << endl;
	return 0;
}
