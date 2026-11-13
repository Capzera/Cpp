#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
inline long long read() {
	long long ans = 0, f = 1;
	char ch = getchar();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar();
	}
	return ans * f;
}
void write(long long x) {
	if (x < 0) putchar('-'), x = -x;
	if (x > 9) write(x / 10);
	putchar(x % 10 + '0');
}
struct node {
	long long loc;
	int id;
	bool operator < (const struct node& rhs) const {
		if (loc != rhs.loc) return loc < rhs.loc;
		return id < rhs.id;
	}
} a[N];
int n, p, mx, start, gift;
long long m;
vector<int> ans, idx;
int main() {
	n = read(), m = read(), p = read();
	for (int i = 1; i <= n; i++) {
		a[i].loc = read();
		a[i].id = i;
	}
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) {
		if (a[i].id != p) continue;
		gift = i;
	}
	for (int i = 1, j = 1; i <= n; i++) { // 令当前最左边能访问的点为排序后的 i 号点
		int ext = 0;
		if (i <= gift) {
			if (a[gift].loc <= 0 && -2ll * a[i].loc <= m + 5) ext = 5;
			if (a[i].loc <= 0 && a[gift].loc >= 0 && 2ll * (a[gift].loc - a[i].loc) <= m + 5) ext = 5;
			if (a[i].loc >= 0 && 2ll * a[gift].loc <= m + 5) ext = 5;
		}
		if (i == gift + 1) j = i;
		if (2ll * abs(a[i].loc) > m + ext) continue; // 当前点本身回不去
		j = max(j, i);
		while (j <= n) { // 往右延伸
			if (a[j].loc <= 0) j++;
			else if (a[i].loc <= 0 && a[j].loc >= 0 && 2ll * (a[j].loc - a[i].loc) <= m + ext) j++;
			else if (a[i].loc >= 0 && 2ll * a[j].loc <= m + ext) j++;
			else break;
		}
		if (j - i > mx) {
			mx = j - i;
			ans.clear();
		}
		if (j - i == mx) ans.push_back(i);
	}
	cout << mx << '\n';
	for (auto& st : ans) {
		vector<int> tmp(mx);
		for (int i = 0; i < mx; i++) {
			tmp[i] = a[st + i].id;
		}
		sort(tmp.begin(), tmp.end());
		if (idx.empty() || tmp < idx) {
			idx = tmp;
			start = st;
		}
	}
	for (int i = 0; i < mx; i++) write(a[start + i].id), putchar(' ');
	return 0;
}
