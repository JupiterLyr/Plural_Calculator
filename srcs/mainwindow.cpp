#include "mainwindow.h"
#include <QIcon>
#include <QKeySequence>
#include <QPen>
#include <QShortcut>
#include <vector>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setFixedSize(458, 590);
    this->setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    this->setStatusBar(nullptr);  // 禁用状态栏
    this->setWindowIcon(QIcon(":/resources/icon.png"));

    digitBtn = {
        ui->num_0, ui->num_1, ui->num_2, ui->num_3, ui->num_4,
        ui->num_5, ui->num_6, ui->num_7, ui->num_8, ui->num_9, ui->num_point
    };
    for (auto btn : digitBtn) {
        connect(btn, &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    }
    operatorBtn = {
        ui->operator_add, ui->operator_subtract, ui->operator_multiply, ui->operator_divide
    };
    for (auto btn : operatorBtn) {
        connect(btn, &QPushButton::clicked, this, &MainWindow::onOperatorClicked);
    }
    elseBtnXceptAC = {
        ui->num_opposite, ui->symbol_i, ui->symbol_angle, ui->operator_pre, ui->operator_nxt,
        ui->backspace, ui->operator_equal, ui->form_abi, ui->form_atheta
    };
    connect(ui->num_opposite, &QPushButton::clicked, this, [=]() { logic.toggleSign(); updateDisplay(); });
    connect(ui->symbol_i, &QPushButton::clicked, this, [=]() { logic.inputSymbol_i(); updateDisplay(); });
    connect(ui->symbol_angle, &QPushButton::clicked, this, [=]() { logic.inputSymbol_angle(); updateDisplay(); });
    connect(ui->operator_nxt, &QPushButton::clicked, this, [=]() { logic.moveNext(); updateDisplay(); });
    connect(ui->operator_pre, &QPushButton::clicked, this, [=]() { logic.movePrev(); updateDisplay(); });
    connect(ui->backspace, &QPushButton::clicked, this, [=]() { logic.backspace(); updateDisplay(); });
    connect(ui->form_abi, &QPushButton::clicked, this, [=]() { logic.setFormatCartesian(); updateDisplay(); });
    connect(ui->form_atheta, &QPushButton::clicked, this, [=]() { logic.setFormatPolar(); updateDisplay(); });
    connect(ui->operator_equal, &QPushButton::clicked, this, [=]() { logic.calculateEqual(); updateDisplay(); });
    connect(ui->all_clear, &QPushButton::clicked, this, [=]() { logic.clearAll(); updateDisplay(); });

    connect(&logic, &CalculatorLogic::requestUpdateChart, this, &MainWindow::updateChart);

    setupChart();
    updateDisplay();
    initShortcuts();
}

MainWindow::~MainWindow() {
    delete ui;
}

/// @brief 按钮 + - * / = 的可用性转换
/// @param canOperate ``true`` - 可用 | ``false`` - 不可用
/// @param btnList 待操作按钮的指针列表
void MainWindow::btnAvailability(bool canOperate, QList<QPushButton*> btnList) {
    for (const auto& btn : btnList) {
        if (btn) btn->setEnabled(canOperate);
    }
}

/// @brief 更新计算器显示内容
void MainWindow::updateDisplay() {
    ui->show_formulas->setText(logic.getFormulaDisplay());
    ui->show_result->setText(logic.getResultDisplay());
    btnAvailability(!logic.getComplexEditing(), operatorBtn);
    if (logic.getErrorCondition()) {    // 错误状态禁用 AC 外的所有按钮
        btnAvailability(!logic.getErrorCondition(), digitBtn + operatorBtn + elseBtnXceptAC);
    }
    else if (!ui->num_0->isEnabled()) { // 数字键都被禁用，说明需要解禁
        btnAvailability(true, digitBtn + operatorBtn + elseBtnXceptAC);
    }
}

/// @brief 初始化坐标系
void MainWindow::setupChart() {
    chart = new QChart();
    chart->setAnimationOptions(QChart::NoAnimation);
    chart->setBackgroundVisible(false);

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
/// @param cpx_res 复数
void MainWindow::updateChart(Complex cpx_res) {
    double re = cpx_res.getRe(), im = cpx_res.getIm();
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

void MainWindow::initShortcuts() {
    struct Item {
        QKeySequence key;
        QPushButton* btn;
    };
    std::vector<Item> items = {  // 设置全部快捷键
        {QKeySequence(Qt::Key_0), ui->num_0},
        {QKeySequence(Qt::Key_1), ui->num_1},
        {QKeySequence(Qt::Key_2), ui->num_2},
        {QKeySequence(Qt::Key_3), ui->num_3},
        {QKeySequence(Qt::Key_4), ui->num_4},
        {QKeySequence(Qt::Key_5), ui->num_5},
        {QKeySequence(Qt::Key_6), ui->num_6},
        {QKeySequence(Qt::Key_7), ui->num_7},
        {QKeySequence(Qt::Key_8), ui->num_8},
        {QKeySequence(Qt::Key_9), ui->num_9},
        {QKeySequence(Qt::Key_Period), ui->num_point},
        {QKeySequence(Qt::Key_Backslash), ui->num_opposite},
        {QKeySequence(Qt::Key_Plus), ui->operator_add},
        {QKeySequence(Qt::Key_Minus), ui->operator_subtract},
        {QKeySequence(Qt::Key_Asterisk), ui->operator_multiply},
        {QKeySequence(Qt::Key_Slash), ui->operator_divide},
        {QKeySequence(Qt::Key_Enter), ui->operator_equal},
        {QKeySequence(Qt::Key_Return), ui->operator_equal},
        {QKeySequence(Qt::Key_Left), ui->operator_pre},
        {QKeySequence(Qt::Key_Right), ui->operator_nxt},
        {QKeySequence(Qt::Key_I), ui->symbol_i},
        {QKeySequence(Qt::Key_A), ui->symbol_angle},
        {QKeySequence(Qt::Key_D), ui->form_abi},
        {QKeySequence(Qt::Key_P), ui->form_atheta},
        {QKeySequence(Qt::Key_Backspace), ui->backspace},
        {QKeySequence(Qt::Key_Escape), ui->all_clear}
    };
    for (auto& it : items) {
        if (it.btn) {
            QShortcut* sc = new QShortcut(it.key, this);
            connect(sc, &QShortcut::activated, it.btn, &QPushButton::animateClick);
        }
    }
    // 设置悬浮提示
    ui->symbol_i->setToolTip("定义笛卡尔形式的复数 (I)");
    ui->symbol_angle->setToolTip("定义极坐标形式的复数 (A)");
    ui->form_abi->setToolTip("以笛卡尔形式显示 (D)");
    ui->form_atheta->setToolTip("以极坐标形式显示 (P)");
    ui->num_opposite->setToolTip("取反 (\\)");
}