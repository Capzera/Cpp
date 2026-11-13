#include <bits/stdc++.h>
using namespace std;
struct Trie {
	int val;
	map<string, Trie*> child;
	Trie(int v = 0) : val(v) {}
};
string json, s;
int p, i;
Trie* Build_Tree() {
	Trie *node = new Trie();
	i++; // 跳过左括号
	while (json[i] != '}') {
		i++; // 跳过双引号
		int st = i;
		while (json[i] != '"') i++;
		string key = json.substr(st, i - st);
		i += 2;
		if (json[i] == '{') {
			Trie *cd = Build_Tree(); // 递归建树
			node->child[key] = cd;
		} else {
			int num = 0;
			while (isdigit(json[i])) {
				num = num * 10 + json[i++] - '0';
			}
			Trie *cd = new Trie(num);
			node->child[key] = cd;
		}
		if (json[i] == ',') i++;
	}
	i++;
	return node;
}
int main() {
	cin >> json >> p;
	struct Trie *root = Build_Tree();
	while (p--) {
		cin >> s;
		root = root->child[s];
	}
	cout << root->val;
	return 0;
}
