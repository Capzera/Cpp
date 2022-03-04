class Solution {
public:
    vector<vector<int>> generate(int numRows) {
        vector<vector<int>> yh(numRows);
        int i,j;
        for(i=0;i<numRows;i++)
        {
            yh[i].resize(i+1);
            yh[i][0]=yh[i][i]=1;
            for(j=1;j<i;j++) yh[i][j]=yh[i-1][j-1]+yh[i-1][j];
        }
        return yh;
    }
};