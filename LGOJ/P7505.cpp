#include <bits/stdc++.h>
using namespace std;
const int N = 3e5 + 10;
int n, m, k, op, x, a[N];
long long sum, head = 1, tail = 0, q[N];
int main() {
	cin >> n >> m >> k;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) q[++tail] = a[i];
	while (m--) {
		cin >> op;
		if (op == 1) {
			cin >> x;
			sum += x;
			while (head <= tail && q[tail] + sum > k) tail--;
			
		}
		if (op == 2) {
			cin >> x;
			sum -= x;
			while (head <= tail && q[head] + sum < -k) head++;
		}
		if (op == 3) {
			printf("%lld\n", tail - head + 1);
		}
	}
	return 0;
}
