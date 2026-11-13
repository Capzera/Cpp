#include <bits/stdc++.h>
using namespace std;
long long n, L, R;
int main() {
	cin >> n >> L >> R;
	while (n--) {
		if (L >= 1000000) {
			cout << 1000000 << " ";
			L -= 1000000;
		} else if (L <= -1000000) {
			cout << -1000000 << " ";
			L += 1000000;
		} else {
			cout << L << " ";
			L = 0;
		}
	}
	return 0;
}
