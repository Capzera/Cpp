#include <bits/stdc++.h>
using namespace std;
int main() {
    string str, tmp = "";
    vector<string> word;
    getline(cin, str);
    for (auto& c : str) {
        if (!isspace(c)) tmp += c;
        else word.push_back(tmp);
    }
    for (int i = word.size() - 1; i >= 0; i--) cout << word[i] << " ";
    system("pause");
    return 0;
}