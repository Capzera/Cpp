#include<bits/stdc++.h>
using namespace std;
int main() {
    long long n;
    cin >> n;
    long long l = 0, r = 100005;
    while (l < r) {
        long long mid = l + (r - l) / 2;
        if (mid * mid * mid <= n) l = mid + 1;
        else r = mid;
    }
    cout << l - 1<< endl;
    return 0;
}