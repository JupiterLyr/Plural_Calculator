#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QChart>
#include <QLineSeries>
#include <QPen>
#include <QScatterSeries>
#include <QValueAxis>
#include "complex.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setupComplexChart();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupComplexChart() {
    QChart* chart = new QChart();
    chart->setAnimationOptions(QChart::NoAnimation);

    // X 轴设置
    QValueAxis* axisX = new QValueAxis();
    axisX->setRange(-6, 6);         // X 轴范围
    axisX->setTickCount(13);        // X 轴刻度数量（包括0）
    axisX->setLabelFormat("%.0f");  // 整数标签

    // Y 轴设置
    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(-8, 8);         // Y 轴范围
    axisY->setTickCount(17);        // Y 轴刻度数量（包括0）
    axisY->setLabelFormat("%.0f");

    // ========== 坐标轴样式 ==========

    QPen axisPen(QColor("#475569"), 1.5);
    axisX->setLinePen(axisPen);
    axisY->setLinePen(axisPen);

    // 刻度线颜色
    QPen tickPen(QColor("#94a3b8"), 1);
    axisX->setGridLinePen(tickPen);
    axisY->setGridLinePen(tickPen);

    // 标签样式
    QFont labelFont("Microsoft YaHei", 9);
    axisX->setLabelsFont(labelFont);
    axisY->setLabelsFont(labelFont);
    axisX->setLabelsColor(QColor("#64748b"));
    axisY->setLabelsColor(QColor("#64748b"));

    // 网格线 - 淡色虚线
    QPen gridPen(QColor("#e2e8f0"), 1, Qt::DashLine);
    axisX->setGridLinePen(gridPen);
    axisY->setGridLinePen(gridPen);
    axisX->setGridLineVisible(true);
    axisY->setGridLineVisible(true);

    // 实轴（X轴）：水平线 y=0
    QLineSeries* realAxis = new QLineSeries();
    realAxis->append(-6, 0);
    realAxis->append(6, 0);
    QPen realPen(QColor("#334155"), 2.5);  // 深灰加粗
    realAxis->setPen(realPen);
    chart->addSeries(realAxis);
    realAxis->attachAxis(axisX);
    realAxis->attachAxis(axisY);
    
    // 虚轴（Y轴）：垂直线 x=0
    QLineSeries* imagAxis = new QLineSeries();
    imagAxis->append(0, -8);
    imagAxis->append(0, 8);
    QPen imagPen(QColor("#334155"), 2.5);
    imagAxis->setPen(imagPen);
    chart->addSeries(imagAxis);
    imagAxis->attachAxis(axisX);
    imagAxis->attachAxis(axisY);
    
    // 原点高亮标记
    QScatterSeries* origin = new QScatterSeries();
    origin->append(0, 0);
    origin->setMarkerSize(10);
    origin->setColor(QColor("#f43f5e"));      // 红色
    origin->setBorderColor(QColor("#ffffff")); // 白边
    chart->addSeries(origin);
    origin->attachAxis(axisX);
    origin->attachAxis(axisY);
    
    // 隐藏图例
    chart->legend()->setVisible(false);

    // 设置视图
    ui->graphicsView->setChart(chart);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setStyleSheet("background: transparent; border: none;");
}