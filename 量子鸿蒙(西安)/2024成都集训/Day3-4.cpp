#include <bits/stdc++.h>
using namespace std;
int main() {
	int n;
	cin >> n;
	vector<int> a(n);
	for (auto& x : a) cin >> x; // 自动遍历输入数组

	int n, x;
	cin >> n;
	vector<int> a;
	for (int i = 0; i < n; i++) {
		cin >> x;
		a.push_back(x); // 使用push_back输入
	}

	int n;
	cin >> n;
	vector<int> a(n);
	for (int i = 0; i < n; i++) {
		cin >> a[i]; // 传统输入
	}

	vector<int> a = {1, 5, 2, 4, 3};
	reverse(a.begin(), a.end());
	
	
	vector<int> a(1000);
	iota(a.begin(), a.end(), 10);
	a = {10, 11, 12, 13, 14, ... , 1009};
	iota(a.begin(), a.begin() + 3, 3);
	
	
	sort(a.begin(), a.end()); // 从小到大
	sort(a.begin(), a.end(), greater<int>()); // 动态数组专用从大到小排序
	sort(a, a + n);
	sort(a + 1, a + n + 1);
	
	
	
	vector<int> a = {1, 5, 2, 4, 3};
	a.resize(3);
	修改后长度小于原长度 --> 值丢失
	修改后长度等于原长度 --> 无事发生
	修改后长度大于原长度 --> 自动填充为0
	a.resize(6, val);
	修改后长度大于原长度 --> 自动填充为val
	return 0;
}
