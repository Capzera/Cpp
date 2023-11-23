#include <bits/stdc++.h>
using namespace std;
bool vis[100000010];
int prime[6000010];
int main() {
	int n, q, x, k = 1;
    cin >> n >> q;
	memset(vis, 0, sizeof(vis));
	for(int i = 2; i <= n; i++) {
		if (!vis[i]) prime[k++] = i;
		for(int j = 1; j <= k && prime[j] * i <= n; j++) {
			vis[i * prime[j]] = 1;
            if (i % prime[j] == 0) break;
		}
	}
	while (q--) {
		cin >> x;
		cout << prime[x] << endl;
	}
    system("pause");
	return 0;
}