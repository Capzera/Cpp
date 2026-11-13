#include <bits/stdc++.h>
using namespace std;
int sy = 2239, sm = 9, sd = 9;
int ey = 9876, em = 1, ed = 1;
int mon[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int ans;
int main() {
//	for (int y = sy; y <= ey; y++) {
//		for (int m = 1; m <= (y < ey ? 12 : 1); m++) {
//			if (m == 2) {
//				if (y % 400 || y % 4 == 0 && y % 100) mon[2] = 29;
//				else mon[2] = 28;
//			}
//			for (int d = 1; d <= (y == ey && m == em ? 1 : mon[m]); d++) {
//				string date = to_string(y) + to_string(m) + to_string(d);
//				vector<int> cnt(10), times;
//				int flag = 1;
//				for (auto& x : date) cnt[x - '0']++;
//				for (auto& x : cnt) {
//					if (!x) continue;
//					if (times.size() && x != times.back()) flag = 0;
//					times.push_back(x);
//				}
//				ans += flag;
//			}
//		}
//	}
//	cout << ans - 2;
	cout << 210778;
	return 0;
}
