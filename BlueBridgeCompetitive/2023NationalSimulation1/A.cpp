#include <bits/stdc++.h>
using namespace std;
long long solve() {
    string str;
    getline(cin, str);
    long long ans = 0, base = str[0] == 'i' ? 4 : 8, cnt = 0, flag = 0;
    if (str.size() == 0) return ans;
    if (str[0] == 'S') {
        for (auto& ch : str) {
            if (ch == '"') {
                if (flag) {
                    ans += cnt;
                    cnt = 0;
                }
                flag ^= 1;
                continue;
            }
            if (!flag) continue;
            cnt++;
        }
        return ans;
    }
    if (str.find('[') == string::npos) {
        for (auto& ch : str)
            if (ch == '=') ans++;
        return base * ans;
    }
    for (auto& ch : str) {
        if (ch == '[') {
            flag = 1;
            continue;
        }
        if (ch == ']') {
            ans += cnt;
            cnt = flag = 0;
        }
        if (!flag) continue;
        cnt = cnt * 10 + ch - '0';
    }
    return ans * base;
}

int main() {
    int t = 1;
    long long ans = 0;
    cin >> t;
    t++;
    while (t--){
        ans += solve();
    }
    long long G = 1073741824, M = 1048576, K = 1024;
    if (ans >= G) cout << ans / G << "G";
    ans %= G;
    if (ans >= M) cout << ans / M << "MB";
    ans %= M;
    if (ans >= K) cout << ans / K << "KB";
    ans %= K;
    if (ans > 0) cout << ans << "B";
    cout << endl;
    system("pause");
    return 0;
}
/*
1
long[] nums = new long [131072];
4
int a=0, b=0;
long x=0,y=0;
String s1=”hello”,s2=”world”;
long[] arr1=new long[100000],arr2=new long[100000];
200000*8 + 10 + 16 + 8
1600034
1MB538KB546B

int 和 long 型 找等号，多少个等号乘base即答案
int[] long[] 找'['符号，符号内数字求和乘base
String型以"作为基准，找到"内求和
*/