#include<bits/stdc++.h>
using namespace std;
int main(){
    int k, m;
    priority_queue<int, vector<int>, greater<int> > pq;
    string s;
    cin >> k >> m;
    pq.push(1);
    while (k--) {
        int x = pq.top(); pq.pop();
        s += to_string(x);
        pq.push(2 * x + 1);
        pq.push(4 * x + 5);
    }
    cout << s << endl;
    while (m--) {
        for(int i = 0; i < s.size() - 1; i++) {
            if(s[i] >= s[i + 1]) continue;
            s.erase(i, 1);     
            break;
        }
    }
    cout << s << endl;
	return 0;
}
