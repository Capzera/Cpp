#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, ans = -1;
	string s;
	double mn = 100000;
	cin >> n >> s;
	for (int i = 0; i <= n; i++) {  // 枚举 道路建设的位置
		int l = 0, r = 0;
		bool fl = i == 0 || 1.0 * l >= i / 2.0;
		bool fr = i == n || 1.0 * r >= (n - i) / 2.0;
		if (fl && fr) {
			if (abs(n / 2.0 - i) < mn) {
				mn = abs(n / 2.0 - i);
				ans = i;
			}
		}
	}
	cout << ans << endl;
	return 0;
}
