/*
问题描述：P0074
编写一个高效的算法来判断?m x n?矩阵中，是否存在一个目标值。该矩阵具有如下特性：
每行中的整数从左到右按升序排列。
每行的第一个整数大于前一行的最后一个整数。
示例 1：
输入：matrix = [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target = 3
输出：true
示例 2：
输入：matrix = [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target = 13
输出：false
提示：
m == matrix.length
n == matrix[i].length
1 <= m, n <= 100
-104 <= matrix[i][j], target <= 104
*/
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int m=matrix.size();
        if (!m) return false;
        int n=matrix[0].size();
        int l=0,r=m,i=0,mid;
        while (l<r){
            mid=l+(r-l)/2;
            if (matrix[mid][0]==target) return true;
            if (matrix[mid][0]<target) {
                i=mid;
                l=mid+1;
            }
            else r=mid;
        }
        l=0;r=n;
        while (l<r){
            mid=l+(r-l)/2;
            if (matrix[i][mid]==target) return true;
            if (matrix[i][mid]>target) r=mid;
            else l=mid+1;
        }
        return false;
    }
};