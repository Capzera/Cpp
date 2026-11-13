#include <bits/stdc++.h>
using namespace std;
long long k, ans;
string s;
int main() {
	cin >> k >> s;
	for (int i = s.size() - 1, j = 1; i >= 0; i--, j *= k) {
		if (isdigit(s[i])) {
			ans += j * (s[i] - '0');
		} else {
			ans += j * (s[i] - '7');
		}
	}
	cout << ans;
	return 0;
}

