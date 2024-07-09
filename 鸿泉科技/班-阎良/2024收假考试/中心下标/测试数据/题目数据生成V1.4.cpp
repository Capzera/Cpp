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
	long long a = INT_MIN, b = INT_MIN, c = INT_MIN, d = INT_MIN, e = INT_MIN, ans = LLONG_MIN;
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
			else if (y <= 1e14) {
				a = rand() % 1000;
				b = rand() % 1000;
				c = rand() % 1000;
				d = rand() % 1000;
				e = rand() % 100;
				ans = a * 1e11 + b * 1e8 + c * 1e5 + d * 100 + e;
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
int ll = 8, rr = 9; //Test Cases Number
//|---------------------------------------------|
void solve(int number) {
	string aaa = to_string(number) + ".in";
	freopen(aaa.c_str(), "w", stdout);
	//STEP 1 : std 100%AC Code copy here and replace "cin" with generator
	//|---------------------------------------------|
	int n;
	n = mrand(8000, 10000, 0, 2);
	long long sum = mrand(1, 1e14, -1, -1), sum2 = sum;
	int ans = mrand(0, n - 1, 0, -1);
	vector<long long> nums(n);
	nums[ans] = mrand(1, 1e9, -1, -1);
	for (int i = 0; i < ans; i++) {
		long long x = mrand(1, 1e9, -1, -1);
		sum -= x;
		nums[i] += x;
	}
	long long fen = 1ll * sum / ans;
	for (int i = 0; i < ans - 1; i++) {
		nums[i] += fen;
		sum -= fen;
	}
	nums[ans - 1] += sum;
	for (int i = ans + 1; i < n; i++) {
		long long x = mrand(1, 1e9, -1, -1);
		sum2 -= x;
		nums[i] += x;
	}
	fen = 1ll * sum2 / (n - ans - 1);
	for (int i = ans + 1; i < n - 1; i++) {
		nums[i] += fen;
		sum2 -= fen;
	}
	nums[n - 1] += sum2;
	for (int i = 0; i < n; i++) cout << nums[i] << " ";
    //|---------------------------------------------|
	string bbb = to_string(number) + ".out";
	freopen(bbb.c_str(), "w", stdout);
	
	//STEP 2 : cout ans here :
	//|---------------------------------------------|
	cout << ans << endl;		
	//|---------------------------------------------|
}

int main() {
	srand(time(NULL));
	for (int i = ll; i <= rr; i++) {
		solve(i);
	}
	return 0; 
} 
