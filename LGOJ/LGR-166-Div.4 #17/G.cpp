#include <bits/stdc++.h>
using namespace std;
unordered_map<char, char> mp = {
    {'A', 'T'},
    {'T', 'A'},
    {'C', 'G'},
    {'G', 'C'}
};
int main() {
    int T, n;
    string dna;
    cin >> T;
    auto solve = [&]() -> long long {
        long long ans = 0;
        cin >> n >> dna;
        if (dna.find('U') != string::npos) return 0;
        for (int i = 1; i <= n / 2; i++) {
            ans += (n + 1) * (mp[dna[i - 1]] == dna[n - i]);
        }
        return ans;
    };
    while (T--) { cout << solve() << endl; }
    system("pause");
    return 0;
}