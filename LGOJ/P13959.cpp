#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 1;
struct node {
	int st, val;
	bool operator < (const struct node& rhs) const {
		return st < rhs.st;
	}
} a[N];
int t, n, m;
bool solve() {
	cin >> n >> m;
	for (int i = 1; i <= m; i++) cin >> a[i].st >> a[i].val;
	sort(a + 1, a + m + 1);
	for (int i = 1; i <= m; i++) {
		if (a[i].val > a[i].st) return 0;
		int diff = a[i].val - (a[i].st - a[i - 1].st); // diff 上一个应该得从diff开始起步
		if (diff >= 0 && diff != a[i - 1].val) return 0;
	}
	return 1;
}
int main() {
	cin >> t;
	while (t--) cout << (solve() ? "Yes\n" : "No\n");
	return 0;
}
/*


a[i].a - a[i - 1].a 中间能做几次操作 op


diff = a[i].b - op 上一个元素必须要以diff结尾
diff >= 0 <====>  a[i].b >= op  中间一个c都不能填
diff < 0   a[i].b < op  中间可以在某个(a[i].b - op - 1)位置 填写c，往后的位置填写+
*/
