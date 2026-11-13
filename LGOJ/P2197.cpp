#include <bits/stdc++.h>
using namespace std;
long long t, n, x;
int solve() {
	cin >> n;
	long long ans = 0;
	while (n--) {
		cin >> x;
		ans ^= x;
	}
	return ans != 0;
}

int main() {
	cin >> t;
	while (t--) cout << (solve() ? "Yes\n" : "No\n");
	return 0;
}
