#include <bits/stdc++.h>
using namespace std;
int ans, cnt[] = {1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3, 4, 1, 2, 3, 1, 2, 3, 4};
string s;
int main() {
	getline(cin, s);
	for (auto& x : s) {
		if (x == ' ') ans++;
		else ans += cnt[x - 'a'];
	}
	cout << ans;
	return 0;
}
