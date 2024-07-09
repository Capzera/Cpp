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
				a = rand() % 200 + 800;
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
int ll = 3, rr = 3; //Test Cases Number
//|---------------------------------------------|
void solve(int number) {
	string aaa = "energy" + to_string(number) + ".in";
	freopen(aaa.c_str(), "w", stdout);
	//STEP 1 : std 100%AC Code copy here and replace "cin" with generator
	//|---------------------------------------------|
	int n, m;
	n = mrand(1, 1e3);
	m = mrand(1, 1e3, 0, 2);
	vector<int> a(n), b(m);
	for (auto& x : a) {
		int ppp = rand() % 3 + 1;
		x = mrand(1, pow(10, ppp));
	}
	cout << endl;
	for (auto& x : b) {
		int ppp = rand() % 3 + 1;
		x = mrand(1, pow(10, ppp));
	}
	sort(a.begin(), a.end(), greater<int>());
	sort(b.begin(), b.end(), greater<int>());
	long long ans = 0;
	for (int i = 0, j = 0; i < n && j < m; i++, j++) {
		while (j < m && a[i] < b[j]) j++;
		if (j == m) break;
		ans++;
	}
	
	//cout << endl << suma << " " << sumb;
    //|---------------------------------------------|
	string bbb = "energy" + to_string(number) + ".out";
	freopen(bbb.c_str(), "w", stdout);
	
	//STEP 2 : cout ans here :
	//|---------------------------------------------|
	cout << ans;		
	//|---------------------------------------------|
}

int main() {
	srand(time(NULL));
	for (int i = ll; i <= rr; i++) {
		solve(i);
	}
	return 0; 
} 
