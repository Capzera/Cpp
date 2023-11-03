%{Cpp:LicenseTemplate}\
  /********************************************
  *	> Project Name :	%{ProjectName}
  *	> File Name    :   	%{CppFileName}
  *	> Author       :   	Capzera
  *	> QQ-Mail      :   	1786126188@qq.com
  *	> Create Time  :	%{CurrentDate:ISO} %{CurrentTime:ISO}
  *   > Tips		   :	None
  *********************************************/
  #include "mainwindow.h"
  #include <QApplication>

  int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
