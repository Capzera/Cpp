#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
unsigned long long a[N];
int n, k, head = 1, tail = 0, q[N];
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) {
		while (head <= tail && a[q[tail]] <= a[i]) tail--;
		q[++tail] = i;
		while (head <= tail && q[head] <= i - k) head++;
		if (i >= k) printf("%d\n", tail - head + 1);
	}
	return 0;
}
