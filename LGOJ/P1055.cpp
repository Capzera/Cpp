#include <bits/stdc++.h>
using namespace std;
string s;
int num, code;
int main() {
	cin >> s;
	for (int i = 0, j = 1; i < 11; i++) {
		if (s[i] == '-') continue; // 如果是'-' 跳过
		num += (s[i] - '0') * j; // 否则进行IBSN求和
		j++; // 下一次的权重 + 1
	}
	num %= 11; // 后续都需要num % 11，直接操作完了再做判断
	if (s[12] == 'X') code = 10; // 判断是X的情况
	else code = s[12] - '0';
	if (num == code) { // 如果识别码正确
		cout << "Right";
	}
	else {
		if (num != 10) s[12] = num + '0'; // 如果正确识别码不为10
		else s[12] = 'X'; // 否则将最后一位改成X
		cout << s;
	}
	return 0;
}
