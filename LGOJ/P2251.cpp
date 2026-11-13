#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
int n, m, a[N];
int head = 1, tail, q[N];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
	for (int i = 1; i <= n; i++) {
		while (head <= tail && q[head] <= i - m) head++;
		while (head <= tail && a[q[tail]] > a[i]) tail--;
		q[++tail] = i;
		if(i >= m) printf("%d\n", a[q[head]]);
	}
	return 0;
}
