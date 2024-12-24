#include <bits/stdc++.h>
using namespace std;
long long n, x, ans, a[500010], mp[500010], tree[2000010];

inline bool cmp(int x, int y) {
	return a[x] == a[y] ? x > y : a[x] > a[y];
}

inline int lowbit(int x) {
	return x & -x;
}

inline void add(int x, int k) {
	for(; x <= n; x += lowbit(x)) tree[x] += k;
}

inline long long get(int x) {
	long long ans = 0;
	for (; x; x -= lowbit(x)) ans += tree[x];
	return ans;
}

int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		mp[i] = i;
	}
	sort(mp + 1, mp + n + 1, cmp);
	for (int i = 1; i <= n; i++) {
		add(mp[i], 1);
		ans += get(mp[i] - 1);
	}
	cout << ans;
	return 0;
}