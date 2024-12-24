#include <bits/stdc++.h>
using namespace std;
string s, ans;
int n, x, y;
void dfs(int i, int x, int y, string s) {
	if (i == n) {
		ans = max(ans, s);
		return;
	}
	char ori = s[i];
	int nx = '9' - s[i];
	int ny = s[i] - '0' + 1;
	if (nx <= x) {
		s[i] = '9';
		dfs(i + 1, x - nx, y, s);
		s[i] = ori;
	}
	if (ny <= y) {
		s[i] = '9';
		dfs(i + 1, x, y - ny, s);
		s[i] = ori;
	}
	if (nx > x && ny > y) {
		s[i] += x;
		dfs(i + 1, 0, y, s);
		s[i] = ori;
	}
}
int main() {
	cin >> s >> x >> y;
	n = s.size();
	dfs(0, x, y, s);
	cout << ans;
	return 0;
}
