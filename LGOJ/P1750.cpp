#include <bits/stdc++.h>
using namespace std;
int n, m, x, a[10001];
stack<int> stk;
int main() {
	cin >> n >> m;
	for (int i = 1, j = 1; i <= n; i++) {
		cin >> a[i];
		if (stk.empty() || a[i] < stk.top()) {
			while (j <= i) stk.push(a[j++]);
		}
		if (stk.size() + (i - j + 1) >= m || i == n) {
			cout << stk.top() << " ";
			stk.pop();
		}
	}
	while (stk.size()) {
		cout << stk.top() << " ";
		stk.pop();
	}
	return 0;
}
