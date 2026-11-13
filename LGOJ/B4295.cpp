#include <bits/stdc++.h>
using namespace std;
int x, y, k, n;
int head = 0, tail = 0, vis[201], q[201];
int main() {
	cin >> x >> y >> k;
	n = x + y;
	for (int i = 0; i < n; i++) {
		q[tail++] = i + 1;
	}
	while (y--) {
		for (int i = 1; i < k; i++) {
			q[tail] = q[head];
			tail = (tail + 1) % 201;
			head = (head + 1) % 201;
		}
		vis[q[head]] = 1;
		head = (head + 1) % 201;
	}
	for (int i = 1; i <= n; i++) {
		if (vis[i]) continue;
		cout << i << " ";
	}
	return 0;
}
