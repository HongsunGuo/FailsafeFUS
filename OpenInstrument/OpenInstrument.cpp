#include <QtCharts>
#include "OpenInstrument.h"
#include "TaskEditor.h"
#include "SequenceEditor.h"
#include "FileMgr.h"
#include "Task.h"
#include "RunSeqDialog.h"
#include "DataProc.h"

using namespace std;

OpenInstrument::OpenInstrument(QWidget* parent)
    : QMainWindow(parent)
{
    m_taskListPtr = make_shared<vector<Task>>();
    m_seqListPtr = make_shared<vector<SequenceListItem>>();
    //load from ini file
    FileMgr::readTasksFromFile(*m_taskListPtr, m_taskFileName);
    FileMgr::readSeqListFromFile(*m_seqListPtr, m_seqFileName);
    //
    ui.setupUi(this);

    chart = new QChart();
    //chart->setTitle("Sample Chart");
    axisX = new QValueAxis();
    axisY = new QValueAxis();

    axisX->setGridLineVisible(true);
    axisX->setTickCount(10);
    axisY->setGridLineVisible(true);
    axisY->setTickCount(10);
    QPen gridPen;
    //gridPen.setColor(Qt::white);
    gridPen.setColor(Qt::gray);
    gridPen.setWidth(0.5);
    gridPen.setStyle(Qt::DashLine);
    axisX->setGridLinePen(gridPen);
    axisY->setGridLinePen(gridPen);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    chart->setTheme(QChart::ChartThemeLight);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setBackgroundVisible(false);

    mainChartView = new QChartView(chart);
    mainChartView->setRenderHint(QPainter::Antialiasing);
    
    //
    vLayout = new QVBoxLayout(ui.centralWidget);
    vLayout->addWidget(mainChartView);
    ui.centralWidget->setLayout(vLayout);

    configureLines();
    configureToolbar();
}

OpenInstrument::~OpenInstrument()
{}

void OpenInstrument::configureLines()
{
}

void OpenInstrument::configureToolbar() {
    //
    ui.mainToolBar->addAction(ui.actionOpen);
    ui.mainToolBar->addAction(ui.actionSave);
    ui.mainToolBar->addAction(ui.actionRun);
    ui.mainToolBar->addSeparator();
    ui.mainToolBar->addAction(ui.actionEdit_Tasks);
    ui.mainToolBar->addAction(ui.actionEdit_Sequence);
    ui.mainToolBar->addSeparator();
    ui.mainToolBar->addAction(ui.actionAnalyze_Data);
    connect(ui.actionEdit_Tasks, SIGNAL(triggered()), this, SLOT(onTaskEditTriggered()));
    connect(ui.actionEdit_Sequence, SIGNAL(triggered()), this, SLOT(onSequenceEditTriggered()));
    connect(ui.actionAnalyze_Data, SIGNAL(triggered()), this, SLOT(onAnalyzeDataTriggered()));
    connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(onSaveTriggered()));
    connect(ui.actionRun, SIGNAL(triggered()), this, SLOT(onRunTriggered()));
}

void OpenInstrument::onTaskEditTriggered() {
    TaskEditor taskEditorDialog(m_taskListPtr);
    taskEditorDialog.exec();
}

void OpenInstrument::onSequenceEditTriggered() {
    SequenceEditor seqEditorDialog(m_seqListPtr, m_taskListPtr, m_curSeqID);
    seqEditorDialog.exec();
}

void OpenInstrument::onSaveTriggered() {
    if (!FileMgr::saveTasksToFile(*m_taskListPtr, m_taskFileName)) {
        QMessageBox::warning(nullptr, "No save", "Could not save to file task.ini");
        return;
    }

    if (!FileMgr::saveSeqListToFile(*m_seqListPtr, m_seqFileName)) {
        QMessageBox::warning(nullptr, "No save", "Could not save to file seq.ini");
        return;
    }
}

void OpenInstrument::onAnalyzeDataTriggered() {
    if (!FileMgr::readDataFile("./xy_data.txt", m_curData))
        return;
    addNewCurve(m_curData, "Raw", Qt::black);
    pair<vector<double>, vector<double>> filterData = DataProc::movemean(m_curData, 20);
    addNewCurve(filterData, "FIltered", QColor(255, 50, 50), 2);
    /*
    vector<int> peakInds = DataProc::findMaxPeaks(filterData.second, 70);
    for (int i = 0; i < peakInds.size(); i++) {
        vector<double> xx = { filterData.first[peakInds[i]], filterData.first[peakInds[i]] };
        vector<double> yy = { -0.4, 10.6 };
        addNewCurve(make_pair(xx, yy),
            "Peak " + QString::number(i + 1), QColor(255, 50, 50), 2, Qt::DashDotLine);
    }

        vector<vector<int>> ranges =
            DataProc::findRangesOfPeaks(filterData.second, peakInds);
        for (auto& curRange : ranges) {
            for (auto& ind : curRange) {
                vector<double> xx = { filterData.first[ind], filterData.first[ind] };
                vector<double> yy = { -0.4, 10.6 };
                addNewCurve(make_pair(xx, yy),
                    "", QColor(255, 10, 10), 2, Qt::DashDotLine);
            }
        }
     */
    vector<double> cleanData;
    DataProc::removeBaseline(m_curData.second, cleanData);
    vector<double> intersectionLine(cleanData.size(), 0);
    for (int i = 0; i < intersectionLine.size(); i++) {
        intersectionLine[i] = filterData.second[i] - cleanData[i];
    }
    addNewCurve(make_pair(m_curData.first, intersectionLine),
        "Real baseline", QColor(128, 0, 128), 2, Qt::DashLine);
    addNewCurve(make_pair(m_curData.first, cleanData),
        "Baseline removed", QColor(50, 50, 250), 2, Qt::SolidLine);
}

void OpenInstrument::onRunTriggered() {
    RunSeqDialog dialog(m_taskListPtr, (*m_seqListPtr)[m_curSeqID]);
    dialog.start();
    int result = dialog.exec();
}

void OpenInstrument::addNewCurve(const pair<vector<double>, vector<double>>& data,
    const QString& legendStr, const QColor& col, int penWidth, Qt::PenStyle style) {
    shared_ptr<QLineSeries> curve = make_shared<QLineSeries>();
    for (int i = 0; i < data.first.size(); i++) {
        curve->append(data.first[i], data.second[i]);
    }

    QPen pen;
    pen.setWidth(penWidth);
    pen.setColor(col);
    pen.setStyle(style);
    curve->setPen(pen);
    m_curvePtrs.push_back(curve);
    chart->addSeries(curve.get());
    axisY->setRange(0, 11);
    curve->attachAxis(axisX);
    curve->attachAxis(axisY);
    if(!legendStr.isEmpty())
        curve->setName(legendStr);
}
