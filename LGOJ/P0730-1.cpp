#include <bits/stdc++.h>
using namespace std;
string a, b;
string div(string a, string b, int op = 0) {
	int l1 = a.size(), l2 = b.size(), x;
	if (l1 < l2 || l1 == l2 && a < b) return "0";
	string ans, num = a.substr(0, l2 - 1);
	for (int i = l2 - 1; i < l1; i++) { // 被除数逐个增加一个数进库
		if (num == "0") num = "";
		num += a[i];
		for (x = 0; x <= 9; x++) { // 枚举商
			int l = num.size() - 1, r = l2 - 1, jw = 0;
			if (l < r || l == r && num < b) break; // 被减数无法减去除数，此时该商
			while (r >= 0) {
				int n1 = l >= 0 ? num[l] - '0' : 0;
				int n2 = r >= 0 ? b[r--] - '0' : 0;
				jw = n1 - n2 + 10 - jw;
				num[l--] = jw % 10 + '0';
				jw = jw / 10 ^ 1;
			}
			if (jw && num[0] != '0') num[0]--;
			while (num[0] == '0') num = num.substr(1);
		}
		if (x || ans != "") ans += x + '0'; // 防止出现前导0
	}
	if (!op) return ans;
	if (num == "") num = "0";
	return num;
}
int main() {
	cin >> a >> b;
	cout << div(a, b);
	return 0;
}
