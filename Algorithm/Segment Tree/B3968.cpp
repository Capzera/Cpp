#include <bits/stdc++.h>
using namespace std;
struct stu {
	int yw, sx, yy, zf, id;
	bool operator < (stu& rhs) {
		if (zf != rhs.zf) return zf > rhs.zf;
		if (yw + sx != rhs.yw + rhs.sx) return yw + sx > rhs.yw + rhs.sx;
		if (max(yw, sx) != max(rhs.yw, rhs.sx)) return max(yw, sx) > max(rhs.yw, rhs.sx);
		int x = min(id, rhs.id);
		return id < rhs.id;
	}
	bool operator == (stu& rhs) {
		return zf == rhs.zf && yw + sx == rhs.yw + rhs.sx && max(yw, sx) == max(rhs.yw, rhs.sx);
	}
} a[10010];
int n, ans[10010];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i].yw >> a[i].sx >> a[i].yy;
		a[i].zf = a[i].yw + a[i].sx + a[i].yy;
		a[i].id = i;
	}
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) {
		ans[a[i].id] = a[i] == a[i - 1] ? ans[a[i - 1].id] : i;
	}
	for (int i = 1; i <= n; i++) cout << ans[i] << endl;
	return 0;
}
