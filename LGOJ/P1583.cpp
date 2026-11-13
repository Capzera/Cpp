#include <bits/stdc++.h>
using namespace std;
long long n, k;
int e[11];
struct node {
	long long id, w;
	bool operator < (const struct node& rhs) {
		if (w != rhs.w) return w > rhs.w;
		return id < rhs.id;
	} 
} a[20010];
int main() {
	cin >> n >> k;
	for (int i = 1; i <= 10; i++) cin >> e[i];
	for (int i = 1; i <= n; i++) cin >> a[i].w, a[i].id = i;
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) {
		a[i].w += e[(i - 1) % 10 + 1]; 
	}
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= k; i++) cout << a[i].id << " ";
	return 0;
}
/*
21 20 19 18 17 16 15 14 13 12

*/
