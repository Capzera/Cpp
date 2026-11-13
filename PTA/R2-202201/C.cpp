#include <bits/stdc++.h>
using namespace std;
struct node {
	int name, score, id, rank;
	void read(int i) {
		id = i;
		cin >> name >> score;
	}
	void show() {
		printf("%d %d %d\n", rank, name, score);
	}
	bool operator < (const struct node& rhs) const {
		if (score != rhs.score) return score > rhs.score;
		return id < rhs.id;
	}
	bool operator == (const struct node& rhs) const {
		return score == rhs.score;
	}
} a[1001];
int n;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) a[i].read(i);
	sort(a + 1, a + n + 1);
	for (int i = 1, lst = 1; i <= n; i++) {
		a[i].rank = a[i] == a[i - 1] ? a[i - 1].rank : i;
		a[i].show();
	}
	return 0;
}
