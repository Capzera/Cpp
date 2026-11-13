#include <bits/stdc++.h>
using namespace std;
const int dx[] = {-1, 0, 1, 0}, dy[] = {0, 1, 0, -1};
vector<int> node, info;
char a[11][11];
int tx, ty, cx, cy, dt, df;
map<vector<int>, int> mp;
void op(int &x, int& y, int& d) {
	int mx = x + dx[d], my = y + dy[d];
	if (mx < 1 || mx > 10 || my < 1 || my > 10 || a[mx][my] == '*') {
		d = (d + 1) % 4;
		return;
	}
	x = mx, y = my;
}
int main() {
	for (int i = 1; i <= 10; i++) {
		for (int j = 1; j <= 10; j++) {
			cin >> a[i][j];
			if (a[i][j] == 'C') tx = i, ty = j, a[i][j] = '.';
			if (a[i][j] == 'F') cx = i, cy = j, a[i][j] = '.';
		}
	}
	queue<vector<int> > q;
	node = {tx, ty, cx, cy, 0, 0};
	q.push(node);
	mp[node] = 0;
	while (q.size()) {
		info = q.front(); q.pop();
		tx = info[0], ty = info[1], cx = info[2], cy = info[3], dt = info[4], df = info[5];
		if (tx == cx && ty == cy) {
			cout << mp[info];
			return 0;
		}
		op(tx, ty, dt), op(cx, cy, df);
		node = {tx, ty, cx, cy, dt, df};
		if (mp.count(node)) {
			cout << 0;
			return 0;
		}
		mp[node] = mp[info] + 1;
		q.push(node);
	}
	cout << 0;
	return 0;
}
