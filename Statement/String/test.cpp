




void getnext(string t) {
    
}

function KMP(string s,string t) {
    //预设Next数组
    int i=0,j=0//i是主串s的指针，j是子串t的指针
    while (i<s.size()&&j<t.size()) {
        if (s[i]==t[j]) {//判断当前两个指针指向的字符是否相等
            i++;
            j++;
        }
        else j=next[j];
        if (j>=t.size()) 成功
        else 失败
    }
}
/*next[j]=(-1,其他);
next[j]==-1 j==0
next[j]==0  无前后缀匹配 
next[j]==其他情况 有前后缀匹配
/\
当前两个串指针指向的字符相等时
当前两个串指针指向的字符不等时
*/