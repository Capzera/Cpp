/*
问题描述：P5253 力扣周赛第286场
给你一个整数数组?queries?和一个 正?整数?intLength?，请你返回一个数组?answer?，
其中?answer[i] 是长度为?intLength?的?正回文数 中第?queries[i]?小的数字，如果不存在这样的回文数，则为 -1?。
回文数 指的是从前往后和从后往前读一模一样的数字。回文数不能有前导 0 。
示例 1：
输入：queries = [1,2,3,4,5,90], intLength = 3
输出：[101,111,121,131,141,999]
解释：
长度为 3 的最小回文数依次是：
101, 111, 121, 131, 141, 151, 161, 171, 181, 191, 201, ...
第 90 个长度为 3 的回文数是 999 。
示例 2：
输入：queries = [2,4,6], intLength = 4
输出：[1111,1331,1551]
解释：
长度为 4 的前 6 个回文数是：
1001, 1111, 1221, 1331, 1441 和 1551 。
提示：
1 <= queries.length <= 5 * 104
1 <= queries[i] <= 109
1 <= intLength?<= 15
*/
class Solution {
public:
    vector<long long> kthPalindrome(vector<int>& queries, int intLength) {
        vector<long long> ans;
        int exp=intLength/2-1;//exp保存回文数的前半部分的10的次方级
		if (intLength%2) exp++;//对于奇数，还要枚举中间的那一位
		int MAX=9*pow(10,exp);
        for (int i=0;i<queries.size();i++) {
            int j=queries[i]-1+pow(10,exp);//exp加queries[i]-1为第j个数的前半部分
            if (queries[i]>MAX) {//超出部分特殊判断
                ans.push_back(-1);
                continue;
             }
			string s1=to_string(j),s2;
			s2=s1.substr(0,exp-intLength%2+1);//如果为奇数，s2保存s1的前exp-1位，否则s2=s1
			reverse(s2.begin(),s2.end());//回文对称
			ans.push_back(stoll(s1+s2));//保存答案
        }
        return ans; 
    }
};