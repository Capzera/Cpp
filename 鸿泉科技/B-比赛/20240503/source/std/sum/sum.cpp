#include <bits/stdc++.h>
using namespace std;
string add(string s1, string s2) {  
	string ans;
	int i = s1.size() - 1, j = s2.size() - 1, x = 0;
	while(i >= 0 || j >= 0) {
		int a = i >= 0 ? s1[i--] - '0' : 0;
		int b = j >= 0 ? s2[j--] - '0' : 0;
		x += a + b;
		ans += x % 2 + '0'; 
		x /= 2;
	}
	if (x) ans += "1";
	reverse(ans.begin(),ans.end());
	return ans;
}
int main() {
	freopen("sum.in", "r", stdin);
	freopen("sum.out", "w", stdout);
	string a, b, ans;
	cin >> a >> b;
	sort(a.begin(), a.end(), greater<char>());
	sort(b.begin(), b.end(), greater<char>());
	int l1 = a.size(), l2 = b.size(), l = 0, r = 0;
	while (l < l1 && a[l] == '1') l++;
	while (r < l2 && b[r] == '1') r++;
	string ans1 = "", ans2 = "";
	if (l && l != l1) {
		string t = a;
		swap(t[l - 1], t[l1 - 1]);
		ans1 = add(t, b);
	} else if (l) ans1 = add(a, b);
	if (r && r != l2) {
		string t = b;
		swap(t[r - 1], t[l2 - 1]);
		ans2 = add(a, t);
	} else if (r) ans2 = add(a, b);
	if (ans1.size() > ans2.size()) cout << ans1;
	else if (ans2.size() > ans1.size()) cout << ans2;
	else cout << max(ans1, ans2);
	return 0;
}

