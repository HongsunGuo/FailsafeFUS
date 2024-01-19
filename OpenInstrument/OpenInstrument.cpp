#include <QtCharts>
#include "OpenInstrument.h"
#include "TaskEditor.h"
#include "SequenceEditor.h"
#include "FileMgr.h"
#include "Task.h"

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
    chart->setTitle("Sample Chart");
    axisX = new QValueAxis();
    axisY = new QValueAxis();

    axisX->setGridLineVisible(true);
    axisY->setGridLineVisible(true);
    QPen gridPen;
    gridPen.setColor(Qt::white);
    gridPen.setStyle(Qt::DashLine);
    axisX->setGridLinePen(gridPen);
    axisY->setGridLinePen(gridPen);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->setTheme(QChart::ChartThemeDark);
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
    QPen penRaw, penProcessed;
    penRaw.setWidth(1); // Set the width of the line
    penRaw.setColor(Qt::white); // Set the color of the line
    penProcessed.setWidth(1);
    penProcessed.setColor(Qt::yellow);
    //
    m_rawlinePtr = make_shared<QLineSeries>();
    m_processedlinePtr = make_shared<QLineSeries>();
    //
    m_rawlinePtr->setPen(penRaw);
    m_rawlinePtr->setName("Raw data");

    m_processedlinePtr->setPen(penProcessed);
    m_processedlinePtr->setName("Processed data");
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

    m_rawlinePtr->clear();
    for (auto& pair : m_curData) {
        m_rawlinePtr->append(pair.first, pair.second);
    }

    chart->addSeries(m_rawlinePtr.get());
    m_rawlinePtr->attachAxis(axisX);
    m_rawlinePtr->attachAxis(axisY);
    chart->setTitle("Sample xxxxx");

}
