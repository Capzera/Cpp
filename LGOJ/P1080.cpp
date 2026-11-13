#include <bits/stdc++.h>
using namespace std;

string HAMulti(string s1, string s2) {
    if (s1 == "0" || s2 == "0") return "0";
    int l1 = s1.size(), l2 = s2.size(), carry;
    string ans(l1 + l2 - 1, '0');
    for (int i = l1 - 1; i >= 0; i--) {
        carry = 0;
        for (int j = l2 - 1; j >= 0; j--) {
            carry += ans[i + j] - '0' + (s1[i] - '0') * (s2[j] - '0');
            ans[i + j] = carry % 10 + '0';
            carry /= 10;
        }
        if (carry && i) ans[i - 1] = carry + '0';
    }
    if (carry) ans = to_string(carry) + ans;
    return ans;
}

string HADivide(string s1, string s2) {
    int l1 = s1.size(), l2 = s2.size();
    if (l1 < l2 || l1 == l2 && s1 < s2) return "0";
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
    return ans;
}

int main() {
    int n, l, r;
    cin >> n >> l >> r;
    vector<vector<int> > hands(n, vector<int>(2));
    for (auto& hand : hands) cin >> hand[0] >> hand[1];
    sort(hands.begin(), hands.end(), [](auto& a, auto& b) { return a[0] * a[1] < b[0] * b[1]; });
    string left = to_string(l), ans;
    for (int i = 0; i < n; i++) {
        string dis = HADivide(left, to_string(hands[i][1]));
        if (dis.size() > ans.size() || dis.size() == ans.size() && dis > ans) ans = dis;
        left = HAMulti(left, to_string(hands[i][0]));
    }
    cout << ans << endl;
    system("pause");
    return 0;
}
/*
#include<bits/stdc++.h>
using namespace std;
int main() {
	int n, k;
    long long sum = 1, ans = LLONG_MAX;
	cin >> n >> sum >> k;
    vector<vector<int> > hands(n, vector<int>(2));
	for(auto& hand : hands) {
		cin >> hand[0] >> hand[1];
		sum *= hand[0];
	}
	for(int i = 0; i < n; i++) {
		ans = min(ans, sum / (hands[i][0] * hands[i][1]));
	}
	cout << ans << endl;
	return 0;
}
*/