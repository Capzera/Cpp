#include <bits/stdc++.h>
using namespace std;
int t, n;
void solve() {
	scanf("%d", &n);
	if (n < 8 || n % 3 != 2) {
		puts("-1");
		return;
	}
	int k = (n - 2) / 3;
	for (int i = 0; i < k + 2; i++) printf("0 ");
	for (int i = 0; i < k + 2; i++) printf("1 ");
	for (int i = 0; i < k - 2; i++) printf(i % 3 == 2 ? "1 " : "0 ");
	puts("");
}
int main() {
	cin >> t;
	while (t--) solve();
	return 0;
}
