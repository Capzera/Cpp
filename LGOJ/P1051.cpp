#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 5;
struct stu {
	string name;
	int g_score, c_score, number, money, id;
	char isLeader, isWest;
	void read() {
		cin >> name >> g_score >> c_score >> isLeader >> isWest >> number;
		if (g_score > 80 && number > 0) money += 8000;
		if (g_score > 85 && c_score > 80) money += 4000;
		if (g_score > 90) money += 2000;
		if (g_score > 85 && isWest == 'Y') money += 1000;
		if (c_score > 80 && isLeader == 'Y') money += 850;
	}
	bool operator < (const struct stu& rhs) const {
		if (money != rhs.money) return money > rhs.money;
		return id < rhs.id;
	}
} a[N];
int n, ans;
long long sum;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i <= n; i++) {
		a[i].read();
		a[i].id = i;
		sum += a[i].money;
	}
	sort(a + 1, a + n + 1);
	cout << a[1].name << '\n' << a[1].money << '\n' << sum;
	return 0;
}
