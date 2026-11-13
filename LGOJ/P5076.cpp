#include <bits/stdc++.h>
using namespace std;
vector<int> tr = {-2147483647, 2147483647};
int n, op, x;
int main() {
	scanf("%d", &n);
	while (n--) {
		scanf("%d%d", &op, &x);
		if (op == 1) {
			printf("%d\n", lower_bound(tr.begin(), tr.end(), x) - tr.begin());
		}
		if (op == 2) {
			printf("%d\n", tr[x]);
		}
		if (op == 3) {
			printf("%d\n", *--lower_bound(tr.begin(), tr.end(), x));
		}
		if (op == 4) {
			printf("%d\n", *upper_bound(tr.begin(), tr.end(), x));
		}
		if (op == 5) {
			tr.insert(upper_bound(tr.begin(), tr.end(), x), x);
		}
	}
	return 0;
}
