#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

public slots:
    void showMainMenu();
    void showChallengeMenu();

private slots:
    void startEndlessMode();
    void startChallengeMode(int difficulty); // 0:简单, 1:中等, 2:困难
    void startExtremeMode();
    void showLeaderboard();
    void showSettings();

private:
    QStackedWidget *stackedWidget;

    QWidget *menuWidget;
    QWidget *challengeMenuWidget;

    void setupMainMenu();
    void setupChallengeMenu();

protected:
    // 核心重载：接管窗口缩放事件，动态调整所有字体大小
    void resizeEvent(QResizeEvent *event) override;
};

#endif // MAINWINDOW_H