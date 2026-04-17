#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QChart>
#include <QLineSeries>
#include <QMainWindow>
#include <QScatterSeries>
#include <QValueAxis>
#include "calculate.h"
#include "complex.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void requestUpdate(Complex cpx);
    void onButtonClicked();

private:
    Ui::MainWindow* ui;
    Calculator* calc;

    QChart* chart;
    QLineSeries* realAxisSeries;   // 实轴
    QLineSeries* imagAxisSeries;   // 虚轴
    QScatterSeries* originSeries;  // 原点
    QScatterSeries* pointSeries;   // 复数点
    QLineSeries* vectorSeries;     // 向量
    QValueAxis* axisX;
    QValueAxis* axisY;

    void setupChart();
    void updateComplex(double re, double im);
};

#endif // MAINWINDOW_H
