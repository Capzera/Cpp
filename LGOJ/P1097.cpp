#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, x;
    map<int, int> mp;
    cin >> n;
    while (n--) {
        cin >> x;
        mp[x]++;
    }
    for(auto& [a, b] : mp) {
        cout << a << " " << b << endl;
    }
    return 0;
}