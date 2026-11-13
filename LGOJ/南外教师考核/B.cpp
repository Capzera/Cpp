#include <bits/stdc++.h>
using namespace std;
long long n;
int main() {
	cin >> n;
	cin.tie(nullptr)->sync_with_stdio(0);
	for (int k = sqrt(n) + 1; k > 0; k--) {
		if ((n - k * k) % (2 * k) == 0) {
			cout << (n - k * k) / k / 2;
			return 0;
		}
	}
	cout << -1;
	return 0;
}
