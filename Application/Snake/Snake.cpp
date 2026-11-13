#include"Snake.h"

void Snake::Show_information(int& score,int& sleeptime)
{
    Pos(44, 4);
    cout << "得分: " << score<<"     ";//空格调整相对位置
    cout << "速度: "<<  sleeptime;
}

void Snake::overout(const int score)                    //游戏结束显示
{
    Pos(44, 14);
    cout << "游戏结束";
    Pos(44, 18);
    cout << "得分：" << score;
}

void Snake::Gameover(int& score,int &x)                 //游戏结束原因及得分
{
    //撞墙
    if (snakepoint[0].x >= 100 || snakepoint[0].x < 22 || snakepoint[0].y >= 36 || snakepoint[0].y < 6)
    {
        system("cls");      //清屏
        Pos(44, 10);
        cout << "撞墙啦，头太铁";
        overout(score);
        score = 0;
        Sleep(3000);
        exit(0);            //退出
    }
    //撞到自身
    for (int i = 1; i < snakepoint.size(); i++)
        if (snakepoint[0].x == snakepoint[i].x && snakepoint[0].y == snakepoint[i].y)
        {
            system("cls");  //清屏
            Pos(44, 10);
            cout << "撞到自身啦，肯定很痛";
            overout(score);
            score = 0;
            Sleep(3000);
            exit(0);       //退出
        }
}

void Snake::Control(Snakepoint& nexthead)
{
    static char Direction = 'R';                //静态变量防止改变移动方向后重新改回来，，将蛇设置为初始向右走

    if (_kbhit())
    {
        char temp = _getch();                   //临时变量储存键盘输入的值

        if ((temp == 'd') && Direction != 'L')
        {
            Direction = 'R';
        }
        else if ((temp == 'a') && Direction != 'R')
        {
            Direction = 'L';
        }
        else if ((temp == 'w') && Direction != 'D')
        {
            Direction = 'U';
        }
        else if ((temp == 's') && Direction != 'U')
        {
            Direction = 'D';
        }
    }

    switch (Direction)                          //根据Direction的值来确定蛇的移动方向
    {
    case 'R':
        nexthead.x = snakepoint.front().x + 2;  //新的蛇头的头部等于容器内第一个数据(旧蛇头)x坐标+2 
        nexthead.y = snakepoint.front().y;
        break;
    case 'L':
        nexthead.x = snakepoint.front().x - 2;
        nexthead.y = snakepoint.front().y;
        break;
    case 'U':
        nexthead.x = snakepoint.front().x;
        nexthead.y = snakepoint.front().y - 1;  //因为控制台的x长度是y的一半，所以用两个x做蛇头，需要的坐标是二倍
        break;
    case 'D':
        nexthead.x = snakepoint.front().x;
        nexthead.y = snakepoint.front().y + 1;
    }
}

void Snake::sleep(int& score, int& sleeptime)
{
    if (score < 10)
    {
        sleeptime = 120;
    }
    else if (score <20)
    {
        sleeptime = 80 ;
    }
    else if (score <30)
    {
        sleeptime = 50 ;
    }
    else
    {
        sleeptime = 30 ;
    }
    Sleep(sleeptime);                                   //速度随分数加快
}

void Snake::move(Food& food, int& score,int& sleeptime,int& x)
{
    Snakepoint nexthead;
    Control(nexthead);
    snakepoint.insert(snakepoint.begin(), nexthead);    //将蛇头插入容器的头部

    Show_information(score ,sleeptime);
    Gameover(score,x);

    if (snakepoint[0].x == food.getFood_x() && snakepoint[0].y == food.getFood_y()) //蛇头与食物重合
    {
        Pos(snakepoint[0].x, snakepoint[0].y);          //吃到食物时这次蛇没有移动，所以蛇会卡顿一下
        cout << "¤";                                   //重新输出一下蛇头和第一节蛇身让蛇不卡顿
        Pos(snakepoint[1].x, snakepoint[1].y);
        cout << "●";

        score++;                                        //吃到食物得分+1
        food.randfood();                                //重新产生一个食物
        return ;                                        //直接结束本次移动
    }

    //蛇每移动一次，容器向前动一次，打印出来的部分除蛇尾意外其余都被覆盖，所以最后清除蛇尾
    for (int i = 0; i < snakepoint.size(); i++)         //遍历容器，判断食物与蛇身是否重合并输出整条蛇
    {                                            
        Pos(snakepoint[i].x, snakepoint[i].y);
        if (!i)                                         //蛇头
            cout << "¤";
        else
            cout << "●";

        //如果食物刷新到了蛇身上，则重新产生一个食物
        if (snakepoint[i].x == food.getFood_x() && snakepoint[i].y == food.getFood_y())
            food.randfood();
    }

    Pos(snakepoint.back().x, snakepoint.back().y);      //在容器尾部的地方输出空格 清除画面上的蛇尾
    cout << "  ";
    snakepoint.pop_back();                              //删除容器中最后一个数据   清除数据上的蛇尾
}

Snake::Snake()
{
    Direction = 'R';
    Snakepoint temp;                                    //临时结构变量用于创建蛇
    for (int i = 5; i >= 0; i--)                        //反向创建初始蛇身，初始蛇头朝右
    {
        temp.x = 22 + (i << 1);                         //偶数 在蛇头左移生成蛇身
        temp.y = 7;
        snakepoint.push_back(temp);                     //在蛇尾尾插入临时变量
    }
}