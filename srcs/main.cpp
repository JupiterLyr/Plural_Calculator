#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QFile>

QString load_qss(const QString& path) {
    QFile file(path);
    if (file.open(QFile::ReadOnly))
        return QLatin1String(file.readAll());
    return "";
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QString style;
    style += load_qss(":/resources/global.qss");
    style += load_qss(":/resources/calculator.qss");
    style += load_qss(":/resources/chart.qss");
    app.setStyleSheet(style);

    MainWindow win;
    win.show();
    return app.exec();
}
