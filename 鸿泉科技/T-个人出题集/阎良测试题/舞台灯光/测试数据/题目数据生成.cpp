#include <bits/stdc++.h>
using namespace std;
void print(long long x) {
	cout << x;
}
long long mrand(long long x, long long y) {
	long long a = INT_MIN, b = INT_MIN, c = INT_MIN, ans = LLONG_MIN;
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
			ans = a * 1000 + b;
		}
		else if (y <= 1e9) {
			a = rand() % 1000;
			b = rand() % 1000;
			c = rand() % 1000;
			ans = a * 1e6 + b * 1e3 + c;
		}
	}
	print(ans);
	return ans;
}
string add(string s1, string s2) {
    string ans = "";
    int l = s1.size() - 1, r = s2.size() - 1, carry = 0;
    while (l >= 0 || r >= 0) {
        int a = l >= 0 ? s1[l--] - '0' : 0;
        int b = r >= 0 ? s2[r--] - '0' : 0;
        carry += a + b;
        ans += carry % 10 + '0';
        carry /= 10;
    }
    if (carry) ans += "1";
    reverse(ans.begin(), ans.end());
    return ans;
}
string ssolve(int n) {
    if (n == 1) return "2";
    if (n == 2) return "3";
    string f, f1 = "3", f2 = "2";
    for (int i = 3; i <= n; i++) {
        f = add(f1, f2);
        f2 = f1;
        f1 = f;
    }
    return f;
}
void solve(int number) {
	string aaa = to_string(number) + ".in";
	freopen(aaa.c_str(), "w", stdout);
	//STEP 1 : std 100%AC Code copy here and replace "cin" with generator
	//┌------------Template Dividing Line------------┐ 
	long long n = mrand(46, 93);
    //└------------Template Dividing Line------------┘ 
	string bbb = to_string(number) + ".out";
	freopen(bbb.c_str(), "w", stdout);
	//STEP 2 : cout ans here :
	//┌------------Template Dividing Line------------┐ 
	cout << ssolve(n) << endl;			
	//└------------Template Dividing Line------------┘ 
}
	// STEP 4 : Modify the cases number
	//┌------------Template Dividing Line------------┐ 
	int ll = 13, rr = 19; //Test Cases Number
	//└------------Template Dividing Line------------┘ 
int main() {
	srand(time(NULL));
	for (int i = ll; i <= rr; i++) {
		solve(i);
	}
	return 0; 
} 
