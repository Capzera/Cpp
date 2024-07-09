#include <bits/stdc++.h>
using namespace std;
void print(char x) {
	cout << x;
}
void solve(int number) {
	string aaa = to_string(number) + ".in";
	freopen(aaa.c_str(), "w", stdout);
	//STEP 1 : std 100%AC Code copy here and replace "cin" with generator
	//┌------------Template Dividing Line------------┐ 
	string s;
    int ans = 0;
    int n = rand() % 100000;
    int x = rand() % 26;
	
    for (int i = 0; i < n; i++) {
    	int y = rand() % 2;
    	char ch;
    	if (y) {
    		ch = 'A' + x;
		} else {
			ch = 'a' + x;
		}
		s += ch;
		print(ch);
	}
    for (auto& x : s) x = toupper(x);
    for (int i = 1; i < s.size(); i++) {
        ans += s[i] != s[i - 1];
    }	
    //└------------Template Dividing Line------------┘ 
	string bbb = to_string(number) + ".out";
	freopen(bbb.c_str(), "w", stdout);
	//STEP 2 : cout ans here :
	//┌------------Template Dividing Line------------┐ 
	cout << ans << endl;			
	//└------------Template Dividing Line------------┘ 
}
	// STEP 4 : Modify the cases number
	//┌------------Template Dividing Line------------┐ 
	int ll = 1, rr = 3; //Test Cases Number
	//└------------Template Dividing Line------------┘ 
int main() {
	srand(time(NULL));
	for (int i = ll; i <= rr; i++) {
		solve(i);
	}
	return 0; 
} 
