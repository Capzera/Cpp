/*
问题描述：P2232 力扣第288场周赛
给你一个下标从 0 开始的字符串 expression ，格式为 "<num1>+<num2>" ，其中 <num1> 和 <num2> 表示正整数。
请你向 expression 中添加一对括号，使得在添加之后， expression 仍然是一个有效的数学表达式，
并且计算后可以得到 最小 可能值。左括号 必须 添加在 '+' 的左侧，而右括号必须添加在 '+' 的右侧。
返回添加一对括号后形成的表达式?expression ，且满足 expression 计算得到 最小 可能值。
如果存在多个答案都能产生相同结果，返回任意一个答案。
生成的输入满足：expression 的原始值和添加满足要求的任一对括号之后 expression 的值，都符合 32-bit 带符号整数范围。
示例 1：
输入：expression = "247+38"
输出："2(47+38)
解释：表达式计算得到 2 * (47 + 38) = 2 * 85 = 170 。
注意 "2(4)7+38" 不是有效的结果，因为右括号必须添加在 '+' 的右侧。
可以证明 170 是最小可能值。
示例 2：
输入：expression = "12+34"
输出："1(2+3)4"
解释：表达式计算得到 1 * (2 + 3) * 4 = 1 * 5 * 4 = 20 。
示例 3：
输入：expression = "999+999"
输出："(999+999)"
解释：表达式计算得到 999 + 999 = 1998 。
提示：
3 <= expression.length <= 10
expression 仅由数字 '1' 到 '9' 和 '+' 组成
expression 由数字开始和结束
expression 恰好仅含有一个 '+'.
expression 的原始值和添加满足要求的任一对括号之后 expression 的值，都符合 32-bit 带符号整数范围
*/
class Solution {
public:
    string minimizeResult(string expression) {
        int n=expression.size(),i=0,minsum=INT_MAX,ansl=-1,ansr=-1;
        while (i<n&&expression[i]!='+') i++;
        string s=expression.substr(0,i),t=expression.substr(i+1,n-i-1),ans="";
        int len1=s.size(),len2=t.size();
        for (int i=0;i<len1;i++) {
            for (int j=0;j<len2;j++) {
                int plus1,plus2,mul1,mul2;
                plus1=stoi(s.substr(i,len1-i));
                plus2=stoi(t.substr(0,j+1));
                if (i==0) mul1=1;
                else mul1=stoi(s.substr(0,i));
                if (j==len2-1) mul2=1;
                else mul2=stoi(t.substr(j+1,len2-j-1));
                int sum=mul1*(plus1+plus2)*mul2;
                if (sum<minsum) {
                    minsum=sum;
                    ansl=i;
                    ansr=j;
                }
            }
        }
        ans=s.substr(0,ansl)+"("+s.substr(ansl,len1-ansl)+"+"+t.substr(0,ansr+1)+")"+t.substr(ansr+1,len2-ansr-1);
        return ans;
    }
};