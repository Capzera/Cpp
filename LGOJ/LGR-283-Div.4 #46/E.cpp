#include <bits/stdc++.h>
using namespace std;
struct Time {
	int h, m, k;
	void read() {
		cin >> h >> m >> k;
	}
	void op(int x) {
		m += x;
		if (m >= 60) h += m / 60, m %= 60;
	}
	void show() {
		printf("%d %d\n", h % 24, m);
	}
	bool operator > (const struct Time& rhs) const {
		return h * 60 + m > rhs.h * 60 + rhs.m;
	}
} st, ed;
int main() {
	st.read(), ed.read();
	int k = st.k / ed.k, flag = 0;
	if (st > ed) ed.h += 24;
	while (k--) {
		st.op(ed.k);
		if (st > ed && !flag) {
			st.op(1380);
			flag = 1;
		}
		st.show();
	}
	return 0;
}
