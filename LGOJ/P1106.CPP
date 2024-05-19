#include<bits/stdc++.h>
using namespace std;
int main(){
    int k;
    string s;
    cin >> s >> k;
    s += '0';
    while (k--) {
        for(int i = 0; i < s.size() - 1; i++) {
            if(s[i] <= s[i + 1]) continue;
            s.erase(i, 1);     
            break;
        }
    }
    s.pop_back();
    int i = 0;
    while (s[i] == '0' && i < s.size() - 1) {
        i++;
    }
    cout << s.substr(i) << endl;
    system("pause");
	return 0;
}