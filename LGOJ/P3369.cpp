#include<bits/stdc++.h>
#include <vector>
#define ALL(x) a.begin(), a.end(), x
using namespace std;
int n, op, x;
vector<int> a;
int main() {
	cin >> n;
	while (n--) {
		cin >> op >> x;
		if (op == 1) a.insert(upper_bound(ALL(x)), x);
		if (op == 2) a.erase(lower_bound(ALL(x)));
		if (op == 3) printf("%d\n", lower_bound(ALL(x)) - a.begin() + 1);
		if (op == 4) printf("%d\n", a[x - 1]);
		if (op == 5) printf("%d\n", *--lower_bound(ALL(x)));
		if (op == 6) printf("%d\n", *upper_bound(ALL(x)));
	}
}
