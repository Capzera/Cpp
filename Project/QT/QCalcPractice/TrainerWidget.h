#ifndef TRAINERWIDGET_H
#define TRAINERWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include <QRect>

// 自定义画板，用于绘制横向等式与射线槽位
class MathCanvas : public QWidget {
    Q_OBJECT
public:
    explicit MathCanvas(QWidget *parent = nullptr);
    void setProblem(int a, int b);
    void setPracticeMode(bool on);
    void setWrongState(bool wrong);
    void setCorrectState(bool correct);

    QLineEdit *answerEdit;
    QLabel *statusLabel;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QString strA;
    QString strB;
    bool isPracticeMode;
    bool isWrong;
    bool isCorrect;

    // 布局坐标缓存
    QVector<QRect> aRects;
    QVector<QRect> bRects;
    QVector<QRect> boxRects;
    QRect symMulRect;
    QRect symEqRect;

    void updateLayout();
};

// 无尽模式（原练习器窗口）
class TrainerWidget : public QWidget {
    Q_OBJECT

public:
    explicit TrainerWidget(QWidget *parent = nullptr);

signals:
    void backToMenuRequested();

private slots:
    void generateNextQuestion();
    void verifyAnswer();
    void togglePracticeMode(int state);

private:
    QPushButton *backButton;
    QComboBox *levelComboBox;
    QCheckBox *practiceCheckBox;
    QPushButton *nextButton;
    QPushButton *checkButton;

    MathCanvas *canvas;

    long long currentNumA;
    long long currentNumB;

    void setupUI();
    int generateValidNumber(int digits);
};

#endif // TRAINERWIDGET_H