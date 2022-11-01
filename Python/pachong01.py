#引入requests库
import requests

#要请求的url
url="https://www.baidu.com/"

# 反爬机制
header={'user-agent':'Mozilla/5.0 (iPad; CPU OS 11_0 like Mac OS X) AppleWebKit/604.1.34 (KHTML, like Gecko) '
                     'Version/11.0 Mobile/15A5341f Safari/604.1'}
response = requests.get(url,header)
# print(type(response))
# print(response.status_code)   #http请求的返回状态  200表示请求成功 404请求失败

#获得网页的源码
#方法一：
# response.encoding='utf-8'
# print(response.text)

# 方法二
#content是爬取下来的字符串
content = response.content.decode('utf-8')