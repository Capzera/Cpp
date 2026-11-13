#include <bits/stdc++.h>
using namespace std;
string a;
long long b;
string div(string a, long long b, int op = 0) {
	long long l1 = a.size(), num = 0;
	string ans;
	for (int i = 0; i < l1; i++) {
		num = num * 10 + a[i] - '0'; // 从被除数拿出一个数和当前临时的num组合
		int x = num / b; // 计算当前部分除以除数的结果
		if (ans != "" || x) ans += x + '0'; // 保存入答案，防止前导0
		num %= b; // num只剩下余数了
	}
	if (ans == "") ans = "0";
	if (!op) return ans;
	return to_string(num);
}
int main() {
	cin >> a >> b;
	cout << div(a, b);
	return 0;
}
