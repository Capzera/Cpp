# 导入函数库

#总体回测前要做的事情
def initialize(context):
    set_params()        #1设置策参数
    set_variables() #2设置中间变量
    set_backtest()   #3设置回测条件


#1
#设置策参数
def set_params():
    g.tc=15  # 调仓频率
    g.yb=63  # 样本长度
    g.N=20   # 持仓数目
    g.factors=["market_cap","roe"] # 用户选出来的因子
    # 因子等权重里1表示因子值越小越好，-1表示因子值越大越好
    g.weights=[[1],[-1]]
    
    
#2
#设置中间变量
def set_variables():
    g.t=0              #记录回测运行的天数
    g.if_trade=False   #当天是否交易
    
    
#3
#设置回测条件
def set_backtest():
    set_option('use_real_price', True)#用真实价格交易
    log.set_level('order', 'error')
