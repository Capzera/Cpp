#include <bits/stdc++.h>
using namespace std;
int n, ans, f[10001];
void fun(int n, int m) {
	if (!n) {
		if (!f[m]) ans++;
		f[m] = 1;
		return;
	}
	for (int i = n; i > 0; i--) {
		fun(n - i, i * (n - i) + m);
	}
}
int main() {
	cin >> n;
	fun(n, 0);
	cout << ans;
	return 0;
}
