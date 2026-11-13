#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int top, t, n, x, a[N], b[N], stk[N];
int solve() {
	scanf("%d", &n);
	top = 0;
	for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
	for (int i = 1; i <= n; i++) scanf("%d", &b[i]);
	for (int i = 1, j = 1; i <= n; i++) {
		stk[++top] = a[i];
		while (top && stk[top] == b[j]) j++, top--;
	}
	return top == 0;
}
int main() {
	cin >> t;
	while (t--) printf("%s\n", solve() ? "Yes" : "No");
	return 0;
}
