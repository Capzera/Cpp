#include <bits/stdc++.h>
using namespace std;
int k, x, y, n, m;
void exgcd(int a, int b) {
	if (b == 0) {
		x = 1, y = 0;
		return;
	}
	exgcd(b, a % b);
	k = x, x = y, y = k - a / b * y;
	return;
}
int main() {
	cin >> n >> m;
	exgcd(n, m);
	cout << (x + m) % m;
	return 0;
}
