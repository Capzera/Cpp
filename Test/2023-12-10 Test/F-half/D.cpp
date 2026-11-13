#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, sum = 0, ans = 0;
    cin >> n;
    vector<int> books(n), vec;
    for (auto& x : books) cin >> x;
    sort(books.begin(), books.end());
    vec.emplace_back(books[0]);
    for (int i = 1; i < n; i++) {
        if (books[i] == books[i - 1]) sum++;
        else vec.emplace_back(books[i]);
    }
    int l = 0, r = vec.size() - 1, target = 1;
    while (l <= r) {
        if (vec[l] == target) l++;
        else {
            if (sum > 1) sum -= 2;
            else if (sum == 1)  {
                r--;
                sum = 0;
            }
            else if (r - l) r -= 2;
            else break;
        }
        ans++, target++;
    }
    cout << ans + sum / 2 << endl;
    system("pause");
    return 0;
}