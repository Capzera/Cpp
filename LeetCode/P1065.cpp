/*
问题描述：P1065
给出?字符串 text 和?字符串列表 words, 
返回所有的索引对 [i, j] 使得在索引对范围内的子字符串 text[i]...text[j]（包括?i?和?j）属于字符串列表 words。
示例 1:
输入: text = "thestoryofleetcodeandme", words = ["story","fleet","leetcode"]
输出: [[3,7],[9,13],[10,17]]
示例 2:
输入: text = "ababa", words = ["aba","ab"]
输出: [[0,1],[0,2],[2,3],[2,4]]
解释: 
注意，返回的配对可以有交叉，比如，"aba" 既在 [0,2] 中也在 [2,4] 中
提示:
所有字符串都只包含小写字母。
保证 words 中的字符串无重复。
1 <= text.length <= 100
1 <= words.length <= 20
1 <= words[i].length <= 50
按序返回索引对 [i,j]（即，按照索引对的第一个索引进行排序，当第一个索引对相同时按照第二个索引对排序）。
*/
class Solution {
public:
    vector<vector<int>> indexPairs(string text, vector<string>& words) {
		vector<vector<int>> ans;
        unordered_map<string,int> um;
        int maxlen=-1,minlen=INT_MAX,n=text.size();
        for (auto i=0;i<words.size();i++) {
			um[words[i]]++;
            int n=words[i].length();
            maxlen=max(maxlen,n);
            minlen=min(minlen,n);
		}
        for (int i=0;i<n;i++) {
			for (int j=minlen;j<=maxlen;j++) {
                if (i+j>n) break;
				if (um.count(text.substr(i,j))) ans.push_back({i,i+j-1});
			}
		}
        return ans;
    }
};