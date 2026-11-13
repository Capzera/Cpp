#include <bits/stdc++.h>
using namespace std;
int main() {
    string A, B;
    cin >> A >> B;
    int ans = 0, len = A.size() + B.size();
    for (int i = 0; i <= A.size(); i++) {
        string str = A.substr(0, i) + B + A.substr(i);//完整串
        string str2 = str.substr(0, len / 2);//前半串
        str = str.substr(len / 2 + len % 2);//后半串
        reverse(str.begin(), str.end());
        if (str == str2) ans++;
    }
    cout << ans << endl;
    system("pause");
    return 0;
}