#include <bits/stdc++.h>
using namespace std;
int main() {
	long long n, x, sum = 0, ans = 0;
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		sum += x;
		if (i > 1 && sum > 0) ans += sum;
	}
	cout << ans << endl;
	return 0;
}
