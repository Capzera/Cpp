#include <bits/stdc++.h>
using namespace std;
long long n, x, sum, ans;
int main() {
	cin >> n;
	while (n--) {
		cin >> x;
		sum -= x;
		ans = max(ans, sum + 1);
	}
	cout << ans;
	return 0;
}

