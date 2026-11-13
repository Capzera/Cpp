#include <bits/stdc++.h>
using namespace std;
struct stu {
	int id, score;
	void read() {
		cin >> id >> score;
	}
	bool operator < (const struct stu& rhs) {
		if (score != rhs.score) return score > rhs.score;
		return id < rhs.id;
	}
} a[5010];
int n, m, cnt;
int main() {
	cin >> n >> m;
	m = m * 3 / 2;
	for (int i = 1; i <= n; i++) a[i].read();
	sort(a + 1, a + n + 1);
	cout << a[m].score << " ";
	for (int i = 1; i <= n; i++) {
		if (a[i].score < a[m].score) break;
		cnt++;
	}
	cout << cnt << endl;
	for (int i = 1; i <= cnt; i++) {
		cout << a[i].id << " " << a[i].score << endl;
	}
	return 0;
}
