#include <bits/stdc++.h>
using namespace std;
int main() {
    int game[5][5] = {{0, 0, 1, 1, 0}, {1, 0, 0, 1, 0}, {0, 1, 0, 0, 1}, {0, 0, 1, 0, 1}, {1, 1, 0, 0, 0}};
    int n, na, nb, ansa = 0, ansb = 0;
    cin >> n >> na >> nb;
    vector<int> a(na), b(nb);
    for (auto& x : a) cin >> x;
    for (auto& x : b) cin >> x;
    for (int i = 0; i < n; i++) {
        ansa += game[a[i % na]][b[i % nb]];
        ansb += game[b[i % nb]][a[i % na]];
    }
    cout << ansa << " " << ansb << endl;
    system("pause");
    return 0;
}