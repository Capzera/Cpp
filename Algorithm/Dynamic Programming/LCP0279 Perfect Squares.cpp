/*
问题描述：P0279
给你一个整数 n ，返回 和为 n 的完全平方数的最少数量 。
完全平方数 是一个整数，其值等于另一个整数的平方；换句话说，其值等于一个整数自乘的积。
例如，1、4、9 和 16 都是完全平方数，而 3 和 11 不是。
示例?1：
输入：n = 12
输出：3 
解释：12 = 4 + 4 + 4
示例 2：
输入：n = 13
输出：2
解释：13 = 4 + 9
提示：
1 <= n <= 104
*/

//动态规划 算法时间复杂度O（n^1.5） 空间复杂度O(N)；
class Solution {
public:
    int numSquares(int n) {
        vector<int> dp(n+1);
        int power=2;
        dp[1]=1;
        for(int i=2;i<=n;i++)
        {
            if (i==power*power)
            {
                power++;
                dp[i]=1;
            }
            else
            {
                dp[i]=INT_MAX;
                for(int j=1;j<power;j++)
                {
                    dp[i]=min(dp[i],dp[i-j*j]+1);
                }
            }
        }
        return dp[n];
    }
};
//数学算法——四平方和定理 时间复杂度O（n^0.5） 空间复杂度O（1）
class Solution {
public:
    // 判断是否为完全平方数
    bool isPerfectSquare(int x) {
        int y = sqrt(x);
        return y * y == x;
    }

    // 判断是否能表示为 4^k*(8m+7)
    bool checkAnswer4(int x) {
        while (x % 4 == 0) {
            x /= 4;
        }
        return x % 8 == 7;
    }

    int numSquares(int n) {
        if (isPerfectSquare(n)) {
            return 1;
        }
        if (checkAnswer4(n)) {
            return 4;
        }
        for (int i = 1; i * i <= n; i++) {
            int j = n - i * i;
            if (isPerfectSquare(j)) {
                return 2;
            }
        }
        return 3;
    }
};
//