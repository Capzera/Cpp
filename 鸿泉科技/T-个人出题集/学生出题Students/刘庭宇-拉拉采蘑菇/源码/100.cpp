#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, x, ans = 0;
	cin >> n;
	while (n--) {
		cin >> x;
		ans += (x - 1) ^ 1;
	}
	cout << ans;
	return 0;
}
