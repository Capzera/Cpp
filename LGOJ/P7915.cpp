#include <bits/stdc++.h>
using namespace std;

int n, a[1000005];
vector<array<int,2>> pos;   // pos[x] = {第一次出现位置, 第二次出现位置}

string solve(int first) {
	string ans;
	int l, r, p, x;
	if (first == 0) {               // 第一步取 L
		ans = "L";
		x = a[1];
		p = (pos[x][0] == 1 ? pos[x][1] : pos[x][0]);
		l = 2; r = 2 * n;
	} else {                        // 第一步取 R
		ans = "R";
		x = a[2 * n];
		p = (pos[x][0] == 2 * n ? pos[x][1] : pos[x][0]);
		l = 1; r = 2 * n - 1;
	}
	
	vector<int> st;
	st.push_back(x);                // 第一步取出的数入栈
	
	while (l <= r) {
		if (l == p) {               // p 必须留到最后
			ans += 'R';
			int y = a[r];
			if (!st.empty() && st.back() == y) st.pop_back();
			else st.push_back(y);
			r--;
		} else if (r == p) {
			ans += 'L';
			int y = a[l];
			if (!st.empty() && st.back() == y) st.pop_back();
			else st.push_back(y);
			l++;
		} else {
			if (!st.empty() && st.back() == a[l]) {
				ans += 'L';
				st.pop_back();
				l++;
			} else if (!st.empty() && st.back() == a[r]) {
				ans += 'R';
				st.pop_back();
				r--;
			} else {
				int other_l = (pos[a[l]][0] == l ? pos[a[l]][1] : pos[a[l]][0]);
				int other_r = (pos[a[r]][0] == r ? pos[a[r]][1] : pos[a[r]][0]);
				if (other_l == r) {
					ans += 'L';
					st.push_back(a[l]);
					l++;
				} else if (other_r == l) {
					ans += 'R';
					st.push_back(a[r]);
					r--;
				} else if (other_l == p) {
					ans += 'R';
					st.push_back(a[r]);
					r--;
				} else if (other_r == p) {
					ans += 'L';
					st.push_back(a[l]);
					l++;
				} else {
					ans += 'L';
					st.push_back(a[l]);
					l++;
				}
			}
		}
	}
	
	if (st.empty()) return ans;
	return "-1";
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	
	int T;
	cin >> T;
	while (T--) {
		cin >> n;
		pos.assign(n + 1, {-1, -1});
		for (int i = 1; i <= 2 * n; i++) {
			cin >> a[i];
			if (pos[a[i]][0] == -1) pos[a[i]][0] = i;
			else pos[a[i]][1] = i;
		}
		
		string ans = solve(0);
		if (ans == "-1") ans = solve(1);
		cout << ans << '\n';
	}
	return 0;
}
