#include <bits/stdc++.h>
using namespace std;
long long n, k, m, a[5010];
int main() {
	cin >> n >> k >> m;
	for (int i = 1; i <= n; i++) cin >> a[i];
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) a[i] += a[i - 1];
	for (int x = 1, i = 0; x <= m; x++) {
		// x表示早来的小偷背包容积，i表示商铺装进背包最后一个物品的指针
		while (i < n - k && a[i + 1] <= x) i++;
		cout << a[i + k] - a[i] << '\n';
	}
	return 0;
}
