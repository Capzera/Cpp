#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10;
inline int read() {
	int ans = 0, f = 1;
	char ch = getchar();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar();
	}
	return ans * f;
}
int n, k, x, head = 1, tail;
int ans = INT_MIN, a[N], q[N];
int main() {
	n = read(), k = read();
	for (int i = 1; i <= n; i++) x = read(), a[i] = a[i - 1] + x;
	q[++tail] = 0;
	for (int i = 1; i <= n; i++) {
		while (head <= tail && q[head] < i - k) head++;
		ans = max(ans, a[i] - a[q[head]]);
		while (head <= tail && a[q[tail]] >= a[i]) tail--;
		q[++tail] = i;
	}
	cout << ans;
	return 0;
}
