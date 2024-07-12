#include <bits/stdc++.h>
#define int long long
using namespace std;

const int colorSize = 4;
const int circleSize = 6;
long long ans;
int vis[circleSize + 1];
void dfs(int idx) {
	if (idx == circleSize + 1) {
		ans++;
		return;
	}
	for (int i = 1; i <= colorSize; i++) {
		if (vis[idx - 1] == i || i == vis[1]) continue;
		if (idx == circleSize && i == vis[2]) continue;
		vis[idx] = i;
		dfs(idx + 1);
	}
}
signed main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	for (int i = 1; i <= colorSize; i++) {
		vis[1] = i;
		dfs(2);
	}
	cout << ans;
	return 0;
}
