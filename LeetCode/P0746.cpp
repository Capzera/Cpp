class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        int f[1005],i,n=cost.size()+1;
        cost.push_back(0);
        memset(f,0,sizeof(f));
        f[0]=0;f[1]=cost[0];f[2]=cost[1];
        for(i=3;i<=n;i++) f[i]=cost[i-1]+min(f[i-1],f[i-2]);
        return f[n];
    }
};