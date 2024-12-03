#include <bits/stdc++.h>
using namespace std;
int n, ret;
vector<string> a, ans;
void rotate() {
	vector<string> tmp(n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			tmp[i] += a[n - j - 1][i];
	a = tmp;
}
bool get(int x = 5) {
	for (int i = 1; i <= 3; i++) {
		rotate();
		if (a == ans) {
			cout << max(i, x);
			return 1;
		}
	}
	rotate();
	for (auto& v : a) reverse(v.begin(), v.end());
	return 0;
}
int main() {
	cin >> n;
	a = ans = vector<string>(n);
	for (auto& x : a) cin >> x;
	for (auto& x : ans) cin >> x;
	if (get(0)) return 0;
	if (a == ans) return puts("4"), 0;
	if (get(5)) return 0;
	if (a == ans) return puts("6"), 0;
	return puts("7"), 0;
}
