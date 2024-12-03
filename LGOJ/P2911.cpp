#include <bits/stdc++.h>
using namespace std;
long long s1, s2, s3, mx, ans, a[81];
int main() {
	cin >> s1 >> s2 >> s3;
	for (int i = 1; i <= s1; i++) {
		for (int j = 1; j <= s2; j++) {
			for (int k = 1; k <= s3; k++) {
				a[i + j + k]++;
			}
		}
	}
	for (int i = 3; i <= s1 + s2 + s3; i++) {
		if (a[i] > mx) {
			mx = a[i];
			ans = i;
		}
	}
	cout << ans;
	return 0;
}
