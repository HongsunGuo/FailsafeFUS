#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OpenInstrument.h"
#include <vector>

class QVBoxLayout;
class QChartView;
class QChart;
class QLineSeries;
class QValueAxis;
class Task;
class SequenceListItem;
class OpenInstrument : public QMainWindow
{
    Q_OBJECT

public:
    OpenInstrument(QWidget *parent = nullptr);
    ~OpenInstrument();

private:
    const std::string m_taskFileName = "task.ini";
    const std::string m_seqFileName = "seq.ini";
    Ui::OpenInstrumentClass ui;
    QVBoxLayout* vLayout;
    QChartView* mainChartView;
    QChart* chart;
    QValueAxis* axisX;
    QValueAxis* axisY;
    //

    std::shared_ptr<std::vector<Task>> m_taskListPtr;
    std::shared_ptr<std::vector<SequenceListItem>> m_seqListPtr;
    int m_curSeqID = 0;
    //

    std::shared_ptr<QLineSeries> m_rawlinePtr;
    std::shared_ptr<QLineSeries> m_processedlinePtr;
    std::vector<std::shared_ptr<QLineSeries>> m_peaksLines;
    std::vector<std::shared_ptr<QLineSeries>> m_curvePtrs;
    //std::vector<std::pair<double, double>> m_curData;

    std::pair<std::vector<double>, std::vector<double>> m_curData;

    void configureToolbar();
    void configureLines();
    void addNewCurve(const std::pair<std::vector<double>, std::vector<double>>& data,
        const QString& legendStr, const QColor& col, int penWidth = 1, Qt::PenStyle = Qt::SolidLine);
 
private slots:
    void onTaskEditTriggered();
    void onSequenceEditTriggered();
    void onAnalyzeDataTriggered();
    void onSaveTriggered();
    void onRunTriggered();
};
