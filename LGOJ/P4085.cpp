#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
long long n, m, ans = LLONG_MAX, f[N], s[N];
long long slide, head = 1, tail, q[N];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) cin >> f[i] >> s[i];
	for (int l = 1, r = 1; r <= n; r++) {
		while (head <= tail && s[q[tail]] <= s[r]) tail--;
		q[++tail] = r;
		slide += f[r];
		while (slide >= m) {
			ans = min(ans, s[q[head]]);
			slide -= f[l];
			if (q[head] == l++) head++;
		}
	}
	cout << ans;
	return 0;
}
