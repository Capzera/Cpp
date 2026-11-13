#include <bits/stdc++.h>
using namespace std;
long long n, m, a, b, x, mx, id, f;
struct music{
	long long id, love;
	bool operator < (const struct music &rhs) {
		return love > rhs.love;
	}
} ans[100010];
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m >> a >> b;
	for (int i = 1; i <= n; i++) ans[i].id = i;
	for (int i = 1; i <= a; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> x;
			ans[j].love += x;
			if (i == b && x > mx) {
				mx = x;
				id = j;
			} 
		}
	}
	sort(ans + 1, ans + n + 1);
	for (int i = 1; i < m; i++) {
		if (ans[i].id == id) {
			f = 1;
			for (int j = i; j > 1; j--) ans[j].id = ans[j - 1].id;
			ans[1].id = id;
			break;
		}
	}
	if (!f) ans[m].id = id;
	for (int i = 1; i <= m; i++) cout << ans[i].id << " ";
	return 0;
}
