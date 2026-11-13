#include <bits/stdc++.h>
using namespace std;
const int N = 2e6 + 10;
inline long long read() {
	long long ans = 0, f = 1;
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
int n, m, head = 1, tail = 0, a[N], q[N];
int main() {
	n = read(), m = read();
	for (int i = 1; i <= n; i++) a[i] = read();
	for (int i = 1; i <= n; i++) {
		cout << a[q[head]] << '\n';
		while (head <= tail && q[head] <= i - m) head++;
		while (head <= tail && a[q[tail]] >= a[i]) tail--;
		q[++tail] = i;
	}
	return 0;
}
