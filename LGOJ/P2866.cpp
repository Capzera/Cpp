#include <bits/stdc++.h>
using namespace std;
const int N = 8e4 + 10;
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
inline void write(long long x) {
	if (x < 0) putchar('-'), x = -x;
	if (x > 9) write(x / 10);
	putchar(x % 10 + '0');
}
int n, top, a[N], stk[N];
long long ans;
int main() {
	n = read();
	for (int i = 1; i <= n; i++) a[i] = read();
	for (int i = 1; i <= n; i++) {
		while (top && a[i] >= a[stk[top]]) top--;
		ans += top;
		stk[++top] = i;
	}	
	write(ans);
	return 0;
}
