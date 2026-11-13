#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int n, x, top, a[N], stk[N], l[N], r[N];
long long ans, sum[N];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		sum[i] = sum[i - 1] + a[i];
	}
	for (int i = 1; i <= n; i++) {
		while (top && a[i] <= a[stk[top]]) top--;
		l[i] = stk[top];
		stk[++top] = i;
	}
	top = 0, stk[0] = n + 1;
	for (int i = n; i > 0; i--) {
		while (top && a[i] <= a[stk[top]]) top--;
		r[i] = stk[top];
		stk[++top] = i;
	}
	//for (int i = 1; i <= n; i++) cout << l[i] << " "; cout << '\n';
	//for (int i = 1; i <= n; i++) cout << r[i] << " "; cout << '\n';
	for (int i = 1; i <= n; i++) {
		ans = max(ans, a[i] * (sum[r[i] - 1] - sum[l[i]]));
	}
	cout << ans;
	return 0;
}
/*
i  01 02 03 04 05 06
a  03 01 06 04 05 02
s  03 04 10 14 19 21
l  00 00 03 03 05 03
r  01 06 03 05 05 06
*/
