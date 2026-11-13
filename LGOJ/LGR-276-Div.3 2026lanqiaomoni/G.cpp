#include <bits/stdc++.h>
#if defined (_WIN64)
#define getchar_unlocked _getchar_nolock
#define putchar_unlocked _putchar_nolock
#endif
using namespace std;
typedef long long ll;
const int N = 5001;
inline ll read() {
	ll ans = 0, f = 1;
	char ch = getchar_unlocked();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar_unlocked();
	}
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar_unlocked();
	}
	return ans * f;
}
inline void write(ll x) {
	if (x < 0) putchar_unlocked('-'), x = -x;
	if (x > 9) write(x / 10);
	putchar_unlocked(x % 10 + '0');
}
ll n, m, a[N], diff[N], sum[N];
int main() {
	n = read(), m = read();
	for (int i = 1; i <= n; i++) cin >> a[i];
	vector<int> diff(n + 1, 0);
	vector<ll> pref(n + 1, 0);
	for (int i = 2; i <= n; ++i) {
		diff[i] = a[i] != a[i - 1];
		sum[i] = sum[i - 1] + diff[i];
	}
	ll B = 1 + pref[n];  // 原始美丽度
	ll total_base = 0;
	for (int l = 1; l <= n; ++l) {
		for (int r = l; r <= n; ++r) {
			ll base = B - (pref[r] - pref[l])- (l > 1 ? diff[l] : 0) - (r < n ? diff[r + 1] : 0) + 1;
			total_base += base;
		}
	}
	
	// 左边界贡献：位置 i (1 ≤ i ≤ n-1) 的权重为 n-i
	vector<ll> cnt_left(m + 1, 0);
	ll total_left = 0;
	for (int i = 1; i <= n - 1; ++i) {
		ll w = n - i;
		total_left += w;
		cnt_left[a[i]] += w;
	}
	
	// 右边界贡献：位置 j (2 ≤ j ≤ n) 的权重为 j-1
	vector<ll> cnt_right(m + 1, 0);
	ll total_right = 0;
	for (int j = 2; j <= n; ++j) {
		ll w = j - 1;
		total_right += w;
		cnt_right[a[j]] += w;
	}
	
	// 输出每个颜色的答案
	for (int k = 1; k <= m; ++k) {
		ll left_sum = total_left - cnt_left[k];
		ll right_sum = total_right - cnt_right[k];
		ll ans = total_base + left_sum + right_sum;
		cout << ans << '\n';
	}
	
	return 0;
}
