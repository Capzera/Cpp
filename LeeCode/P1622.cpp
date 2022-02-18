/*
问题描述：
请你实现三个 API append，addAll?和?multAll?来实现奇妙序列。
请实现?Fancy?类 ：
Fancy()?初始化一个空序列对象。
void append(val) 将整数?val?添加在序列末尾。
void addAll(inc)?将所有序列中的现有数值都增加?inc?。
void multAll(m)?将序列中的所有现有数值都乘以整数?m?。
int getIndex(idx) 得到下标为?idx?处的数值（下标从 0 开始），并将结果对?109 + 7?取余。如果下标大于等于序列的长度，请返回?-1?。
示例：
输入：
["Fancy", "append", "addAll", "append", "multAll", "getIndex", "addAll", "append", "multAll", "getIndex", "getIndex", "getIndex"]
[[], [2], [3], [7], [2], [0], [3], [10], [2], [0], [1], [2]]
输出：
[null, null, null, null, null, 10, null, null, null, 26, 34, 20]
解释：
Fancy fancy = new Fancy();
fancy.append(2);   // 奇妙序列：[2]
fancy.addAll(3);   // 奇妙序列：[2+3] -> [5]
fancy.append(7);   // 奇妙序列：[5, 7]
fancy.multAll(2);  // 奇妙序列：[5*2, 7*2] -> [10, 14]
fancy.getIndex(0); // 返回 10
fancy.addAll(3);   // 奇妙序列：[10+3, 14+3] -> [13, 17]
fancy.append(10);  // 奇妙序列：[13, 17, 10]
fancy.multAll(2);  // 奇妙序列：[13*2, 17*2, 10*2] -> [26, 34, 20]
fancy.getIndex(0); // 返回 26
fancy.getIndex(1); // 返回 34
fancy.getIndex(2); // 返回 20
*/
int mod =1000000007;
long long val[100005];
int cnt = 0;
class Fancy {
public:
    Fancy() 
    {
        cnt= 0;
    }
    void append(int v)
    {
        v=(v-add+mod)*qpow(mult,mod-2)%mod;
        val[cnt++]=v;
    }
    void addAll(int inc) 
    {
        add=(add+inc)%mod;
    }
    void multAll(int m) 
    {
        mult=(mult*m)%mod;
        add=(add*m)%mod;
    }
    int getIndex(int idx)
    {
        return idx >= cnt? -1:(val[idx]*mult+add)%mod;
    }
    inline int qpow(long long a, int b)
    {
        int ans = 1;
        a=(a%mod+mod)%mod;
        for (;b;b>>=1)
        {
            if (b&1) ans=(a*ans)%mod;
            a=(a*a)%mod;
        }
        return ans;
    }
    long long  mult=1,add=0;
};
/**
 * Your Fancy object will be instantiated and called as such:
 * Fancy* obj = new Fancy();
 * obj->append(val);
 * obj->addAll(inc);
 * obj->multAll(m);
 * int param_4 = obj->getIndex(idx);
 */