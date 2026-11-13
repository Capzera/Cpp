/*
问题描述：P0059
给你一个正整数?n ，生成一个包含 1 到?n2?所有元素，且元素按顺时针顺序螺旋排列的?n x n 正方形矩阵 matrix 
示例 1：
输入：n = 3
输出：[[1,2,3],[8,9,4],[7,6,5]]
示例 2：
输入：n = 1
输出：[[1]]
提示：
1 <= n <= 20
*/
class Solution {
public:
    vector<vector<int>> generateMatrix(int n) {
        int i = 0, j = 0, cnt = 1, direct = 0;
        int U, D, L, R;
        U = L = -1;
        D = R = n;
        vector<vector<int> > matrix(n, vector<int> (n));
        while (cnt <= n * n) {
            matrix[i][j] = cnt++;
            if (direct == 0) {
                j++;
                if (j == R) {
                    j = R - 1;
                    U++;
                    direct = 1;
                }
            }
            if (direct == 1) {
                i++;
                if (i == D) {
                    i = D - 1;
                    R--;
                    direct = 2;
                }
            }
            if (direct == 2) {
                j--;
                if (j == L) {
                    j = L + 1;
                    D--;
                    direct = 3;
                }
            }
            if (direct == 3) {
                i--;
                if (i == U) {
                    i = U + 1;
                    j++;
                    L++;
                    direct = 0;
                }
            }
        }
        return matrix;
    }
};