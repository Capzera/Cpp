#include <bits/stdc++.h>
using namespace std;
const int N = 4e5 + 10;
int t, n, x;
long long pre[N], nxt[N], sum[N], stk[N];
long long solve() {
	scanf("%d", &n);
	long long ans = 0, top = 0;
	nxt[0] = n + 1;
	for (int i = 1; i <= n; i++) {
		scanf("%d", &x);
		pre[i] = pre[i - 1] + x;
		sum[i] = sum[i - 1] + pre[i];
		nxt[i] = n + 1;
	}
	for (int i = 0; i <= n; i++) {
		while (top && pre[i] < pre[stk[top]]) nxt[stk[top--]] = i;
		stk[++top] = i;
	}
	for (int l = 1; l <= n; l++) {
		int r = nxt[l - 1] - 1;
		if (l <= r) ans += sum[r] - sum[l - 1] - (r - l + 1) * pre[l - 1];
	}
	return ans;
}
int main() {
	cin >> t;
	for (int i = 1; i <= t; i++) {
		printf("Case #%d: %lld\n", i, solve());
	}
	return 0;
}


