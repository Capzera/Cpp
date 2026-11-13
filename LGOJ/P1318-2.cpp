#include <bits/stdc++.h>
using namespace std;
const int N = 1e4 + 10;
int n, top, ans, a[N], stk[N];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) {	
		while (top && a[i] >= a[stk[top]]) {
			int t = stk[top];
			if (!--top) break;
			int j = stk[top];
			ans += (i - j - 1) * (min(a[i], a[j]) - a[t]);
		}
		stk[++top] = i;
	}
	cout << ans;
	return 0;
}
