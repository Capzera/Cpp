#include <bits/stdc++.h>
using namespace std;
int a, b, ans;
int month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int main() {
	cin >> a >> b;
	ans = month[b];
	if (b == 2 && (a % 400 == 0 || a % 4 == 0 && a % 100 != 0)) {
		ans++;
	}
	cout << ans;
	return 0;
}
