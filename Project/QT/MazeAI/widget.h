#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <vector>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onStartSolve();   // 处理 Enter/Return 按键
    void onQuit();         // 处理 Esc 按键

private:
    static const int N = 20;               // 迷宫复杂度（路径数量）
    static const int SIZE = 2 * N + 1;     // 网格尺寸 = 41

    int cellSize;                          // 每个格子的像素大小（动态计算）
    std::vector<std::vector<int>> maze;

    int startR, startC, targetR, targetC;
    int headR = -1, headC = -1;

    bool isSolving = false;
    bool isGenerated = false;
    bool mazeSolved = false;               // 标记当前迷宫是否已被求解

    void generateMaze(int r, int c);
    bool solveDFS(int r, int c);
    void resetMaze();                      // 重新生成迷宫并重置状态
    void delay(int ms);
};

#endif // WIDGET_H