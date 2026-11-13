/*
问题描述：
给定n个十六进制正整数，输出它们对应的八进制数。
输入格式
　　输入的第一行为一个正整数n （1<=n<=10）。
　　接下来n行，每行一个由0~9、大写字母A~F组成的字符串，表示要转换的十六进制正整数，每个十六进制数长度不超过100000。
输出格式
　　输出n行，每行为输入对应的八进制正整数。
　　【注意】
　　输入的十六进制数不会有前导0，比如012A。
　　输出的八进制数也不能有前导0。
样例输入
　　2
　　39
　　123ABC
样例输出
　　71
　　4435274
*/
#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n;
    int str_length;
    cin>>n;
    string in_str[n],out_str[n],str,octstr,binarystr;
    for(int i=0; i<n; i++)cin>>in_str[i];
    for(int i=0; i<n; i++)
    {
        binarystr="";
        str = in_str[i];
        str_length = str.length();
        for(int j=0; j<str_length; j++)
        {
            switch(str[j]){
                case '0':binarystr+="0000";break;
                case '1':binarystr+="0001";break;
                case '2':binarystr+="0010";break;
                case '3':binarystr+="0011";break;
                case '4':binarystr+="0100";break;
                case '5':binarystr+="0101";break;
                case '6':binarystr+="0110";break;
                case '7':binarystr+="0111";break;
                case '8':binarystr+="1000";break;
                case '9':binarystr+="1001";break;
                case 'A':binarystr+="1010";break;
                case 'B':binarystr+="1011";break;
                case 'C':binarystr+="1100";break;
                case 'D':binarystr+="1101";break;
                case 'E':binarystr+="1110";break;
                case 'F':binarystr+="1111";break;
                default:break;
            }
        }
        str_length=binarystr.length();
        if(str_length%3==1) binarystr="00"+binarystr;
        if(str_length%3==2) binarystr="0"+binarystr;
        str_length =binarystr.length();
        octstr="";
        int t;
        for(int j=0; j<str_length; j+=3)
        {
            t = (binarystr[j]-'0')*4 + (binarystr[j+1]-'0')*2 + (binarystr[j+2]-'0')*1;
            stringstream ss;
            ss << t;
            octstr += ss.str();
        }
        for(int j=0; j<3; j++)
            if(octstr[0]=='0') octstr=octstr.erase(0,1);
        out_str[i] = octstr;
    }
    for(int i=0; i<n; i++)cout<<out_str[i]<<endl;
    system("pause");
    return 0;
}