#include <bits/stdc++.h>
using namespace std;
int n, op, x, mx;
set<int> mp;
int main() {
	cin >> n;
	while (n--) {
		cin >> op >> x;
		if (op == 1) {
			if (!mp.insert(x).second) puts("Already Exist");
		} else {
			if (mp.empty()) {
				puts("Empty");
			} else if (mp.count(x)){
				cout << x << '\n';
				mp.erase(x);
			} else {
				auto lo = mp.lower_bound(x), hi = lo;
				if (lo == mp.begin()) cout << *lo << '\n', mp.erase(lo);
				else if (hi == mp.end()) cout << *(--hi) << '\n', mp.erase(hi);
				else {
					int ans = (*hi - x < x - *(--lo) ? *hi : *lo);
					cout << ans << "\n";
					mp.erase(ans);
				}
			}
			
		}
	}
	return 0;
}
/*
set = {1, 5}



3
2 3
2 3
2 3

*/
