#include <bits/stdc++.h>
using namespace std;
string a, b;
string add(string s1, string s2) {
	string ans;
	int l = s1.size() - 1, r = s2.size() - 1, carry = 0;
	while (l >= 0 || r >= 0) {
		int a = (l >= 0 ? s1[l--] - '0' : 0);
		int b = (r >= 0 ? s2[r--] - '0' : 0); // 拆出两个加数，如果其中一个串被加完了则补0
		carry += a + b; // 计算a + b + 进位数
		ans += carry % 10 + '0'; // 获取个位数
		carry /= 10; // 获取进位数
	}
	if (carry) ans += "1"; // 如果加完还有进位，则最高位补1
	reverse(ans.begin(), ans.end());
	return ans;
}
string sub(string s1, string s2, int minus) {
	string ans;
	int l = s1.size() - 1, r = s2.size() - 1, carry = 0;
	if (l < r || l == r && s1 < s2) return sub(s2, s1, 1); // 如果被减数大于减数，则交换被减数和减数并输出负号
	if (minus) cout << '-';
	while (l >= 0 || r >= 0) {
		int a = l < 0 ? 0 : s1[l--] - '0';
		int b = r < 0 ? 0 : s2[r--] - '0';
		carry = a + 10 - b - carry; // 计算a + 10 - b - 借位数
		// 如果结果大于10则说明不用借位，如果小于10则说明要借位
		ans += carry % 10 + '0'; // 获取答案的个位数
		carry = carry / 10 ^ 1; // carry / 10 = 0则要借位借位为1，如果carry / 10 = 1则不要借位借位为0
	}
	while (ans.size() > 1 && ans.back() == '0') ans.pop_back(); // 去除前导0并且防止结果就是0仍需要保留1个0
	reverse(ans.begin(), ans.end());
	return ans;
}
string mul(string a, string b) {
	if (a == "0" || b == "0") return "0"; // 处理乘数是0的时候
	int l1 = a.size(), l2 = b.size(), x = 0;
	string ans(l1 + l2 - 1, '0');
	for (int i = l1 - 1; i >= 0; i--) { // 对乘数1从后往前遍历
		x = 0; // 乘数1切换数字时，重置进位数
		for (int j = l2 - 1; j >= 0; j--) { // 对乘数2从后往前遍历
			int c1 = a[i] - '0', c2 = b[j] - '0';
			x += c1 * c2 + ans[i + j] - '0';
			ans[i + j] = x % 10 + '0';
			x /= 10;
		}
		if (i) ans[i - 1] += x; // 处理乘完一个乘数1后的爆位操作
	}
	if (x) ans = to_string(x) + ans; // 处理最高位爆位
	return ans;
}
vector<string> chu(string s1, string s2) {
	int l1 = s1.size(), l2 = s2.size();
	if (l1 < l2 || l1 == l2 && s1 < s2) return {"0", s1};
	string ans, div = s1.substr(0, l2 - 1);
	for (int i = l2 - 1, j; i < l1; i++) {
		if (div == "0") div = "";
		div += s1[i];
		for (j = 0; j <= 9; j++) {
			int l = div.size(), r = l2 - 1, carry = 0;
			string ret;
			if (l < l2 || l == l2 && div < s2) break;
			l--;
			while (l >= 0 || r >= 0) {
				int a = l < 0 ? 0 : div[l--] - '0';
				int b = r < 0 ? 0 : s2[r--] - '0';
				carry = 10 + a - b - carry;
				ret += carry % 10 + '0';
				carry = carry / 10 ^ 1;
			}
			while (ret.size() > 1 && ret.back() == '0') ret.pop_back();
			reverse(ret.begin(), ret.end());
			div = ret;
		}
		if (!j && ans.empty()) continue;
		ans.push_back(j + '0');
	}
	return {ans, div};
}
int main() {
	cin >> a >> b;
	cout << add(a, b) << endl;
	cout << sub(a, b, 0) << endl;
	cout << mul(a, b) << endl;
	vector<string> ans = chu(a, b);
	cout << ans[0] << endl << ans[1] << endl;
	return 0;
}
