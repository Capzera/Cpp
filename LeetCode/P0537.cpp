/*
问题描述：P0537
复数 可以用字符串表示，遵循 "实部+虚部i" 的形式，并满足下述条件：
实部 是一个整数，取值范围是 [-100, 100]
虚部 也是一个整数，取值范围是 [-100, 100]
i2 == -1
给你两个字符串表示的复数 num1 和 num2 ，请你遵循复数表示形式，返回表示它们乘积的字符串。
示例 1：
输入：num1 = "1+1i", num2 = "1+1i"
输出："0+2i"
解释：(1 + i) * (1 + i) = 1 + i2 + 2 * i = 2i ，你需要将它转换为 0+2i 的形式。
示例 2：
输入：num1 = "1+-1i", num2 = "1+-1i"
输出："0+-2i"
解释：(1 - i) * (1 - i) = 1 + i2 - 2 * i = -2i ，你需要将它转换为 0+-2i 的形式。 
*/
class Solution {
public:
    void trans(string s,int &real,int &vir)
    {
        int i,minus=0,minus2=0;
        if (s[0]=='-')
        {
            minus2=1;
            i=1;
        }
        else i=0;
        while (s[i]!=43) real=real*10+s[i++]-48;
        if (minus2) real-=2*real;
        if (s[i+1]==45) 
        {
            minus=1;
            i+=2;
        }
        else i+=1;
        while (s[i]!='i') vir=vir*10+s[i++]-48;
        if (minus) vir-=2*vir;
    }
    string complexNumberMultiply(string num1, string num2) {
        int real1=0,real2=0,vir1=0,vir2=0;
        string ans="";
        trans(num1,real1,vir1);
        trans(num2,real2,vir2);
        ans+=to_string(real1*real2-vir1*vir2);
        ans+='+';
        ans+=to_string(real1*vir2+real2*vir1);
        ans+='i';
        return ans;
    }
};