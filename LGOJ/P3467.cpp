#include <bits/stdc++.h>
using namespace std;
const int N = 3e5 + 10;
int n, t, x, ans, top, stk[N];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> t >> x;
		while (top && x <= stk[top]) {
			if (x == stk[top]) ans++;
			top--;
		}
		stk[++top] = x;
	}
	cout << n - ans;
	return 0;
}
