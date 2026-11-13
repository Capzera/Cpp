#include <bits/stdc++.h>
using namespace std;
int n, k;
struct node {
	int id;
	double avg = 0;
	void read(int i) {
		int x;
		for (int i = 1; i <= k; i++) {
			cin >> x;
			avg += x;
		}
		avg /= k; 
		id = i;
	}
	bool operator < (const struct node& rhs) const {
		if (avg != rhs.avg) return avg > rhs.avg;
		return id < rhs.id;
	}
} a[105];

int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> k;
	for (int i = 1; i <= n; i++) a[i].read(i);
	sort(a + 1, a + n + 1);
	cout << a[1].id << '\n' << a[2].id;
	return 0;
}
