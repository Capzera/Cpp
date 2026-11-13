class Solution {
public:
    int climbStairs(int n) {
        int f[100001],i;
        f[1]=1;f[2]=2;
        for(i=3;i<=n;i++) f[i]=f[i-1]+f[i-2];
        return f[n];
    }
};