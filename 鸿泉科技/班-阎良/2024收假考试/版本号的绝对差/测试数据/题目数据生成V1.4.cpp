/*************************************
This code is for teacher who made a new
problem and need to create test cases.
This is a manual of the code.

@Create : 2024-01-31 17:59
@Author : Capzera
@E-mail : 1786126188@qq.com

void print(long long x, int p) // This function is used to output the input data to a file
	p = 0 cout space after print
	p = 1 cout null after print
	p = 2 cout enter after print

long long mrand(ll x, ll y, int g, int p) //This function is a custom random function
	g = 0 generate the ramdom number
	g = 1 generate the random character
	g = 2 generate the char number
**************************************/
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
	if (!g) { //d = 0 only Long long int numbers
		while (ans < x || ans > y) {	
			if (y <= 1e4) {
				ans = rand() % (y - x + 1) + x;
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
			else if (y <= eps1) {
				a = rand() % 1000;
				b = rand() % 1000;
				c = rand() % 1000;
				d = rand() % 1000;
				ans = a * 1e9 + b * 1e6 + c * 1e3 + d;
			}
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
    print(ansc, p);
	return ansc;
}
char temp(int p) {
	int ran = rand() % 9 + 1;
	char x = '0' + ran;
	print(x, p);
	return x;
}
string add(string s1, string s2) {
    string ans;
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
string sub(string s1, string s2) {
    string ans;
    int l = s1.size() - 1, r = s2.size() - 1, carry = 0;
    if (l < r || l == r && s1 < s2) {
    	swap(s1, s2);
    	swap(l, r);
	}	
    while (l >= 0 || r >= 0) {
        int a = l < 0 ? 0 : s1[l--] - '0';
        int b = r < 0 ? 0 : s2[r--] - '0';
        carry = a + 10 - b - carry;
        ans += carry % 10 + '0';
        carry = carry / 10 ^ 1;
    }
    while (ans.size() > 1 && ans.back() == '0') ans.pop_back();
    reverse(ans.begin(), ans.end());
    return ans;
}
string mul(string s1, string s2) {
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
        if (i) ans[i - 1] += x;
    }
    if (x) ans = to_string(x) + ans;
    return ans;
}
// STEP 4 : Modify the cases number
//|---------------------------------------------|
int ll = 4, rr = 4; //Test Cases Number
//|---------------------------------------------|
void solve(int number) {
	string aaa = to_string(number) + ".in";
	freopen(aaa.c_str(), "w", stdout);
	//STEP 1 : std 100%AC Code copy here and replace "cin" with generator
	//|---------------------------------------------|
	string a1 = "V", b1 = "V", a2, b2, k;
	cout << "V";
	a1 += temp(1);
	int n1 = mrand(50, 100, 0, -1);
	for (int i = 1; i < n1; i++) {
		a1 += mrand(0, 0, 2, 1);
	}
	cout << ".";
	a1 += ".";
	a1 += temp(1);
	int n2 = mrand(1, 3, 0, -1);
	for (int i = 1; i < n2; i++) {
		a1 += mrand(0, 0, 2, 1);
	}
	cout << endl << "V";
	b1 += temp(1);
	int m1 = mrand(50, 100, 0, -1);
	for (int i = 1; i < m1; i++) {
		b1 += mrand(0, 0, 2, 1);
	}
	cout << ".";
	b1 += ".";
	b1 += temp(1);
	int m2 = mrand(1, 3, 0, -1);
	for (int i = 1; i < m2; i++) {
		b1 += mrand(0, 0, 2, 1);
	}
	cout << endl;
	int xxx = mrand(max(m2, n2) + 1, 4, 0, -1);
	k += temp(1);
	for (int i = 1; i < xxx; i++) {
		k += mrand(0, 0, 2, 1);
	}
	k = add(k, "1");
	int i = 1, j = 1;
	while (i < a1.size() && a1[i] != '.') {
		i++;
	} 
	while (j < b1.size() && b1[j] != '.') {
		j++;
		
	}
	a2 = a1.substr(i + 1);
	a1 = a1.substr(1, i - 1);
	b2 = b1.substr(j + 1);
	b1 = b1.substr(1, j - 1);
	string x1 = add(mul(a1, k), a2);
	string x2 = add(mul(b1, k), b2);
    //|---------------------------------------------|
	string bbb = to_string(number) + ".out";
	freopen(bbb.c_str(), "w", stdout);
	//STEP 2 : cout ans here :
	//|---------------------------------------------|
	cout << sub(x1, x2);		
	//|---------------------------------------------|
}

int main() {
	srand(time(NULL));
	for (int i = ll; i <= rr; i++) {
		solve(i);
	}
	return 0; 
} 
