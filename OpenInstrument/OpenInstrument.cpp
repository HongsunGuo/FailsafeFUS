#include <QtCharts>
#include "OpenInstrument.h"
#include "TaskEditor.h"
#include "SequenceEditor.h"
#include "FileMgr.h"

using namespace std;

OpenInstrument::OpenInstrument(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    chart = new QChart();
    //chart->setTitle("Sample Chart");
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
    chart->legend()->show();
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
}

void OpenInstrument::onTaskEditTriggered() {
    TaskEditor taskEditorDialog;
    taskEditorDialog.exec();
}

void OpenInstrument::onSequenceEditTriggered() {
    SequenceEditor seqEditorDialog;
    seqEditorDialog.exec();
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
}
