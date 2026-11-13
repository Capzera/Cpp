#include <bits/stdc++.h>
using namespace std;
const int N = 150;
int n, ans;
struct patient {
	string id;
	int rk, age, old = 0;
	void read() {
		cin >> id >> age;
		if (age > 59) old = 1;
	}
	bool operator < (const struct patient& rhs) const {
		if (old != rhs.old) return old > rhs.old;
		if (old == 1 && age != rhs.age) return age > rhs.age;
		return rk < rhs.rk;
	}
} a[N];
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i <= n; i++) {
		a[i].read();
		a[i].rk = i;
	}
	sort(a + 1,a + n + 1);
	for (int i = 1; i <= n; i++) {
		cout << a[i].id << "\n";
	}
	return 0;
}
