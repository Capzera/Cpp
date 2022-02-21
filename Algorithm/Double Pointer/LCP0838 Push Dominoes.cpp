/*
问题描述：P0838
n 张多米诺骨牌排成一行，将每张多米诺骨牌垂直竖立。在开始时，同时把一些多米诺骨牌向左或向右推。
每过一秒，倒向左边的多米诺骨牌会推动其左侧相邻的多米诺骨牌。同样地，倒向右边的多米诺骨牌也会推动竖立在其右侧的相邻多米诺骨牌。
如果一张垂直竖立的多米诺骨牌的两侧同时有多米诺骨牌倒下时，由于受力平衡， 该骨牌仍然保持不变。
就这个问题而言，我们会认为一张正在倒下的多米诺骨牌不会对其它正在倒下或已经倒下的多米诺骨牌施加额外的力。
给你一个字符串 dominoes 表示这一行多米诺骨牌的初始状态，其中：
dominoes[i] = 'L'，表示第 i 张多米诺骨牌被推向左侧，
dominoes[i] = 'R'，表示第 i 张多米诺骨牌被推向右侧，
dominoes[i] = '.'，表示没有推动第 i 张多米诺骨牌。
返回表示最终状态的字符串。
示例 1：
输入：dominoes = "RR.L"
输出："RR.L"
解释：第一张多米诺骨牌没有给第二张施加额外的力。
示例 2：
输入：dominoes = ".L.R...LR..L.."
输出："LL.RR.LLRRLL.."
提示：
n == dominoes.length
1 <= n <= 105
dominoes[i] 为 'L'、'R' 或 '.'
*/
class Solution {
public:
    string pushDominoes(string dominoes) {
        int n=dominoes.size(),i=0;
        char l='L';//初始左边界向左
        while (i<n)
        {
            int j=i;
            while (j<n&&dominoes[j]=='.') j++;//找到连续的多米诺的右状态
            char r=(j<n? dominoes[j]:'R');//如果下一个状态为边界，自动赋一个右状态
            if (l==r)//左右状态相同，中间的骨牌向一个方向倒
            {
                while (i<j) dominoes[i++]=r;
            }
            else if (l=='R'&&r=='L')//方向相同，两边的骨牌向中间倒
            {
                int k=j-1;//右边界
                while (i<k)
                {
                    dominoes[i++]='R';//左边骨牌向右
                    dominoes[k--]='L';//右边骨牌向左
                }
            }
            l=r;
            i=j+1;
        }
        return dominoes;
    }
};