#include <bits/stdc++.h>
using namespace std;
long long t, x, y;
int main() {
	cin >> t;
	while (t--) {
		cin >> x >> y;
		int p = 0; // 统计游戏进行了几轮
		while (x && y) {
			if (x < y) x++; // 小，就拿一块
			else x /= 2; // 多，就给一半
			swap(x, y); // 每操作一次后，轮流执行
			p++; // 游戏轮数++
		}
		if (p % 2) swap(x, y); // 如果游戏进行了奇数轮，x, y处于交换状态
		cout << x << "]\
" << y << '\n';
	}
	return 0;
}

