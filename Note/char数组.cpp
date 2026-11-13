1. C++ 字符数组
	1.1 字符数组可以直接输入输出，不需要依赖循环
	1.2 字符串比较函数 int strcmp(char a[], char b[]);
		如果 a < b 输出 -1
		如果 a = b 输出 0
		如果 a > b 输出 1
	1.3 字符串拷贝函数 void strcpy(char a[], char b[]);
		把字符串 b 完整拷贝到字符串 a。
		必须保证strlen(a) >= strlen(b)，否则 RE。
	1.4 字符串链接函数 void strcat(char a[], char b[]);
		把字符串 b 添加到 a 末尾。
		必须保证strlen(a) >= strlen(b)，否则 RE。
	1.5 求字符串长度函数 int strlen(char a[]);
		获取 a 的长度
	1.6 字符数组末尾存在一个看不见的 '\0'，表示字符串终止
2. C语言输入输出
		2.1.1 C语言输入可以简化部分需要做字符串分割的题目
		2.1.2 scanf和prinf本身就没有输入输出流，输入输出效率极高  快读快写 > scanf > cin
	2.2 占位符类型 %(格式化符号)
		%d int
		%c char
		%lld long long
		%f float
		%lf double
		%s char[]
	2.3 占位符修饰数(格式化场宽)
		2.3.1 %x.yd 系列
			x > 0 右端对齐，不足 x 位左边补空格
			x < 0 左端对齐，不足 x 位右侧补空格
			y : 不足 y 位补前导0
		2.3.2 %x.yf 系列 默认保留6位小数
			x > 0 右端对齐，不足 x 位左边补空格
			x < 0 左端对齐，不足 x 位右侧补空格
			y : 保留 y 位小数
3. string 
	3.1 字符串定义
	3.2 字符串的内置函数
		3.2.1 size_t size(); 获取字符串的长度
		3.2.2 size_t length(); 获取字符串的长度
		3.2.3 void pop_back(); 删掉字符串最后一位字符
		3.2.4 char back(); 获取字符串最后一位的字符
		*3.2.5.1* string substr(int x); 从字符串下标x开始截取到字符串末尾
		*3.2.5.2* string substr(int x, int y); 从字符串下标x开始截取y个字符
		**3.2.6** size_t find(string t); 
			从当前字符串找子串t，返回出现的第一个位置
			如果找不到，则返回结果可以和-1做比较
			因为找不到返回的值为string::npos，可以和-1划等号
		*3.2.7* void replace(int x, int y, string s);
		把原始串中从下标x开始连续y个字符替换成s
