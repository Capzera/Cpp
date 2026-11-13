#include <bits/stdc++.h>
using namespace std;
int k, n;
int head = 0, tail = 0, q[101];
int main() {
	cin >> n >> k;
	for (int i = 0; i < n; i++) q[tail++] = i + 1;
	while (n--) {
		for (int i = 1; i < k; i++) {
			q[tail] = q[head];
			tail = (tail + 1) % 101;
			head = (head + 1) % 101;
		}
		cout << q[head] << " ";
		head = (head + 1) % 101;
	}
	return 0;
}
