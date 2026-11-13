#include <bits/stdc++.h>
using namespace std;
long long k, n, x, cnt = 0;
int main() {
	cin >> n >> k;
	while (n--) {
		cin >> x;
		cnt += k - x;
	}
	cout << max(k - cnt, 0ll);
	return 0;
}
