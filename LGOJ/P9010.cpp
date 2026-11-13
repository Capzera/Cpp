#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int n, ans, L, R, a[N];
char s[N];
int main() {
	cin >> n >> (s + 1);
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) {
		if (s[i] == s[1]) R = i;
	}
	for (int i = 2; i <= n; i++) {
		if (s[i] == s[1]) continue;
		L = i;
		break;
	}
	for (int i = 1; i < L; i++) {
		// i 表示当前枚举的牛的位置
		// L 代表第2种第1头牛位置
		// R 代表第1种牛最后一个位置
		if ((i == 1 && a[i] >= R) || a[i] >= L) ans++;
	}
	cout << ans;
	return 0;
}
