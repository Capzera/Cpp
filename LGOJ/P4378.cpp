#include <bits/stdc++.h>
using namespace std;
int n, ans, t[400010];
struct vec{
	int val, id;
} a[100010];

inline bool cmp(vec x, vec y) {
	return x.val < y.val || x.val == y.val && x.id < y.id;
}

int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i].val, a[i].id = i;
	}
	sort(a + 1, a + n + 1, cmp);
	for (int i = 1; i <= n; i++) {
		ans = max(ans, a[i].id - i);
	}
	cout << ans + 1;
	return 0;
}
