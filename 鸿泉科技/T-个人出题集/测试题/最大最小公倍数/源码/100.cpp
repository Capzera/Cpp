#include<bits/stdc++.h>
using namespace std;
int main() {
    long long n;
    cin >> n;
    if(n % 2 == 1) cout << n * (n - 1) * (n - 2);
    else if(n % 3 != 0) cout << n * (n - 1) * (n - 3);
    else cout << (n - 1) * (n - 2) * (n - 3);
    return 0;
}
