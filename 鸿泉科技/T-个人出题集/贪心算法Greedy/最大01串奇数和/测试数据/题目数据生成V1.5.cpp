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
// STEP 4 : Modify the cases number
//|---------------------------------------------|
int ll = 4, rr = 4; //Test Cases Number
//|---------------------------------------------|
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
void solve(int number) {
	string aaa = to_string(number) + ".in";
	freopen(aaa.c_str(), "w", stdout);
	//STEP 1 : std 100%AC Code copy here and replace "cin" with generator
	//|---------------------------------------------|
	int l1 = mrand(800, 1000, 0, -1), l2 = mrand(800, 1000, 0, -1);
	string a(l1, '0'), b(l2, '0'), ans;
	for (int i = 0; i < l1; i++) {
		int pppp = rand() % 10 + 1;
		if (pppp <= 5) a[i] = '1';
	}
	for (int i = 0; i < l2; i++) {
		int pppp = rand() % 10 + 1;
		if (pppp <= 5) b[i] = '1';
	}
	/*int ppp;
	ppp = mrand(1, l1, 0, -1);
	a[ppp]='1';
	ppp = mrand(1, l2, 0, -1);
	b[ppp]='1';*/
	print(a, 2);
	print(b, 2);
	sort(a.begin(), a.end(), greater<char>());
	sort(b.begin(), b.end(), greater<char>());
	int l = 0, r = 0;
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
	
    //|---------------------------------------------|
	string bbb = to_string(number) + ".out";
	freopen(bbb.c_str(), "w", stdout);
	
	//STEP 2 : cout ans here :
	//|---------------------------------------------|
	if (ans1.size() > ans2.size()) cout << ans1;
	else if (ans2.size() > ans1.size()) cout << ans2;
	else cout << max(ans1, ans2);		
	//|---------------------------------------------|
}

int main() {
	srand(time(NULL));
	for (int i = ll; i <= rr; i++) {
		solve(i);
	}
	return 0; 
} 
