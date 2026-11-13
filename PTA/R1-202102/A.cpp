#include <bits/stdc++.h>
using namespace std;
string s;
int ans;
int main() {
	while (cin >> s) {
		ans += s.size();
	}
	cout << ans;
	return 0;
}
