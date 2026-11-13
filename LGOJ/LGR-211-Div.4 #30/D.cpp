#include <bits/stdc++.h>
using namespace std;
long long n, x, y, s;
int main() {
	cin >> n;
	while (n--) {
		cin >> x;
		double y = 0;
		for (int i = 1; i <= x; i++) {
			cin >> s;
			if (!s) y++;
		}
		printf("%4lf\n", y / x);
	}
	return 0;
}
