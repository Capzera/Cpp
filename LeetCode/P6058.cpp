/*
问题描述：P6058 力扣第291场周赛
Alice 在给 Bob 用手机打字。数字到字母的 对应?如下图所示。
为了 打出?一个字母，Alice 需要 按?对应字母 i?次，i?是该字母在这个按键上所处的位置。
比方说，为了按出字母?'s'?，Alice 需要按?'7'?四次。类似的， Alice 需要按?'5'?两次得到字母??'k'?。
注意，数字?'0' 和?'1'?不映射到任何字母，所以?Alice 不?使用它们。
但是，由于传输的错误，Bob 没有收到 Alice 打字的字母信息，反而收到了 按键的字符串信息?。
比方说，Alice 发出的信息为?"bob"?，Bob 将收到字符串?"2266622"?。
给你一个字符串?pressedKeys?，表示 Bob 收到的字符串，请你返回 Alice 总共可能发出多少种文字信息?。
由于答案可能很大，将它对?109 + 7?取余 后返回。
示例 1：
输入：pressedKeys = "22233"
输出：8
解释：
Alice 可能发出的文字信息包括：
"aaadd", "abdd", "badd", "cdd", "aaae", "abe", "bae" 和 "ce" 。
由于总共有 8 种可能的信息，所以我们返回 8 。
示例 2：
输入：pressedKeys = "222222222222222222222222222222222222"
输出：82876089
解释：
总共有 2082876103 种 Alice 可能发出的文字信息。
由于我们需要将答案对 109 + 7 取余，所以我们返回 2082876103 % (109 + 7) = 82876089 。
提示：
1 <= pressedKeys.length <= 105
pressedKeys 只包含数字?'2'?到?'9'?。
*/
class Solution {
public:
    long long ans=1;
    int mod=1e9+7;
    int countTexts(string Keys) {
        int n=Keys.size(),cnt=1;
        char last;
        vector<int> dp3(4),dp4(5);
        dp3[1]=dp4[1]=1;
        dp3[2]=dp4[2]=2;
        dp3[3]=dp4[3]=4;
        dp4[4]=8;
        last=Keys[0];
        for (int i=1;i<n;i++) {
            if (Keys[i]==last) {
                cnt++;
                if (last=='7'||last=='9') {
                    if (cnt==dp4.size()) {
                        int tmp=(((dp4[cnt-1]+dp4[cnt-2])%mod+dp4[cnt-3])%mod+dp4[cnt-4])%mod;
                        dp4.push_back(tmp);
                    }
                }
                else {
                    if (cnt==dp3.size()) {
                        int tmp=((dp3[cnt-1]+dp3[cnt-2])%mod+dp3[cnt-3])%mod;
                        dp3.push_back(tmp);
                    }
                }
            }
            if (Keys[i]!=last||i==n-1) {
                if (last=='7'||last=='9') ans=ans*dp4[cnt]%mod;
                else ans=ans*dp3[cnt]%mod;
                last=Keys[i];
                cnt=1;
            }
        }
        
        return ans;
    }
};
/*


*/