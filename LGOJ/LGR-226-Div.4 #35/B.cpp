#include <bits/stdc++.h>
using namespace std;
long long w, f, a, r;
bool check() { // 判反不判正
	if (w < r) return 0;
	if (f > 0.2 * w) return 0;
	if (a > 0.3 * w) return 0;
	return 1;
}
int main() {
	cin >> w >> f >> a >> r;
	cout << (check() ? "Accepted" : "Rejected");
	return 0;
}
/*
检验机类的题，一律写一个检查函数
当条件不满足其中某个点的时候，函数可以直接返回，但程序不好直接退出
*/
