#include <bits/stdc++.h>
using namespace std;
long long n, k, slide, ans;
long long a[1010];
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1); // 约1s
	for (int r = 1, l = 1; r <= n; r++) {
		
	}
	cout << ans;
	return 0;
}
/*
5 1
1 1 2 2 3
    [     ]
slide = 7
相同大小的物品被集中在一个位置

*/
