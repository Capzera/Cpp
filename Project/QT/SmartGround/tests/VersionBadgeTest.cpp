#include <QApplication>
#include <QLabel>
#include <QRegularExpression>
#include <cstdio>
#include "core/AppVersion.h"
#include "ui/MainWindow.h"

static int failures = 0;

static void check(bool cond, const char* msg) {
    if (!cond) { printf("FAIL: %s\n", msg); failures++; }
    else printf("PASS: %s\n", msg);
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    const QString display = AppVersion::display();
    printf("display: %s\n", display.toUtf8().constData());
    check(QRegularExpression("^0\\.[0-9]+\\.[0-9]+$").match(display).hasMatch(),
          "version display uses the major.minor.build form");
    check(AppVersion::tooltip().contains(display), "tooltip embeds the version string");

    MainWindow w;
    QLabel* badge = nullptr;
    for (QLabel* l : w.findChildren<QLabel*>()) {
        if (l->objectName() == "versionBadge") badge = l;
    }
    check(badge != nullptr, "version badge label exists in the status bar");
    if (badge) {
        printf("badge: %s\n", badge->text().toUtf8().constData());
        check(badge->text() == display, "badge shows the current version");
        check(!badge->toolTip().isEmpty(), "badge carries a tooltip");
    }

    printf("\n%d failures\n", failures);
    return failures == 0 ? 0 : 1;
}
