#include <bits/stdc++.h>
using namespace std;
const int N = 1e3 + 10;
struct node {
	double w, h, bmi;
	int id;
	bool operator < (const struct node& rhs) const {
		return bmi > rhs.bmi;
	}
} a[N];
int n;

int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i].w;
	for (int i = 1; i <= n; i++) {
		cin >> a[i].h;
		a[i].id = i;
		a[i].bmi = a[i].w / a[i].h / a[i].h;
	}
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) cout << a[i].id << " ";
	return 0;
}
