#include <bits/stdc++.h>
using namespace std;
const int p = 19260817;
int x, y, a, b;
int read() {
	int ans = 0, ch = getchar();
	while(!isdigit(ch) and ch != EOF) ch = getchar();
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ans %= p;
		ch = getchar();
	}
	return ans;
}
void exgcd(long long a, long long b) {
	if (b == 0) {
		x = 1, y = 0;
		return;
	}
	exgcd(b, a % b);
	int tx = x;
	x = y;
	y = tx - a / b * y;
}
int main() {
	a = read(),	 b = read();
	if (!b) return puts("Angry!"), 0;
	exgcd(b, p);
	x = (x % p + p) % p;
	cout << 1ll * a * x % p;
	return 0;
}
/*
bx % p == a
py + a = bx
bx + py = a
b/ax + p/ay = 1
*/
