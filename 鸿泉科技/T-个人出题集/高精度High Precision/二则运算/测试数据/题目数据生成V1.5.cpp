#include <bits/stdc++.h>
using namespace std;

template<typename T>
void print(T x, int p = 0) {
	cout << x;
	if (!p) cout << " ";
	if (p == 2) cout << endl;
}

long long mrand(long long x, long long y, int g = 0, int p = 0) {
	long long a = INT_MIN, b = INT_MIN, c = INT_MIN, d = INT_MIN, ans = LLONG_MIN;
	long long eps1 = 1000000000000;
	int ran;
	char ansc = ' ';
	if (g < 1) { //d = 0 only Long long int numbers
		while (ans < x || ans > y) {
			if (y <= 1e4) {
				ans = rand() % y;
			}
			else if (y <= 1e5) {
				a = rand() % 100;
				b = rand() % 1000;
				ans = a * 1000 + b;
			}
			else if (y <= 1e6) {
				a = rand() % 1000;
				b = rand() % 1000;
				ans = a * 1e3 + b;
			}
			else if (y <= 1e7) {
				a = rand() % 1000;
				b = rand() % 1000;
				c = rand() % 10;
				ans = a * 1e4 + b * 10 + c;
			}
			else if (y <= 1e8) {
				a = rand() % 1000;
				b = rand() % 1000;
				c = rand() % 100;
				ans = a * 1e5 + b * 1e2 + c;
			}
			else if (y <= 1e9) {
				a = rand() % 1000;
				b = rand() % 1000;
				c = rand() % 1000;
				ans = a * 1e6 + b * 1e3 + c;
			}
			else if (y <= 1ll * 10000000000) {
				a = rand() % 1000;
				b = rand() % 1000;
				c = rand() % 1000;
				d = rand() % 10;
				ans = a * 1e7 + b * 1e4 + c * 10 + d;
			}
			else if (y <= 1ll * 100000000000) {
				a = rand() % 1000;
				b = rand() % 1000;
				c = rand() % 1000;
				d = rand() % 100;
				ans = a * 1e8 + b * 1e5 + c * 100 + d;
			}
			else if (y <= eps1) {
				a = rand() % 1000;
				b = rand() % 1000;
				c = rand() % 1000;
				d = rand() % 1000;
				ans = a * 1e9 + b * 1e6 + c * 1e3 + d;
			}
		}
		if (g == -1) {
			ran = rand() % 2;
			if (ran) ans = -ans;
		}
		if (p != -1) print(ans, p);
		return ans;
	}
	if (g == 1) { // all Letters and Numbers
        ran = rand() % 128;
		while (!isalpha(ansc) || isdigit(ansc)) {
			ran = rand() % 128;
			ansc = ran;
		}
	}
	if (g == 2) {
		ran = rand() % 10;
		ansc = '0' + ran;
	}
	if (g > 2 && g < 6) {
		ran = rand() % 26;
		int cas = 0;
		if (g == 5) cas = 1;
		if (g == 3) cas = rand() % 2;
		if (!cas) ansc = 'A' + ran;
		else ansc = 'a' + ran;
	}
    if (p != -1) print(ansc, p);
	return ansc;
}
string mrand(int n, int g = 0, int p = 0) {
	int x = n * 0.2, y = n * 0.8;
	n = rand() % x + y;
	int ran;
	string ans;
	if (g == -1) ans += "-";
	ran = rand() % 9 + 1;
	ans += to_string(ran);
	for (int i = 1; i < n; i++) {
		ran = rand() % 10;
		ans += to_string(ran);
	}
	if (p != -1) print(ans, p);
	return ans;
}
string sub(string s1, string s2, int minus) {
    string ans;
    int l = s1.size() - 1, r = s2.size() - 1, carry = 0;
    if (l < r || (l == r && s1 < s2)) return sub(s2, s1, 1);
    while (l >= 0 || r >= 0) {
        int a = l < 0 ? 0 : s1[l--] - '0';
        int b = r < 0 ? 0 : s2[r--] - '0';
        carry = a + 10 - b - carry;
        ans += carry % 10 + '0';
        carry = carry / 10 ^ 1;
    }
    while (ans.size() > 1 && ans.back() == '0') ans.pop_back();
    if (minus) ans += '-';
    reverse(ans.begin(), ans.end());
    return ans;
}
string add(string s1, string s2) {
    string ans;
    bool f1 = 0, f2 = 0, f3 = 0;
    if (s1[0] == '-') {
    	f1 = 1;
    	s1 = s1.substr(1);
	} 
    if (s2[0] == '-') {
    	f2 = 1;
    	s2 = s2.substr(1);
	}
    if (f1 && !f2) return sub(s2, s1, 0);
	if (f2 && !f1) return sub(s1, s2, 0);
	f3 = f1 & f2;
    int l = s1.size() - 1, r = s2.size() - 1, carry = 0;
    while (l >= 0 || r >= 0) {
        int a = l >= 0 ? s1[l--] - '0' : 0;
		int b = r >= 0 ? s2[r--] - '0' : 0;
        carry += a + b;
        ans += carry % 10 + '0';
        carry /= 10;
    }
    if (carry) ans += "1";
    if (f3) ans += "-";
    reverse(ans.begin(), ans.end());
    return ans;
}
string mul(string s1, string s2) {
    if (s1 == "0" || s2 == "0") return "0";
    bool f1 = 0, f2 = 0, f3 = 0;
	if (s1[0] == '-') {
		f1 = 1;
		s1 = s1.substr(1);
	}
    if (s2[0] == '-') {
    	f2 = 1;
    	s2 = s2.substr(1);
	}
    f3 = f1 ^ f2;
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
        if (i) ans[i - 1] += x;
    }
    if (x) ans = to_string(x) + ans;
    if (f3) ans = "-" + ans;
    return ans;
}
// STEP 4 : Modify the cases number
//|---------------------------------------------|
int ll = 5, rr = 5; //Test Cases Number
//|---------------------------------------------|
void solve(int number) {
	string aaa = to_string(number) + ".in";
	freopen(aaa.c_str(), "w", stdout);
	//STEP 1 : std 100%AC Code copy here and replace "cin" with generator
	//|---------------------------------------------|
	string s1, s2, s3;
	s1 = mrand(1000);
	s2 = mrand(1000, -1);
	s3 = mrand(1000);

    //|---------------------------------------------|
	string bbb = to_string(number) + ".out";
	freopen(bbb.c_str(), "w", stdout);
	//STEP 2 : cout ans here :
	//|---------------------------------------------|
	cout << mul(add(s1, s2), s3);		
	//|---------------------------------------------|
}

int main() {
	srand(time(NULL));
	for (int i = ll; i <= rr; i++) {
		solve(i);
	}
	return 0; 
} 
