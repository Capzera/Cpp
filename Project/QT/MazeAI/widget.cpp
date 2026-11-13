#include "widget.h"
#include <QPainter>
#include <QShortcut>
#include <QKeySequence>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QApplication>
#include <QScreen>
#include <random>
#include <algorithm>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    // ----- 动态计算格子大小，适配屏幕 -----
    QScreen *screen = QApplication::primaryScreen();
    QRect screenRect = screen->availableGeometry();
    int maxWidth = screenRect.width() * 0.85;
    int maxHeight = screenRect.height() * 0.85;
    int maxSize = std::min(maxWidth, maxHeight);
    cellSize = maxSize / SIZE;
    if (cellSize < 10) cellSize = 10;
    int winSize = cellSize * SIZE;
    setFixedSize(winSize, winSize);
    move(screenRect.x() + (screenRect.width() - winSize) / 2,
         screenRect.y() + (screenRect.height() - winSize) / 2);

    setStyleSheet("background-color: #0f0f19;");

    resetMaze();

    // ---------- 快捷键绑定 ----------
    QShortcut *solveShortcut1 = new QShortcut(QKeySequence(Qt::Key_Return), this);
    connect(solveShortcut1, &QShortcut::activated, this, &Widget::onStartSolve);

    QShortcut *solveShortcut2 = new QShortcut(QKeySequence(Qt::Key_Enter), this);
    connect(solveShortcut2, &QShortcut::activated, this, &Widget::onStartSolve);

    QShortcut *quitShortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    connect(quitShortcut, &QShortcut::activated, this, &Widget::onQuit);
}

Widget::~Widget() {}

void Widget::resetMaze()
{
    maze.assign(SIZE, std::vector<int>(SIZE, 1));
    headR = headC = -1;
    isSolving = false;
    isGenerated = false;
    mazeSolved = false;

    generateMaze(1, 1);

    startR = 1;
    startC = 1;
    targetR = SIZE - 2;
    targetC = SIZE - 2;
    maze[targetR][targetC] = 2;

    isGenerated = true;
    update();
}

void Widget::delay(int ms)
{
    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < ms) {
        QCoreApplication::processEvents();
    }
}

void Widget::generateMaze(int r, int c)
{
    maze[r][c] = 0;

    int dx[] = {0, 1, 0, -1};
    int dy[] = {1, 0, -1, 0};
    int dirs[] = {0, 1, 2, 3};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(std::begin(dirs), std::end(dirs), gen);

    for (int i = 0; i < 4; ++i) {
        int nr = r + dy[dirs[i]] * 2;
        int nc = c + dx[dirs[i]] * 2;

        if (nr > 0 && nr < SIZE && nc > 0 && nc < SIZE && maze[nr][nc] == 1) {
            maze[r + dy[dirs[i]]][c + dx[dirs[i]]] = 0;
            generateMaze(nr, nc);
        }
    }
}

bool Widget::solveDFS(int r, int c)
{
    headR = r;
    headC = c;

    if (r == targetR && c == targetC) {
        maze[r][c] = 3;
        update();
        return true;
    }

    maze[r][c] = 3;
    update();
    delay(40);  // 加快速度

    // ===== 修改方向顺序：优先上、左，再下、右 =====
    // 上 (dr=-1, dc=0) , 左 (dr=0, dc=-1) , 下 (dr=1, dc=0) , 右 (dr=0, dc=1)
    int dr[] = {-1, 0, 1, 0};
    int dc[] = {0, -1, 0, 1};

    for (int i = 0; i < 4; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];

        if (nr >= 0 && nr < SIZE && nc >= 0 && nc < SIZE &&
            (maze[nr][nc] == 0 || maze[nr][nc] == 2)) {
            if (solveDFS(nr, nc)) {
                return true;
            }
            // 回溯：拉回蛇头并停顿
            headR = r;
            headC = c;
            update();
            delay(40);  // 加快回溯停顿
        }
    }

    maze[r][c] = 4;
    update();
    return false;
}

// ---------- 槽函数 ----------
void Widget::onStartSolve()
{
    if (mazeSolved) {
        resetMaze();
        return;
    }

    if (isGenerated && !isSolving) {
        isSolving = true;
        bool found = solveDFS(startR, startC);
        isSolving = false;
        mazeSolved = true;
        update();
    }
}

void Widget::onQuit()
{
    QApplication::quit();
}

// ---------- 绘制 ----------
void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int cs = cellSize;
    QFont font("Microsoft YaHei", cs / 1.5, QFont::Bold);
    painter.setFont(font);

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            QRect rect(j * cs, i * cs, cs, cs);

            if (maze[i][j] == 1) {
                painter.fillRect(rect, QColor(0, 200, 255, 200));
            } else if (maze[i][j] == 2) {
                painter.setBrush(QColor(255, 50, 50));
                painter.setPen(Qt::NoPen);
                painter.drawEllipse(rect.center(), cs/3, cs/3);
            } else if (maze[i][j] == 3) {
                QRect pathRect = rect.adjusted(4, 4, -4, -4);
                painter.fillRect(pathRect, QColor(0, 255, 100));
            } else if (maze[i][j] == 4) {
                QRect deadRect = rect.adjusted(8, 8, -8, -8);
                painter.fillRect(deadRect, QColor(80, 80, 80));
            }
        }
    }

    QRect startRect(startC * cs, startR * cs, cs, cs);
    painter.setPen(QColor(255, 200, 0));
    painter.drawText(startRect, Qt::AlignCenter, "★");

    if ((isSolving || mazeSolved) && headR != -1 && headC != -1) {
        QRect headRect(headC * cs, headR * cs, cs, cs);
        QRect innerHeadRect = headRect.adjusted(2, 2, -2, -2);
        painter.fillRect(innerHeadRect, QColor(255, 255, 0));
        painter.setPen(Qt::black);
        painter.drawText(headRect, Qt::AlignCenter, "★");
    }
}