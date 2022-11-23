#include <bits/stdc++.h>
using namespace std;
long long second(string time1, string time2) {
    int h1 = stoi(time1.substr(0, 2)), h2 = stoi(time2.substr(0, 2));
    int m1 = stoi(time1.substr(3, 2)), m2 = stoi(time2.substr(3, 2));
    int s1 = stoi(time1.substr(6, 2)), s2 = stoi(time2.substr(6, 2));
    long long ans = h1 * 3600 + m1 * 60 + s1 - (h2 * 3600 + m2 * 60 + s2);
    return ans;
}
int main(){
    cin.tie(nullptr)->sync_with_stdio(false);
    int n;
    long long ans = 0;
    cin >> n;
    vector<string> time(n);
    vector<int> fute(n), anbe(n);
    for (int i = 0; i < n; i++) cin >> time[i] >> fute[i] >> anbe[i];
    for (int i = 1; i < n; i++) {
        long long minus = second(time[i], time[i - 1]);
        ans += minus * fute[i - 1] * anbe[i - 1];
    }
    cout << ans << endl;
    return 0;
}
