#include <bits/stdc++.h>
using namespace std;
int main(){
    cin.tie(nullptr)->sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    if (m < n) m += 7;
    cout << m - n << endl;
    return 0;
}
