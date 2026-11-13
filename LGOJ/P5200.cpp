#include <bits/stdc++.h>
using namespace std;
long long n, ans, a[100010], tree[400010];

inline int lowbit(int x) {
	return x & -x;
}

inline void add(int x, int k) {
	for (; x <= n; x += lowbit(x)) tree[x] += k;
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
	}
	int k = n - 1;
	while (k > 0 && a[k] < a[k + 1]) {
		k--;
	}
	cout << k << endl;
	for (int i = k + 1; i <= n; i++) add(a[i] , 1);
	for (int i = 1; i <= k; i++) {
		cout << k - i + get(a[i]) << " ";
		add(a[i], 1);
	}
	return 0;
}
