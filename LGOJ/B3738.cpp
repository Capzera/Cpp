#include <bits/stdc++.h>
using namespace std;
bool st[3000];
vector<int> prime;
int n, x, y, a[21][21];
int dx[] = {0, 1, 0, -1}, dy[] = {1, 0, -1, 0};
int main() {
	cin >> n >> x >> y;
	for (int i = 2; i <= 3000; i++) {
		if (!st[i]) prime.push_back(i);
		for (int j = 0; j < prime.size() && i * prime[j] <= 3000; j++) {
			st[i * prime[j]] = 1;
			if (i % prime[j] == 0) break;
		}
	}
	memset(a, -1, sizeof(a));
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			a[i][j] = 0;
	for (int x = 1, y = 1, dir = 0, i = 0; i < n * n; i++, x += dx[dir], y += dy[dir]) {
		a[x][y] = prime[i];
		int mx = x + dx[dir], my = y + dy[dir];
		if (a[mx][my]) dir = (dir + 1) % 4;
	}
	cout << a[x][y];
	return 0;
}
