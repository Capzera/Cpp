#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
const int read() {
	int ans = 0;
	char ch = getchar();
	while (!isdigit(ch)) ch = getchar();
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar();
	}
	return ans;
}
int n, top, h[N], w[N], stk[N];
long long ans, sum[N];
int main() {
	n = read();
	for (int i = 1; i <= n; i++) {
		h[i] = read(), w[i] = read();
		while (top && h[i] > h[stk[top]]) sum[i] += w[stk[top--]];
		sum[stk[top]] += w[i];
		stk[++top] = i;
	}
	for (int i = 1; i <= n; i++) ans = max(ans, sum[i]);
	cout << ans;
	return 0;
}
