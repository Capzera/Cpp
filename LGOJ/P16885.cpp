#include <bits/stdc++.h>
using namespace std;
long long t, n;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> t;
	for (int i = 1; i <= t; i++) {
		cin >> n;
		printf("Case #%d: %lld\n", i, (n + 4) / 5);
	}
	return 0;
}
