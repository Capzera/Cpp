#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFont>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QDialog>
#include <QMessageBox>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private slots:
    void onClickBtn();
private:
    void init();
    Ui::MainWindow *ui;
    QPushButton *btn;
    QLineEdit *le;
};
#endif // MAINWINDOW_H
