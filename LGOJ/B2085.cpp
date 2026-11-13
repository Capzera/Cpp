#include <bits/stdc++.h>
using namespace std;
const int N = 1e7 + 10;
int st[N];
vector<int> prime;
int n, i = 2, cnt;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	while (1) {
		int f = 1, sq = sqrt(i);
		for (int j = 2; j <= sq; j++) { // 判断i是否是质数(从2-根号)试除
			if (i % j == 0) { // 如果存在其余因数，不是质数
				f = 0;
				break;
			} 
		}
		if (f) cnt++; // 是质数，统计量 ++
		if (cnt == n) { // 统计到第n个质数
			cout << i; // 输出当前数字
			break;
		}
		i++;
	}
	return 0;
}

