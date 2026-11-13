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
int n, k, head = 1, tail = 0;
int a[N], q[N];
int main() {
	n = read(), k = read();
	for (int i = 1; i <= n; i++) a[i] = read();
	for (int i = 1; i <= n; i++) {
		while (head <= tail && q[head] <= i - k) head++;
		while (head <= tail && a[q[tail]] <= a[i]) tail--;
		q[++tail] = i;
		if (i >= k) printf("%d\n", a[q[head]]);
	}
	return 0;
}
