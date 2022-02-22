/*
问题描述：P0784
给定一个字符串?s?，通过将字符串?s?中的每个字母转变大小写，我们可以获得一个新的字符串。
返回 所有可能得到的字符串集合 。以 任意顺序 返回输出。
示例 1：
输入：s = "a1b2"
输出：["a1b2", "a1B2", "A1b2", "A1B2"]
示例 2:
输入: s = "3z4"
输出: ["3z4","3Z4"]
提示:
1 <= s.length <= 12
s?由小写英文字母、大写英文字母和数字组成
*/
class Solution {
public:
    vector<string> ans;
    vector<string> letterCasePermutation(string s) {
        for(int i=0;i<s.size();i++) if (s[i]>96&&s[i]<123) s[i]-=32;
        dfs(s,0);
        return ans;
    }
    void dfs(string &s,int i){
        while (i<s.size()&&s[i]>47&&s[i]<58) i++;
        if(i>=s.size()){
            ans.push_back(s);
            return;
        }
        s[i]+=32;
        dfs(s,i+1);
        s[i]-=32;
        dfs(s,i+1);
    }
};