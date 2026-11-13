#include <bits/stdc++.h>
using namespace std;
long long n;
struct stu {
	string name;
	int yw, sx, yy, zf, id;
} a[1001];
bool check(stu a, stu b) {
	if (abs(a.zf - b.zf) > 10) return 0;
	if (abs(a.yw - b.yw) > 5) return 0;
	if (abs(a.sx - b.sx) > 5) return 0;
	if (abs(a.yy - b.yy) > 5) return 0;
	return 1;
}
int main() {
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> a[i].name >> a[i].yw >> a[i].sx >> a[i].yy;
		a[i].zf = a[i].yw + a[i].sx + a[i].yy;
	}
	for (int i = 0; i < n - 1; i++)
		for (int j = i + 1; j < n; j++)
			if (check(a[i], a[j]))
				cout << a[i].name << " " << a[j].name << endl;
	return 0;
}
