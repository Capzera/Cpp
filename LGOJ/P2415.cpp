#include <bits/stdc++.h>
using namespace std;
long long x, i, ans;
int main() {
	while (cin >> x) {
		ans += x;
		i++;
	}
	ans *= 1 << i - 1;
	cout << ans;
	return 0;
}
