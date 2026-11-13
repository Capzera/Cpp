#include <bits/stdc++.h>
using namespace std;
const int N = 1e4 + 10;
int n, op, x, head = 1, tail;
int q[N];
int main() {
	cin >> n;
	while (n--) {
		cin >> op;
		if (op == 1) {
			cin >> x;
			q[++tail] = x;
		}
		if (op == 2) {
			if (head > tail) puts("ERR_CANNOT_POP");
			else head++;
		}
		if (op == 3) {
			if (head > tail) puts("ERR_CANNOT_QUERY");
			else cout << q[head] << '\n';
		}
		if (op == 4) {
			cout << tail - head + 1 << '\n';
		}
	}
	return 0;
}
