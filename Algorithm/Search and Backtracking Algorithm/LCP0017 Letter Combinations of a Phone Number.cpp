/*
问题描述：P0017
给定一个仅包含数字?2-9?的字符串，返回所有它能表示的字母组合。答案可以按 任意顺序 返回。
给出数字到字母的映射如下（与电话按键相同）。注意 1 不对应任何字母。
示例 1：
输入：digits = "23"
输出：["ad","ae","af","bd","be","bf","cd","ce","cf"]
示例 2：
输入：digits = ""
输出：[]
示例 3：
输入：digits = "2"
输出：["a","b","c"]
提示：
0 <= digits.length <= 4
digits[i] 是范围 ['2', '9'] 的一个数字。
*/
class Solution {
public:
    string digit;
    vector<string> db={
        {"abc"},
        {"def"},
        {"ghi"},
        {"jkl"},
        {"mno"},
        {"pqrs"},
        {"tuv"},
        {"wxyz"}
    };
    vector<string> ans;
    int n;
    void dfs(int i,string tmp)
    {
        if (i==n)
        {
            if (tmp.size())ans.push_back(tmp);
            return;
        }
        int num=digit[i]-50;
        for(int j=0;j<db[num].size();j++)
        {
            dfs(i+1,tmp+db[num][j]);
        }
    }
    vector<string> letterCombinations(string digits) {
        this->n=digits.size();
        this->digit=digits;
        dfs(0,"");
        return ans;
    }
};