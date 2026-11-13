#include <bits/stdc++.h>
using namespace std;
int n;
struct bg {
	string name, actor;
	int cont, level, id;
	bool operator < (const struct bg& rhs) {
		if (cont == rhs.cont) return id < rhs.id;
		return cont > rhs.cont;
	}
} a[111];
bool cmp(const struct bg& lhs, const struct bg& rhs) {
	if (lhs.level != rhs.level) return lhs.level > rhs.level;
	return lhs.id < rhs.id;
}
int main() {
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> a[i].name >> a[i].actor >> a[i].cont >> a[i].level;
		a[i].id = i;
	}
	sort(a + 3, a + n);
	for (int i = 3; i < n; i++) {
		if (i < 5) a[i].actor = "HuFa";
		else if (i < 9) a[i].actor =  "ZhangLao";
		else if (i < 16) a[i].actor = "TangZhu";
		else if (i < 41) a[i].actor = "JingYing";
		else a[i].actor = "BangZhong";
	}
	sort(a + 3, a + min(5, n), cmp);
	if (n > 4) sort(a + 5, a + min(9, n), cmp);
	if (n > 8) sort(a + 9, a + min(16, n), cmp);
	if (n > 15) sort(a + 16, a + min(41, n), cmp);
	if (n > 40) sort(a + 41, a + n, cmp);
	for (int i = 0; i < n; i++) {
		cout << a[i].name << " " << a[i].actor << " " << a[i].level << endl;
	}
	return 0;
}
