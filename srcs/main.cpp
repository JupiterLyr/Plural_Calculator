#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QStringList>

static QString loadQss(const QStringList& paths) {
    QStringList parts;
    parts.reserve(paths.size());
    for (const auto& path : paths) {
        QFile file(path);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            parts << QString::fromUtf8(file.readAll());
        }
    }
    return parts.join('\n');
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    app.setStyleSheet(loadQss({
        ":/resources/global.qss",
        ":/resources/calculator.qss",
        ":/resources/chart.qss"
        }));

    MainWindow win;
    win.show();
    return app.exec();
}
