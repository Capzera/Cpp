class Solution {
    void dfs(vector<vector<int>> &grid,int i,int j){
        if(i<0||i>=grid.size()||j<0||j>=grid[i].size()||grid[i][j]==0) return;
        int dx[]={-1,0,1,0},dy[]={0,-1,0,1};
        grid[i][j]=0;
        for(int x=0;x<4;x++) dfs(grid,i+dx[x],j+dy[x]);
    }
public:
    int numEnclaves(vector<vector<int>>& grid) {
        int m=grid.size(),n=grid[0].size(),i,j,sum=0;
        for(i=0;i<m;i++)
        {
            for(j=0;j<n;j++)
            {
                if (grid[i][j]&&(!i||i==m-1||!j||j==n-1))dfs(grid,i,j);
            }
        }
        for(i=0;i<m;i++)
            for(j=0;j<n;j++) 
                if (grid[i][j]) sum++;
        return sum;
    }
};