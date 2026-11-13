#include <bits/stdc++.h>
using namespace std;
struct node {
	int start, end, val;
	bool operator < (const struct node& rhs) const {
		return end < rhs.end;
	}
} a[5001];
int n, m, ans, vis[30001];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= m; i++) {
		cin >> a[i].start >> a[i].end >> a[i].val;
	}
	sort(a + 1, a + m + 1);
	for (int i = 1; i <= m; i++) {
		int cnt = 0;
		for (int j = a[i].end; j >= a[i].start; j--) {
			cnt += vis[j];
		}
		cnt = a[i].val - cnt;
		for (int j = a[i].end; cnt > 0 && j >= a[i].start; j--) {
			if (vis[j]) continue;
			ans++;
			vis[j] = 1;
			cnt--;
		}
	}
	cout << ans;
	return 0;
}
/*
1 2 3 4 5 6 7 8 9 10 11
    3 3 3 3 3
              3 3 3
          1 1 1
1 1 1
                  1  1
0 0 1 0 0 1 1 1 1 1  0

给3, 6, 7, 8, 9, 10号同学分配1个话筒可以满足所有的要求
[3, 7] 有 3, 6, 7 三个话筒 3 >= 3
[8, 10] 有 8, 9, 10三个话筒 3 >= 3
[6, 8] 有6, 7, 8 三个话筒 3 >= 1
[1, 3] 有 3 一个话筒1 >= 1
[10, 11] 有 10 一个话筒 1 >= 1
全部满足，总共需要至少6个话筒o
ed = 
*/
