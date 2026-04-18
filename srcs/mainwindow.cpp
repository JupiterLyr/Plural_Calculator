#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPen>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setupChart();

    QList<QPushButton*> digitBtn = {
        ui->num_0, ui->num_1, ui->num_2, ui->num_3, ui->num_4,
        ui->num_5, ui->num_6, ui->num_7, ui->num_8, ui->num_9, ui->num_point
    };
    for (auto btn : digitBtn) {
        connect(btn, &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    }
    QList<QPushButton*> operatorBtn = {
        ui->operator_add, ui->operator_subtract, ui->operator_multiply, ui->operator_divide
    };
    for (auto btn : operatorBtn) {
        connect(btn, &QPushButton::clicked, this, &MainWindow::onOperatorClicked);
    }
    connect(ui->symbol_i, &QPushButton::clicked, this, [=]() { logic.inputSymbolI(); updateDisplay(); });
    connect(ui->symbol_angle, &QPushButton::clicked, this, [=]() { logic.inputSymbolAngle(); updateDisplay(); });
    connect(ui->operator_nxt, &QPushButton::clicked, this, [=]() { logic.moveNext(); updateDisplay(); });
    connect(ui->operator_pre, &QPushButton::clicked, this, [=]() { logic.movePrev(); updateDisplay(); });
    connect(ui->backspace, &QPushButton::clicked, this, [=]() { logic.backspace(); updateDisplay(); });
    connect(ui->operator_equal, &QPushButton::clicked, this, [=]() { logic.calculateEqual(); updateDisplay(); });
    connect(ui->all_clear, &QPushButton::clicked, this, [=]() { logic.clearAll(); updateDisplay(); });

    updateDisplay();
}

MainWindow::~MainWindow() {
    delete ui;
}

/// @brief 更新计算器显示内容
void MainWindow::updateDisplay() {
    ui->show_formulas->setText(logic.getFormulaDisplay());
    ui->show_result->setText(logic.getResultDisplay());
}

/// @brief 初始化坐标系
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

/// @brief 更新坐标系图像
/// @param re 复数的实部
/// @param im 复数的虚部
void MainWindow::updateChart(double re, double im) {
    pointSeries->clear();
    pointSeries->append(re, im);
    vectorSeries->clear();
    vectorSeries->append(0, 0);
    vectorSeries->append(re, im);

    double margin = 1.5;
    double maxAbs = std::max({ std::abs(re), std::abs(im), 1.0 });
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
    updateChart(cpx_res.getRe(), cpx_res.getIm());
}

void MainWindow::onDigitClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (btn) {
        logic.inputDigit(btn->text());
        updateDisplay();
    }
}

void MainWindow::onOperatorClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (btn) {
        logic.inputOperator(btn->text());
        updateDisplay();
    }
}