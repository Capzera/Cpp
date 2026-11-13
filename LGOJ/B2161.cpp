#include <bits/stdc++.h>
using namespace std;
int n;
string ans;
int main() {
	cin >> n;
	if (!n) return puts("0"), 0;
	while (n) {
		ans += '0' + n % 2;
		n /= 2;
	}
	reverse(ans.begin(), ans.end());
	cout << ans;
	return 0;
}


