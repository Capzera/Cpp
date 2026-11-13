#include <bits/stdc++.h>
using namespace std;
const int N = 5010;
struct stu {
	int id, score;
	void scanf() {
		cin >> id >> score;
	}
	void show() {
		cout << id << " " << score << '\n';
	}
	bool operator <(const struct stu& rhs) const {
		return score > rhs.score;
	}
	bool operator < (const struct 结构体名& rhs) const {
		return weight > rhs.weight;
	}
} a[N];

int n, m;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) a[i].scanf();
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) a[i].show();
	return 0;
}

