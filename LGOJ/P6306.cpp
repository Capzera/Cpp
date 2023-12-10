#include <bits/stdc++.h>
using namespace std;
long long qread(){
    long long w = 1, c, ret;
    while((c = getchar()) > '9' || c < '0') w = (c== '-' ? -1 : 1); 
    ret = c - '0';
    while((c = getchar()) >= '0'&& c <= '9') ret = ret * 10 + c - '0';
    return ret * w;
}
int main() {
    unordered_map<long long, int> mp;
    long long n, k;
    n = qread(), k = qread();
    while (n--) {
        mp[qread()]++;
    }
    for (auto& [x, time] : mp) {
        if (time % k == 1) {
            cout << x << endl;
            break;
        }
    }
    system("pause");
    return 0;
}