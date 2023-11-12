/*

均分纸牌    
*/
#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, sum = 0, ans = 0;
    cin >> n;
    vector<int> cards(n);
    for (auto& x : cards) cin >> x;
    sum = accumulate(cards.begin(), cards.end(), 0) / n;
    for (auto& x : cards) ans += x < sum;
    cout << ans << endl;
    system("pause");
    return 0;
}
/*
4 -9 1 -1 3 1 1
*/