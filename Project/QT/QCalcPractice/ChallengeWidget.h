#ifndef CHALLENGEWIDGET_H
#define CHALLENGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

// 复用我们在无尽模式中写好的画板
class MathCanvas;

class ChallengeWidget : public QWidget {
    Q_OBJECT
public:
    explicit ChallengeWidget(QWidget *parent = nullptr);

    // 新增：接收从主菜单传来的难度设定
    void setDifficulty(int diff);

signals:
    void backToMenuRequested();

private slots:
    void startGame();
    void generateNextQuestion();
    void verifyAnswer();
    void loadHighScore();

private:
    void setupUI();
    int generateValidNumber(int digits);
    void updateStatsUI();
    void saveHighScore();
    void gameOver();

    MathCanvas *canvas;

    // 移除了 difficultyCombo，现在只有题型选择
    QComboBox *levelComboBox;
    QPushButton *startButton;
    QPushButton *backButton;
    QPushButton *checkButton;
    QPushButton *nextButton;

    QLabel *livesLabel;
    QLabel *scoreLabel;
    QLabel *highScoreLabel;

    int m_difficulty; // 0:简单, 1:中等, 2:困难
    int maxLives;
    int currentLives;
    int currentScore;
    int highScore;
    bool isGameActive;

    long long currentNumA;
    long long currentNumB;
};

#endif // CHALLENGEWIDGET_H