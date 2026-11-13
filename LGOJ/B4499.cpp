#include <bits/stdc++.h>
using namespace std;
int n, ans;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		int num = i;
		string s;
		while (num) {
			s += num % 2 + '0';
			num /= 2;
		}
		string t = s;
		reverse(t.begin(), t.end());
		ans += s == t;
	}
	cout << ans;
	return 0;
}

