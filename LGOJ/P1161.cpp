#include <bits/stdc++.h>
using namespace std;
long long n, t, ans;
double a;
int main() {
	cin >> n;
	while (n--) {
		cin >> a >> t;
		for (int i = 1; i <= t; i++) {
			ans ^= int(i * a);
		}
	}
	cout << ans;
	return 0;
}
