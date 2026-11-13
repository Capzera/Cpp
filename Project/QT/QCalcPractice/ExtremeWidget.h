#ifndef EXTREMEWIDGET_H
#define EXTREMEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QTimer>
#include <QLineEdit>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QVBoxLayout>

// ================= 1. 长方形 3D 拟真能量管 =================
class VerticalHealthBar : public QWidget {
    Q_OBJECT
public:
    explicit VerticalHealthBar(QWidget *parent = nullptr);
    void setTimeMs(int timeMs);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    int m_timeMs;
};

// ================= 2. 霸体大字号并发题卡 =================
class CleanQuestionWidget : public QWidget {
    Q_OBJECT
public:
    CleanQuestionWidget(int a, int b, int rewardSec, int lifespanSec, QWidget *parent = nullptr);
    void start();
    void pause();
    void resume();
    QLineEdit *answerEdit;

signals:
    void answeredCorrectly(int rewardSec, CleanQuestionWidget *self);
    void expired(CleanQuestionWidget *self);

private slots:
    void verifyAnswer();
    void tick();
    void unfreeze();

private:
    int numA, numB;
    int m_rewardSec;
    int m_lifespanMs;
    int m_elapsedMs;
    bool m_isFrozen;
    bool m_isPaused;

    QTimer *lifeTimer;
    QProgressBar *progBar;
    QLabel *equationLabel;
};

// ================= 3. 极限模式主控端 =================
class ExtremeWidget : public QWidget {
    Q_OBJECT
public:
    explicit ExtremeWidget(QWidget *parent = nullptr);
    ~ExtremeWidget();

protected:
    void showEvent(QShowEvent *event) override;

    // [开发者模式] 拦截全局键盘事件
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void backToMenuRequested();

private slots:
    void startGame();
    void gameTick();
    void handleQuestionCorrect(int rewardSec, CleanQuestionWidget *q);
    void handleQuestionExpired(CleanQuestionWidget *q);
    void onBackButtonClicked();

private:
    void setupUI();
    int generateValidNumber(int digits);
    void spawnQuestionAtSlot(int index, int rewardSec, int lifespanSec);

    void updateStatsUI();
    void loadHighScore();
    void saveHighScore();

    bool hasGameSave();
    void saveGameProgress();
    void loadGameProgress();
    void clearGameProgress();
    void gameOver();

    void showTimeBonusAnimation(const QPoint &pos, int reward);

    QComboBox *levelComboBox;
    QPushButton *startButton;
    QPushButton *backButton;

    QLabel *scoreLabel;
    QLabel *highScoreLabel;

    VerticalHealthBar *healthBar;
    QWidget *questionsContainer;
    QVBoxLayout *questionsLayout;

    QTimer *mainLoopTimer;

    int timeLeftMs;
    int currentScore;
    int highScore;
    int m_currentPhase;
    bool isGameActive;

    CleanQuestionWidget* m_qSlots[3];

    // [开发者模式] 记录键盘输入的缓冲区
    QString m_cheatBuffer;
};

#endif // EXTREMEWIDGET_H