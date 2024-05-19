#include <bits/stdc++.h>
using namespace std;
string HAMulti(string s1, string s2) {
    if (s1 == "0" || s2 == "0") return "0";
    int l1 = s1.size(), l2 = s2.size(), x;
    string ans(l1 + l2 - 1, '0');
    for (int i = l1 - 1; i >= 0; i--) {
        x = 0;
        for (int j = l2 - 1; j >= 0; j--) {
        	int a = s1[i] - '0', b = s2[j] - '0';
            x += a * b + ans[i + j] - '0';
            ans[i + j] = x % 10 + '0';
            x /= 10;
        }
        if (i) {
        	ans[i - 1] += x;
		}
    }
    if (x) {
    	ans = to_string(x) + ans;
	}
    return ans;
}
int main() {
    string s1, s2;
    cin >> s1 >> s2;
    cout << HAMulti(s1, s2) << endl;
    system("pause");
    return 0;
}