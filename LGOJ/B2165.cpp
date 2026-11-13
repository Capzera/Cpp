#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
int t, ans, top, stk[N];
string s;
int solve() {
	cin >> s;
	top = 0;
	int n = s.size();
	for (int i = 0; i < n; i++) {
		if (!top || s[i] != stk[top]) {
			stk[++top] = s[i];
		} else {
			top--;
		}
	}
	return top == 0;
}

int main() {
	cin >> t;
	while (t--) ans += solve();
	cout << ans;
	return 0;
}
