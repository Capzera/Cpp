#include <bits/stdc++.h>
using namespace std;
int n, m, sz, head = 0, tail = 0, q[101];
int main() {
	cin >> n >> m;
	sz = n;
	for (int i = 1; i <= n; i++) q[tail++] = i;
	while (sz) {
		for (int i = 1; i < m; i++) {
			q[tail] = q[head];
			tail = (tail + 1) % sz;
			head = (head + 1) % sz;
		}
		cout << q[head] << " ";
		head = (head + 1) % sz;
		sz--;
	}
	return 0;
}
