#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, k;
	long long ans = 0;
	cin >> n;
	for (int i = 1; i <= n; i++) {
		int num = i;
		while (num) {
			ans += num % 10 == 1;
			num /= 10;
		}
	}
	cout << ans << endl;
	return 0;
}
