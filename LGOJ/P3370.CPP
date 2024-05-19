#include <bits/stdc++.h>
using namespace std;
int main() {
    int n;
    string str;
    unordered_set<string> s;
    cin >> n;
    while (n--) {
        cin >> str;
        s.emplace(str);
    }   
    cout << s.size() << endl;
    system("pause");
    return 0;
}