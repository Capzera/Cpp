#include <bits/stdc++.h>
using namespace std;
int main() {
	double n;
	double ans;
	cin >> n;
	if (n < 100) {
		ans = n * 0.69;
	} else if (n < 200) {
		ans = 69 + (n - 100) * 0.57;
	} else {
		ans = 69 + 57 + (n - 200) * 0.43;
	}
	cout << ans << endl;
	return 0;
}
