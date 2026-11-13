#include <bits/stdc++.h>
using namespace std;
string s;
long long ans;
int main() {
	cin >> s;
	for (int i = s.size() - 1; i >= 0; i--) {
		ans += ans & 1 ^ s[i] - '0' == 0;
	}
	cout << ans;
	return 0;
}
