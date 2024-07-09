#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
string sub(string s1, string s2) {
    string ans;
    int l = s1.size() - 1, r = s2.size() - 1, x = 0, minus = 0;
    if (l < r || (l == r && s1 < s2)) {
    	swap(l, r);
    	swap(s1, s2);
    	minus = 1;
	}
    while (l >= 0 || r >= 0) {
        int a = l < 0 ? 0 : s1[l--] - '0';
        int b = r < 0 ? 0 : s2[r--] - '0';
        x = a + 10 - b - x;
        ans += x % 10 + '0';
        x = x / 10 == 1 ? 0 : 1;
    }
    while (ans.size() > 1 && ans.back() == '0') ans.pop_back();
    if (minus) ans += "-";
    reverse(ans.begin(), ans.end());
    return ans;
}
string div(string s1, string s2) {
	int l1 = s1.size(), l2 = s2.size();
	if (l1 < l2 || (l1 == l2 && s1 < s2)) {
		return "0";
	}
	string ans, num;
	for (int i = 0; i < l1; i++) {
		if (num == "0") {
			num = "";
		}
		num += s1[i];
		int j;
		for (j = 0; j < 10; j++) {
			string ret = sub(num, s2);
			if (ret[0] == '-') {
				break;
			}
			num = ret;
		}
		if (ans == "" && j == 0) {
			continue;
		}
		ans += j + '0';
	}
	return ans;
} 
int main() {
	string s1, s2;
	cin >> s1 >> s2;
	cout << div(s1, s2);
	return 0;
}

