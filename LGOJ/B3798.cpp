#include <bits/stdc++.h>
using namespace std;
struct stu {
	string name;
	double s1, s2;
	
	bool operator < (const stu& rhs) {
		if (s2 != rhs.s2) return s2 > rhs.s2;
		return name < rhs.name;
	}
}a[100010];
long long n, m, x, s[21];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		double sum = 0;
		cin >> a[i].name;
		for (int j = 1; j <= m; j++) cin >> s[j], a[i].s1 += s[j];
		a[i].s1 /= m;
		for (int j = 1; j <= m; j++) a[i].s2 += pow(a[i].s1 - s[j], 2);
	}
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= min(n, 1ll * 20); i++) cout << a[i].name << endl;
	return 0;
}
