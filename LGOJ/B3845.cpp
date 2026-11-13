#include <bits/stdc++.h>
using namespace std;
long long n, ans;
int main() {
	cin >> n;
	for (int c = 1; c <= n; c++) {
		for (int a = 1; a <= c; a++) {
			int b2 = c * c - a * a;
			int sq = sqrt(b2);
			if (sq < a) break;
			if (sq * sq == b2) {
				ans++;
			}
		}
	}
	cout << ans;
	return 0;
}
