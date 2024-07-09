#include <bits/stdc++.h>
using namespace std;
int main() {
	int n;
	cin >> n;
	if (n <= 3) cout << 8 << endl;
	else {
		double ans = 1.6 * n + 3.2;
		cout << ans << endl;
	}
	return 0;
}
