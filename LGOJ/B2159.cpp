#include <bits/stdc++.h>
using namespace std;
struct node {
	string name;
	int score;
	void read() {
		cin >> name >> score;
	}
	void print() {
		cout << name << " " << score << '\n';
	}
	bool operator < (const struct node& rhs) const {
		if (score != rhs.score) return score > rhs.score;
		return name < rhs.name;
	}
} a[100];
int n;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i <= n; i++) a[i].read();
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) a[i].print();
	return 0;
}
1 1 1 3 1
2 3


可以对单点 - 任意

或者对连续区间减少1


1 1 1 1 3
2 3
