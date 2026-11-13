#include <bits/stdc++.h>
using namespace std;
int n, k, a[2001];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		if (a[i] == n) k = i; // 存储末尾元素初始的位置
	}
	k = n - k == 0 ? n : n - k;  // k 存储总共需要旋转的次数
	for (int i = 1; i <= k; i++) { // 当前旋转次数为i，数组的后i位应处于数组的前部
		for (int j = n - i + 1; j <= n; j++) cout << a[j] << " ";
		for (int j = 1; j <= n - i; j++) cout << a[j] << " ";
		cout << '\n';
	}
	return 0;
}
