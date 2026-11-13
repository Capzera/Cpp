/*
问题描述：P0131
给你一个字符串 s，请你将 s 分割成一些子串，使每个子串都是 回文串 。返回 s 所有可能的分割方案。
回文串 是正着读和反着读都一样的字符串。
示例 1：
输入：s = "aab"
输出：[["a","a","b"],["aa","b"]]
示例 2：
输入：s = "a"
输出：[["a"]]
提示：
1 <= s.length <= 16
s 仅由小写英文字母组成
*/
class Solution {
public:
    vector<vector<string>> ans;
    vector<string> tmp;
    string s;
    int n;
    static bool check(string s){
		int n=s.length();
        if (n==0) return true;
        string tmp=s.substr(n/2+n%2,n);
    	reverse(tmp.begin(),tmp.end());
        return s.substr(0,n/2)==tmp;
    
	}
    void dfs(int i,vector<string> tmp){
		if (i==n){
			ans.push_back(tmp);
            return;
		}
        for (int j=1;j<=n-i;j++){
			if(check(s.substr(i,j))==0) continue;
            tmp.push_back(s.substr(i,j));
            dfs(i+j,tmp);
            tmp.pop_back();
		}
	}
    vector<vector<string>> partition(string s) {
		this->n=s.length();
        this->s=s;
        dfs(0,tmp);
        return ans;
    }
};