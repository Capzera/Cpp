#include <bits/stdc++.h>
using namespace std;
queue<int> q;
short T, f[1 << 26];
char ch;
int op(int x, int y) {
	x ^= 1 << y;
	if (y > 4) x ^= 1 << y - 5; // 不在第一排
	if (y < 20) x ^= 1 << y + 5; // 不在最后一排
	if (y % 5) x ^= 1 << y - 1; // 不在第一列
	if (y % 5 != 4) x ^= 1 << y + 1; // 不在最后一列
	return x;
}

void bfs() {
	memset(f, -1, sizeof(f));
	q.push((1 << 25) - 1);
	f[(1 << 25) - 1] = 0;
	while (q.size()) {
		int x = q.front(); q.pop();
		if (f[x] >= 6) break;
		for (int y = 0; y < 25; y++) {
			int st = op(x, y);
			if (f[st] == -1) {
				q.push(st);
				f[st] = f[x] + 1;
			}
		}
	}
	return;
}

int solve() {
	int mask = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cin >> ch;
			if (ch == '1') mask |= 1 << i * 5 + j;
		}
	}
	return f[mask];
}
	
int main() {
	bfs();
	cin >> T;
	while (T--) {
		cout << solve() << endl;
	}
	return 0;
}
