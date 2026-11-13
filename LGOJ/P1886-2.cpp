#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
int n, k, head = 1, tail = 0;
int a[N], q[N];
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) {
		while (head <= tail && q[head] <= i - k) head++;
		while (head <= tail && a[q[tail]] >= a[i]) tail--;
		q[++tail] = i;
		if (i >= k) cout << a[q[head]] << " ";
	}
	cout << "\n";
	head = 1, tail = 0;
	for (int i = 1; i <= n; i++) {
		while (head <= tail && q[head] <= i - k) head++;
		while (head <= tail && a[q[tail]] <= a[i]) tail--;
		q[++tail] = i;
		if (i >= k) cout << a[q[head]] << " ";
	}
	return 0;
}
