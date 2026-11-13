#include <bits/stdc++.h>
using namespace std;
long long n;
struct stu {
	string name;
	int yw, sx, yy, zf, id;
	bool operator < (stu& rhs) {
		if (zf != rhs.zf) return zf > rhs.zf;
		return id < rhs.id;
	}
} a[1001];
int main() {
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> a[i].name >> a[i].yw >> a[i].sx >> a[i].yy;
		a[i].zf = a[i].yw + a[i].sx + a[i].yy;
		a[i].id = i;
	}
	sort(a, a + n);
	cout << a[0].name << " " << a[0].yw << " " << a[0].sx << " " << a[0].yy;
	return 0;
}
