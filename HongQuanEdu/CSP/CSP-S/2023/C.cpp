#include <bits/stdc++.h>
using namespace std;
string s;
int k, addr;
vector<vector<int> > cls;
unordered_map<string, int> type = {
    {"byte" , 1},
    {"short", 2},
    {"int", 4},
    {"long", 8}
};
/*vector<unordered_map<string, int> > */
void create_class() {
    cls.emplace_back(vector<int>(0));
    int pragma = 0;
    int size = 0, st = 0;
    cin >> s >> k;
    vector<int> byte;
    while (k--) {
        string ty, nm;
        cin >> ty >> nm;
        byte.emplace_back(type[ty]);//short int 1 4
        pragma = max(pragma, type[ty]);//最大对齐数统一
    }
    for (int i = 0; i < byte.size(); i++) {
        cls.back().emplace_back(st);
        st += pragma;
    }
    cout << st << " " << pragma << endl;
    type[k] = st;
}

int main() {
    int T, op;
    cin >> T;
    while (T--) {
        cin >> op;
        if (op == 1) {
            create_class();
        }
        if (op == 2) {

        }
        if (op == 3) {

        }
        if (op == 4) {

        }
    }
    system("pause");
    return 0;
}