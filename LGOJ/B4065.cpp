#include <bits/stdc++.h>
using namespace std;
int n, ans;
string s;
int main() {
	cin >> n;
	while (n--) {
		cin >> s;
		int sum = 0;
		for (auto& ch : s) sum += ch - '0';
		ans = max(ans, sum);
	}
	cout << ans;
	return 0;
}
