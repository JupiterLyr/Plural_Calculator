#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPen>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setupChart();

    QList<QPushButton*> buttons = this->findChildren<QPushButton*>();
    for (auto btn : buttons) {
        connect(btn, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupChart() {
    chart = new QChart();
    chart->setAnimationOptions(QChart::NoAnimation);

    axisX = new QValueAxis();
    axisY = new QValueAxis();
    axisX->setLabelFormat("%.1f");
    axisY->setLabelFormat("%.1f");
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    realAxisSeries = new QLineSeries();
    imagAxisSeries = new QLineSeries();
    QPen axisPen(QColor("#334155"), 2.5);
    realAxisSeries->setPen(axisPen);
    imagAxisSeries->setPen(axisPen);

    originSeries = new QScatterSeries();
    originSeries->append(0, 0);
    originSeries->setMarkerSize(10);
    originSeries->setColor(QColor("#ef4444"));
    pointSeries = new QScatterSeries();
    pointSeries->setMarkerSize(10);
    pointSeries->setColor(QColor("#f43f5e"));
    vectorSeries = new QLineSeries();
    QPen vecPen(QColor("#3b82f6"), 2);
    vectorSeries->setPen(vecPen);

    chart->addSeries(realAxisSeries);
    chart->addSeries(imagAxisSeries);
    chart->addSeries(vectorSeries);
    chart->addSeries(originSeries);
    chart->addSeries(pointSeries);

    QList<QAbstractSeries*> seriesList = {
        realAxisSeries,
        imagAxisSeries,
        originSeries,
        pointSeries,
        vectorSeries
    };
    for (auto s : seriesList) {
        s->attachAxis(axisX);
        s->attachAxis(axisY);
    }
    chart->legend()->hide();

    ui->graphicsView->setChart(chart);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::updateComplex(double a, double b) {
    pointSeries->clear();
    pointSeries->append(a, b);
    vectorSeries->clear();
    vectorSeries->append(0, 0);
    vectorSeries->append(a, b);

    double margin = 1.5;
    double maxAbs = std::max({ std::abs(a), std::abs(b), 1.0 });
    double R = maxAbs + margin;
    double minX = -R;
    double maxX = R;
    double minY = -R;
    double maxY = R;

    axisX->setRange(minX, maxX);
    axisY->setRange(minY, maxY);
    realAxisSeries->clear();
    realAxisSeries->append(minX, 0);
    realAxisSeries->append(maxX, 0);
    imagAxisSeries->clear();
    imagAxisSeries->append(0, minY);
    imagAxisSeries->append(0, maxY);
}

void MainWindow::requestUpdate(Complex cpx_res) {
    updateComplex(cpx_res.getRe(), cpx_res.getIm());
}

void MainWindow::onButtonClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    QString val = btn->text();

    if (val == "AC") {
        calc->clear();
    } else if (val == "←") {
        calc->backspace();
    } else if (val == "=") {
        calc->inputToken("=");
        // 点击等号时，同步更新绘图
        requestUpdate(calc->getRawResult());
    } else {
        calc->inputToken(val);
    }
    // 实时显示到 Label
    ui->show_formulas->setText(calc->getFormula());
    ui->show_result->setText(calc->getDisplayResult());
}