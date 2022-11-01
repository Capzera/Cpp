/*
问题描述：P0097
给定三个字符串?s1、s2、s3，请你帮忙验证?s3?是否是由?s1?和?s2 交错 组成的。
两个字符串 s 和 t 交错 的定义与过程如下，其中每个字符串都会被分割成若干 非空 子字符串：
s = s1 + s2 + ... + sn
t = t1 + t2 + ... + tm
|n - m| <= 1
交错 是 s1 + t1 + s2 + t2 + s3 + t3 + ... 或者 t1 + s1 + t2 + s2 + t3 + s3 + ...
注意：a + b 意味着字符串 a 和 b 连接。
示例 1：
输入：s1 = "aabcc", s2 = "dbbca", s3 = "aadbbcbcac"
输出：true
示例 2：
输入：s1 = "aabcc", s2 = "dbbca", s3 = "aadbbbaccc"
输出：false
示例 3：
输入：s1 = "", s2 = "", s3 = ""
输出：true
提示：
0 <= s1.length, s2.length <= 100
0 <= s3.length <= 200
s1、s2、和 s3 都由小写英文字母组成
*/
class Solution {
public:
    bool isInterleave(string s1, string s2, string s3) {
		int l1=s1.size(),l2=s2.size(),l3=s3.size(),i=0,j=0;
        if ((l1+l2)!=l3) return false;
        vector<vector<bool>> dp(l1+1,vector<bool>(l2+1));
        dp[0][0]=1;
        while (i<l1&&s1[i]==s3[i]) dp[++i][0]=1;
        while (j<l2&&s2[j]==s3[j]) dp[0][++j]=1;
        for (i=1;i<=l1;i++){
			for(j=1;j<=l2;j++){
            	dp[i][j]=(dp[i][j-1]&&s2[j-1]==s3[i+j-1])||(dp[i-1][j]&&s1[i-1]==s3[i+j-1]);    
            }
		}
        return dp[l1][l2];
    }
};
/*
dp[i][j]=s1 i个字符 s2j个字符能否组成s3前i+j个字符
dp[1][3]=0
dp[1][4]=0
dp[2][3]=1

*/