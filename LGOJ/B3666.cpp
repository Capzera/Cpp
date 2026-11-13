#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
inline unsigned long long read() {
	unsigned long long ans = 0;
	char ch = getchar();
	while (!isdigit(ch)) ch = getchar();
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar();
	}
	return ans;
}
inline void write(unsigned long long x) {
	if (x < 0) putchar('-'), x = -x;
	if (x > 9) write(x / 10);
	putchar(x % 10 + '0');
}
int n, top;
unsigned long long ans, a[N], stk[N];
int main() {
	n = read();
	for (int i = 1; i <= n; i++) a[i] = read();
	for (int i = 1; i <= n; i++) {
		while (top && a[i] >= a[stk[top]]) {
			ans ^= stk[top];
			top--;
		}
		stk[++top] = i;
		ans ^= i;
		write(ans), putchar('\n');
	}	
	return 0;
}
