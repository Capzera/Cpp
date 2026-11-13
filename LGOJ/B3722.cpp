#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
long long n, ans, sa, sb, a[N], b[N];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) cin >> b[i];
	for (int i = 1; i <= n; i++) {
		sa += a[i];
		sb += b[i];
		if (sb > sa) {
			cout << i;
			return 0;
		}
	}
	return 0;
}
