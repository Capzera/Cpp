#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 5, M = 4e8 + 5;
long long n, x, ans;
bool vis[M];
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	while (n--) {
		cin >> x;
		while (x % 3 == 0) {
			if (!vis[x / 3]) {
				vis[x / 3] = 1;
				ans++;
			}
			x /= 3;
		}
	}
	cout << ans;
	return 0;
}

