#include <bits/stdc++.h>
using namespace std;
long long ans;
int main() {
	int n;
	cin >> n;
	for (int l = 1, r = 1; l <= n; l = r + 1) {
		r = n / (n / l);
		ans += 1ll * (n / l) * (r - l + 1);
	}
	cout << ans;
	return 0;
}
