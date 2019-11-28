#ifndef DYNOVIEW_H
#define DYNOVIEW_H

#include <QMainWindow>
#include <QtCore>
#include <QChart>
#include <QLineSeries>

QT_BEGIN_NAMESPACE
namespace Ui { class dynoview; }
QT_END_NAMESPACE

using namespace QtCharts;

class dynoview : public QMainWindow
{
    Q_OBJECT

public:
    dynoview(QWidget *parent = nullptr);
    ~dynoview();

public slots:
    void plot();

private:
    Ui::dynoview *ui;
    QTimer *timer;
    QLineSeries *series;
    QChart *chart;

    void connectToCAN();
    void connectControls();
    void setupCharts();
};
#endif // DYNOVIEW_H
