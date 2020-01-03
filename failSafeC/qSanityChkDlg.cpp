#include <QMessageBox>
#include "qSanityChkDlg.h"

qSanityChkDlg::qSanityChkDlg(testID id, QWidget *parent)
	: QDialog(parent)
{
	m_timer.setSingleShot(true);
	m_timer.setInterval(30000); //10 sec
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
	//
	ui.setupUi(this);
	p_leds = new qLEDs(this);
	ui.verticalLayout_2->addWidget(p_leds);
	p_leds->turnAll(false);
	//
	m_OScopeData.xData = new double[OSCOPE_BUFF];
	m_OScopeData.yData = new double[OSCOPE_BUFF];
	m_OScopeData.size = 0;
	//Init threads
	p_AIBuff = new qAICircularBuff(80000, 10000);
	p_doWriteThread = new qDOWriteThread(this);
	p_sigGenThread = new qSigGenThread(this);
	p_ContAcqOnTrigThread = new qContAcqAITrigThread(0.5, p_AIBuff);

	connect(ui.testBtn, SIGNAL(clicked()), this, SLOT(onTestBtnClicked()));
	connect(ui.listWidget->model(), SIGNAL(rowsInserted(QModelIndex, int, int)),
		ui.listWidget, SLOT(scrollToBottom()));
	//
	connect(p_doWriteThread, SIGNAL(relayOpen(bool)), this, SLOT(onRelayOpened(bool)));
	//
	connect(p_doWriteThread, SIGNAL(threadStarted()), this, SLOT(onAllThreadsStarted()));
	connect(p_sigGenThread, SIGNAL(threadStarted()), this, SLOT(onAllThreadsStarted()));
	connect(p_ContAcqOnTrigThread, SIGNAL(threadStarted()), this, SLOT(onAllThreadsStarted()));
	//
	connect(p_ContAcqOnTrigThread, SIGNAL(allDataAcquired()), this, SLOT(presentData()));
	connect(p_ContAcqOnTrigThread, SIGNAL(errorMsg(const QString &)), this, SLOT(handleErrMsg(const QString &)));
	connect(p_ContAcqOnTrigThread, SIGNAL(response(const QString &)), this, SLOT(handleInfoMsg(const QString &)));
	connect(p_ContAcqOnTrigThread, SIGNAL(acqStarted()), p_doWriteThread, SLOT(breakRelay()), Qt::DirectConnection);
}

qSanityChkDlg::~qSanityChkDlg()
{
	stopAllThreads();
	delete[] m_OScopeData.xData;
	delete[] m_OScopeData.yData;
}

void qSanityChkDlg::accept()
{
	//
	QDialog::accept();
}

void qSanityChkDlg::reject()
{
	//
	QDialog::reject();
}

void qSanityChkDlg::onRelayOpened(bool isOpen)
{
	if (isOpen)
	{
		p_leds->turn(qLEDs::BREAK, true);
	}
}

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else
void qSanityChkDlg::onTestBtnClicked()
{
	ui.listWidget->clear();
	m_timer.start();
	startAllThreads();
}

void qSanityChkDlg::timerTimeout()
{
	stopAllThreads();
	QMessageBox::warning(this, "warning", "Time out! Please try again!");
}

void qSanityChkDlg::presentData()
{
	p_doWriteThread->resetRelay();
	m_timer.stop();
	stopAllThreads();
	p_AIBuff->saveDataToCSV(".\\waveform_data.csv");
	p_leds->turn(qLEDs::RESET, true);
}

void qSanityChkDlg::startAllThreads()
{
	p_doWriteThread->start();
	p_sigGenThread->start();
	p_ContAcqOnTrigThread->start(QThread::TimeCriticalPriority);
}

void qSanityChkDlg::onAllThreadsStarted()
{
	m_mutex.lock();
	m_threadCount++;
	m_mutex.unlock();
	if (m_threadCount == 3)
	{
		p_leds->turn(qLEDs::INIT, true);
		m_threadCount = 0;
		triggerNow();
	}
}

void qSanityChkDlg::stopAllThreads()
{	
	if (p_doWriteThread->isRunning())
	{
		p_doWriteThread->quitNow();
		p_doWriteThread->wait();
	}

	if (p_ContAcqOnTrigThread->isRunning())
	{
		p_ContAcqOnTrigThread->quit();
		p_ContAcqOnTrigThread->wait();
	}
}

void qSanityChkDlg::handleInfoMsg(const QString & infoMsg)
{
	if (!infoMsg.isEmpty())
	{
		ui.listWidget->addItem(infoMsg.trimmed());
		ui.listWidget->item(ui.listWidget->count() - 1)->setForeground(Qt::black);
	}
}

void qSanityChkDlg::handleErrMsg(const QString & errMsg)
{
	if (!errMsg.isEmpty())
	{
		ui.listWidget->addItem(errMsg.trimmed());
		ui.listWidget->item(ui.listWidget->count() - 1)->setForeground(Qt::red);
	}
}

void qSanityChkDlg::triggerNow()
{
	//Use trigger on the DAQ to trigger the function generator
	//and the oscilloscope
	char        errBuff[2048] = { '\0' };
	TaskHandle  taskHandle = 0;
	int32       error = 0;

	/*********************************************/
	// DAQmx Configure Code
	/*********************************************/
	DAQmxErrChk(DAQmxCreateTask("", &taskHandle));
	DAQmxErrChk(DAQmxCreateCOPulseChanTime(taskHandle, "Dev1/ctr0", "", DAQmx_Val_Seconds, DAQmx_Val_Low, 0.1, 0.1, 0.05));

	/*********************************************/
	// DAQmx Start Code
	/*********************************************/
	DAQmxErrChk(DAQmxStartTask(taskHandle));

	/*********************************************/
	// DAQmx Wait Code
	/*********************************************/
	DAQmxErrChk(DAQmxWaitUntilTaskDone(taskHandle, 10.0));
	p_leds->turn(qLEDs::GEN, true);

Error:
	if (DAQmxFailed(error))
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
	if (taskHandle != 0) {
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}
	if (DAQmxFailed(error))
	{
		QMessageBox::warning(this, "warning", QString("DAQmx Error: ") + QString(errBuff));
	}
}
