#include <bits/stdc++.h>
using namespace std;
const int N = 3e5 + 10;
long long n, top, ans, a[N], stk[N];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) {
		while (top && a[i] > a[stk[top]]) {
			ans += i - stk[top--] + 1;
		}
		if (top) ans += i - stk[top] + 1;
		stk[++top] = i;
	}
	cout << ans;
	return 0;
}
