#include "qFailSafeWin.h"
#include "qV2IntensityDlg.h"
#include <QDate>
#include <QTime>
#include <QInputDialog>
#include <QMessageBox>

qFailSafeWin::qFailSafeWin(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	p_logListWidget = new QListWidget(this);
	//
	//Init all the buffs for plotting
	m_AIPlotBuff.init(PLOT_BUF_SIZE);
	m_isppaPlotBuff.init(PLOT_BUF_SIZE);
	m_isptaPlotBuff.init(PLOT_BUF_SIZE);

	//initPlotBuff(&m_AIPlotBuff, PLOT_BUF_SIZE);
	//initPlotBuff(&m_isppaPlotBuff, PLOT_BUF_SIZE);
	//initPlotBuff(&m_isptaPlotBuff, PLOT_BUF_SIZE);

	//
	m_OScopeData.xData = new double[OSCOPE_BUFF];
	m_OScopeData.yData = new double[OSCOPE_BUFF];
	m_OScopeData.size = 0;
	//
	p_gridLayout = new QGridLayout(this);
	p_AIPlot = new qUSIntensityPlot(this, "AI0 voltage (V), "
		"multiply by 27.7 to get amplifier output voltage.",
		&m_AIPlotBuff, QwtPlot::yLeft);
	p_isppaPlot = new qUSIntensityPlot(this, "AI0 peak voltage (V)",
		&m_isppaPlotBuff, QwtPlot::yLeft);
	p_isptaPlot = new qUSIntensityPlot(this, "AI0 averaged voltage (V)",
		&m_isptaPlotBuff, QwtPlot::yRight);

	p_AIPlot->setAxisTitle(QwtPlot::xBottom, "T (ms)");
	p_AIPlot->setAxisTitle(QwtPlot::yLeft, "Voltage (V)");
	//
	p_isppaPlot->setAxisTitle(QwtPlot::xBottom, "T (ms)");
	p_isppaPlot->setAxisTitle(QwtPlot::yLeft, "Voltage (V)");
	//
	p_isptaPlot->setAxisTitle(QwtPlot::xBottom, "T (ms)");
	p_isptaPlot->setAxisTitle(QwtPlot::yRight, "Voltage (V)");

	//
	p_AIPlot->setYAxisMax(QwtPlot::yLeft, 2);
	p_isppaPlot->setYAxisMax(QwtPlot::yLeft, 2);
	p_isptaPlot->setYAxisMax(QwtPlot::yRight, 2);
	//
	//p_AIPlot->setYLimitLine(4.5);
	//p_isppaPlot->setYLimitLine(648);
	//p_isptaPlot->setYLimitLine(6.48);
	//
	p_gridLayout->addWidget(p_isppaPlot, 1, 1);
	p_gridLayout->addWidget(p_isptaPlot, 1, 2);
	p_gridLayout->addWidget(p_AIPlot, 2, 1);
	p_gridLayout->addWidget(p_logListWidget, 2, 2);
	//
	QLayout *curretLayout = ui.centralWidget->layout();
	ui.centralWidget->setLayout(p_gridLayout);
	setWindowState(Qt::WindowMaximized);
	ui.actionStart->setIcon(QIcon(":/failSafeC/Resources/start.png"));
	ui.actionStop->setIcon(QIcon(":/failSafeC/Resources/stop.png"));
	//
	startAction = ui.mainToolBar->addAction(QIcon(":/failSafeC/Resources/start.png"), "Start");
	stopAction = ui.mainToolBar->addAction(QIcon(":/failSafeC/Resources/stop.png"), "Stop");
	ui.mainToolBar->addSeparator();
	//calibrationAction = ui.mainToolBar->addAction(QIcon(":/failSafeC/Resources/calibration.png"), "Calibration curve");
	//isiAction = ui.mainToolBar->addAction(QIcon(":/failSafeC/Resources/ISI.png"), "ISI");
	vThresAction = ui.mainToolBar->addAction(QIcon(":/failSafeC/Resources/Vthres.png"), "Peak voltage threshold");
	isptaThresAction = ui.mainToolBar->addAction(QIcon(":/failSafeC/Resources/isptaThres.png"), "Averaged voltage threshold");

	//
	//Init threads
	int blockSize = 5000;
	p_AIBuff = new qAICircularBuff(400000, blockSize);
	p_aiProducerThread = new qAIProducerThread(this, p_AIBuff);
	p_aiConsumerThread = new qAIConsumerThread(this, p_AIBuff);
	p_doWriteThread = new qDOWriteThread(this);
	//
	p_aiConsumerThread->setPlotBuff(&m_AIPlotBuff, &m_isppaPlotBuff, &m_isptaPlotBuff);
	//
	connect(startAction, SIGNAL(triggered()), this, SLOT(onStartMonitoring()));
	connect(stopAction, SIGNAL(triggered()), this, SLOT(onStopActionClicked()));
	//connect(calibrationAction, SIGNAL(triggered()), this, SLOT(onCalibrActionClicked()));
	//connect(isiAction, SIGNAL(triggered()), this, SLOT(onISIActionClicked()));
	connect(vThresAction, SIGNAL(triggered()), this, SLOT(onVThresActionClicked()));
	connect(isptaThresAction, SIGNAL(triggered()), this, SLOT(onIsptaThresActionClicked()));

	//
	connect(p_aiConsumerThread, SIGNAL(abnormVoltage(double)), p_doWriteThread, SLOT(breakRelay()), Qt::DirectConnection);
	connect(p_aiConsumerThread, SIGNAL(abnormIspta(double)), p_doWriteThread, SLOT(breakRelay()), Qt::DirectConnection);
	//
	connect(p_aiConsumerThread, SIGNAL(abnormVoltage(double)), this, SLOT(logAbnormalVoltage(double)));
	connect(p_aiConsumerThread, SIGNAL(abnormIspta(double)), this, SLOT(logAbnormalIspta(double)));
	//
	connect(p_aiConsumerThread, SIGNAL(allDataAcquired()), this, SLOT(updatePlots()));
	//
	connect(p_aiProducerThread, SIGNAL(threadStarted()), this, SLOT(onAllThreadStarted()));
	connect(p_aiConsumerThread, SIGNAL(threadStarted()), this, SLOT(onAllThreadStarted()));
	connect(p_doWriteThread, SIGNAL(threadStarted()), this, SLOT(onAllThreadStarted()));
	//
	startAction->setEnabled(true);
	stopAction->setEnabled(false);
	//show some log
	//addLog("Trial duration set to " + QString::number(p_aiConsumerThread->getISI())+ " sec");
	addLog("Peak voltage threshold set to " + QString::number(p_aiConsumerThread->getVolThreshold()) + " V");
	addLog("Averaged voltage threshold set to " + QString::number(p_aiConsumerThread->getISPTAThreshold()) + "V");
}

qFailSafeWin::~qFailSafeWin()
{
}

void qFailSafeWin::onStopActionClicked()
{
	onStopMonitoring();
	addLog("Stop monitoring");
}

void qFailSafeWin::logAbnormalVoltage(double v)
{
	m_abnormalV = true;
	addLog("Abnormal peak voltage detected! V = " + QString::number(v)+ " V");
}

void qFailSafeWin::logAbnormalIspta(double ispta)
{
	m_abnormalIspta = true;
	addLog("Abnormal averaged voltage detected! V = " + QString::number(ispta) + " V");
}

void qFailSafeWin::onStartMonitoring()
{
	m_AIPlotBuff.reset();
	m_isptaPlotBuff.reset();
	m_isppaPlotBuff.reset();
	//
	p_AIBuff->reset();
	//
	p_isppaPlot->eraseAllPlots();
	p_isptaPlot->eraseAllPlots();
	p_AIPlot->eraseAllPlots();
	//
	startDAQThreads();
}

void qFailSafeWin::onStopMonitoring()
{
	stopDAQThreads();
	startAction->setEnabled(true);
	stopAction->setEnabled(false);
}

void qFailSafeWin::updatePlots()
{
	p_isppaPlot->replotCurve(m_isppaPlotBuff.wInd);
	p_isptaPlot->replotCurve(m_isptaPlotBuff.wInd);
	p_AIPlot->replotCurve(m_AIPlotBuff.wInd);
	//stop all threads
	QString filePrefix = QDate::currentDate().toString("yyyyMMdd_") +
		QTime::currentTime().toString("HH-mm-ss_zzz_");
	QString AIFile = m_dir.absoluteFilePath(filePrefix + "abnorm_AI_fragment.csv");
	QString ISPTAFile = m_dir.absoluteFilePath(filePrefix + "abnorm_ISPTA_fragment.csv");
	QString ISPPAFile = m_dir.absoluteFilePath(filePrefix + "abnorm_ISPPA_fragment.csv");
	saveData(AIFile.toLocal8Bit().data(), &m_AIPlotBuff);
	saveData(ISPTAFile.toLocal8Bit().data(), &m_isptaPlotBuff);
	saveData(ISPPAFile.toLocal8Bit().data(), &m_isppaPlotBuff);
	addLog("Abnormal data saved");
	//
	onStopMonitoring();
	addLog("Stop monitoring");
}

void qFailSafeWin::onAllThreadStarted()
{
	static int nThreads = 0;
	if (++nThreads == 3)
	{
		//ensure the realy is closed
		p_doWriteThread->resetRelay();
		nThreads = 0;
		startAction->setEnabled(false);
		stopAction->setEnabled(true);
		addLog("Start monitoring");
	}
}

void qFailSafeWin::onCalibrActionClicked()
{
	if (!checkPassword())
		return;

	qV2IntensityDlg voltage2IntensityDlg;
	voltage2IntensityDlg.exec();
}

void qFailSafeWin::onISIActionClicked()
{
	if (!checkPassword())
		return;

	bool ok;
	double isi = QInputDialog::getDouble(this, "Please input (0-1000):",
		"Trial duration (s)", p_aiConsumerThread->getISI(), 0.0001, 1000, 3, &ok); //maxium is 1000 sec, min is 0.0001 to make sure it is not 0.
	if (!ok)
		return;
	//
	onStopMonitoring();
	p_aiConsumerThread->setISI(isi);
	addLog("Trial duration set to " + QString::number(isi) + " sec");
}

void qFailSafeWin::onVThresActionClicked()
{
	if (!checkPassword())
		return;

	bool ok;
	double vThreshold = QInputDialog::getDouble(this, "Please input (0-1000):",
		"Peak voltage threshold (V)", p_aiConsumerThread->getVolThreshold(), 0.00001, 1000, 3, &ok);
	if (!ok)
		return;

	onStopMonitoring();
	p_aiConsumerThread->setVolThreshold(vThreshold);
	addLog("Peak voltage threshold set to " + QString::number(vThreshold) + " V");
}

void qFailSafeWin::onIsptaThresActionClicked()
{
	if (!checkPassword())
		return;

	bool ok;
	double ISPTAThreshold = QInputDialog::getDouble(this, "Please input (0-1000):",
		"Averaged voltage threshold threshold (V)",
		p_aiConsumerThread->getISPTAThreshold(), 0.00001, 1000, 3, &ok);
	if (!ok)
		return;

	onStopMonitoring();
	p_aiConsumerThread->setISPTAThreshold(ISPTAThreshold);
	addLog("Averaged voltage threshold set to " + QString::number(ISPTAThreshold) + " V");
}

void qFailSafeWin::saveData(const char * fileName, qPlotBuff * plotBuff)
{
	FILE *fp;
	/* Open file for output. */
	fp = fopen(fileName, "wb");
	/* Output waveform data in CSV format. */
	for (int i = 0; i < plotBuff->wInd; i++)
		fprintf(fp, "%6f,%6f\n", plotBuff->x[i], plotBuff->y[i]);
	/* Close output file. */
	fclose(fp);
}

void qFailSafeWin::addLog(QString newItem)
{
	QString prefix = QDate::currentDate().toString("yyyyMMdd_") +
		QTime::currentTime().toString("HH:mm:ss.zzz")+"\t";
	p_logListWidget->addItem(prefix+newItem);
	p_logListWidget->scrollToBottom();
}

bool qFailSafeWin::checkPassword()
{
	if (PASSWORD == QInputDialog::getText(this, "Enter password:",
		"Password", QLineEdit::Password))
		return true;
	else
	{
		QMessageBox::warning(this, "Warning",
			"Wrong password!");
		return false;
	}
}

void qFailSafeWin::startDAQThreads()
{
	//
	p_aiConsumerThread->setQuit(false);
	p_doWriteThread->setQuit(false);
	//
	p_aiProducerThread->start();
	p_aiConsumerThread->start(QThread::TimeCriticalPriority);
	p_doWriteThread->start();
}

void qFailSafeWin::stopDAQThreads()
{
	if (p_aiProducerThread->isRunning())
	{
		p_aiProducerThread->quit();
		p_aiProducerThread->wait();
	}

	if (p_aiConsumerThread->isRunning())
	{
		p_aiConsumerThread->setQuit(true);
		p_aiConsumerThread->wait();
	}

	if (p_doWriteThread->isRunning())
	{
		p_doWriteThread->setQuit(true);
		p_doWriteThread->wait();
	}
}

void qFailSafeWin::closeEvent(QCloseEvent * event)
{
	stopDAQThreads();
	QMainWindow::closeEvent(event);
}
