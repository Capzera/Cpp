#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, k, s;
	string s1, s2;
	cin >> n >> k >> s;
	vector<string> vec;
	vector<int> result;
	unordered_map<string, int> mp;
	for (int i = 0; i < k; i++) {
		string str;
		cin >> str;
		vec.push_back(str);
		mp[str] = i + 1; // 编号从1开始
	}
	cin >> s1;
	for (char c : s1) {
		string temp = s2+ c;
		if (mp.find(temp) != mp.end()) {
			s2= temp;
		} else {
			result.push_back(mp[s2]);
			vec.push_back(temp);
			mp[temp] = vec.size();
			s2= string(1, c);
		}
	}
	if (s2.size()) result.push_back(mp[s2]);
	for (int i = 0; i < result.size(); i++) {
		if (i > 0) cout << " ";
		cout << result[i];
	}
	cout << endl << vec.size() << endl;
	for (const string& str : vec) {
		cout << str << endl;
	}
	return 0;
}
