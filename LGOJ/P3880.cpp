#include <bits/stdc++.h>
using namespace std;
string str, s2;
long long n, alpha, vowel, flag, loc = INT_MAX;
set<char> se = {'a', 'e', 'i', 'o', 'e', 'A', 'E', 'I', 'O', 'E'};
int main() {
	getline(cin, str);
	n = str.size();
	for (int i = 0; i < n; i++) { // 提示1，把所有字母替换为'.'
		if (isalpha(str[i])) cout << '.', alpha++; // alpha 统计字母的个数
		else cout << str[i];
	}
	cout << endl;
	long long one_third = round(1.0 * alpha / 3); // 获取1/3字母个数四舍五入，round是四舍五入函数
	long long two_third = round(2.0 * alpha / 3); // 获取2/3字母个数四舍五入，round是四舍五入函数
	for (int i = 0, j = one_third; i < n; i++) { // 提示2，把1/3字母用答案替换
		if (isalpha(str[i]) && !j) s2 += '.';
		else s2 += str[i];
		if (isalpha(str[i]) && j) {
			j--;
			if (!j) loc = i; // 找出第1/3字母的位置，方便接下来元音字母的查找
		}
		if (se.count(str[i]) && i > loc) { // 如果剩下2/3存在元音，则标记为1
			flag = 1;
		}
	}
	cout << s2 << endl;
	if (flag) { // 如果接下来存在元音字母
		for (int i = loc + 1; i < n; i++) { // 从loc + 1开始，把所有元音字母用原始字母替代
			if (se.count(str[i])) s2[i] = str[i]; 
		}
		cout << s2;
		return 0;
	}
	for (int i = 0, j = two_third; i < n; i++) { // 不存在元音字母，把2/3的字母用原始字母替代
		cout << (isalpha(str[i]) && !j ? '.' : str[i]);
		if (isalpha(str[i]) && j) j--;
	}
	return 0;
}

