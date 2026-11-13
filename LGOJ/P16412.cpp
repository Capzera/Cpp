#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10;
long long n, cur, a[N];
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	for (int i = 1; i < n; i++) {
		if (a[i] < cur) continue; // 当前a[i] 在前面被检查过了，跳过
		if (a[i] == cur) cur++; // 当前元素就是准备要检查的元素
		if (a[i] > cur) break; // cur已经没有出现过了
	}
	cout << cur * a[n];
	return 0;
}
