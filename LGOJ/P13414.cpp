#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int n, ans, top, stk[N];
string s;
int main() {
	cin >> n;
	while (n--) {
		cin >> s;
		top = 0;
		for (auto& x : s) {
			if (stk[top] == x) {
				top--;
				continue;
			}
			stk[++top] = x;
		}
		ans += top == 0;
	}
	cout << ans;
	return 0;
}
