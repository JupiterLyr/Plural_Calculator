#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QFile>

void load_qss(QString&& dir, QApplication& a) {
    QFile styleFile(dir);
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        a.setStyleSheet(styleSheet);
        styleFile.close();
    }
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    load_qss(":/resources/calculator.qss", app);
    MainWindow win;
    win.show();
    return app.exec();
}
