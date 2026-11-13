#include <bits/stdc++.h>
using namespace std;
vector<string> fun(int n) {
	if (n == 1) {
		return {"X"};
	}
	vector<string> lst = fun(n - 1);
	int m = lst.size();
	vector<string> cur(m * 3);
	string space(m, ' ');
	for (int i = 0; i < m; i++) {
		cur[i] = cur[i + 2 * m] = lst[i] + space + lst[i];
		cur[i + m] = space + lst[i] + space;
	}
	return cur;
}
int n;
void solve(int n) {
	vector<string> ans = fun(n);
	for (auto& x : ans) cout << x << endl;
	cout << '-' << endl;
}
int main() {
	while (cin >> n, n != -1) solve(n);
	return 0;
}
