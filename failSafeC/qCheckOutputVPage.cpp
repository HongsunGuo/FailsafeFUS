#include "qFailSafeWizard.h"
#include "qCheckOutputVPage.h"
#include "failSafeGlobal.h"
#include <QMessageBox>
#include <QDateTime>

extern const QString FIELD_CHECK_V = "checkV";
#define ISPTA_THRES 0.03
#define V_THRES_HIGH 0.09
#define V_THRES_LOW 0.04 //for checkV

qCheckOutputVPage::qCheckOutputVPage(QWidget *parent)
	: QWizardPage(parent)
{
	ui.setupUi(this);
	p_VolLineEdit = new QLineEdit(this);
	p_VolLineEdit->setVisible(false);
	registerField(FIELD_CHECK_V, p_VolLineEdit);
	checkVBuff.init(PLOT_BUF_SIZE);
	//
	setTitle("Check output voltage of the amplifer");
	setSubTitle(" ");
	ui.listWidget->addItem("Date\tTime\tAmplitude (V)");
	//ui.listWidget->addItem("2018-08-15\t120 V");
	//
	p_leds = new qNewLEDs;
	ui.verticalLayout->addWidget(p_leds);
	p_leds->turnAll(false);
	//
	//Init threads
	int blockSize = 5000;
	p_AIBuff = new qAICircularBuff(400000, blockSize);
	p_aiProducerThread = new qAIProducerThread(this, p_AIBuff);
	p_aiConsumerThread = new qAIConsumerThread(this, p_AIBuff);
	p_doWriteThread = new qDOWriteThread(this);

	connect(p_aiConsumerThread, SIGNAL(abnormVoltage(double)), p_doWriteThread, SLOT(breakRelay()), Qt::DirectConnection);
	connect(p_aiConsumerThread, SIGNAL(abnormIspta(double)), p_doWriteThread, SLOT(breakRelay()), Qt::DirectConnection);
	connect(p_aiConsumerThread, SIGNAL(allDataAcquired()), this, SLOT(acqDone()));
	//
	connect(p_aiProducerThread, SIGNAL(threadStarted()), this, SLOT(onAllThreadStarted()));
	connect(p_aiConsumerThread, SIGNAL(threadStarted()), this, SLOT(onAllThreadStarted()));
	connect(p_doWriteThread, SIGNAL(threadStarted()), this, SLOT(onAllThreadStarted()));

	//p_aiConsumerThread->setRecordMaxLen(50000);
	//connect(this, SIGNAL(parentCompleteChanged()), this, SLOT(completeChanged()));
	//connect(ui.startBtn, SIGNAL(clicked()), this, SLOT(onStartBtnClicked()));
	//connect(ui.stopBtn, SIGNAL(clicked()), this, SLOT(onStopBtnClicked()));
	//
	connect(p_leds->p_isptaStartBtn, SIGNAL(clicked()), this, SLOT(onStartBtnClicked()));
	connect(p_leds->p_isppaStartBtn, SIGNAL(clicked()), this, SLOT(onStartBtnClicked()));
	connect(p_leds->p_checkVStartBtn, SIGNAL(clicked()), this, SLOT(onStartBtnClicked()));
	connect(p_leds->p_resetBtn, SIGNAL(clicked()), this, SLOT(onStopBtnClicked()));

	connect(ui.listWidget->model(), SIGNAL(rowsInserted(QModelIndex, int, int)),
		ui.listWidget, SLOT(scrollToBottom()));
}

qCheckOutputVPage::~qCheckOutputVPage()
{
}

bool qCheckOutputVPage::validatePage()
{
	//return true; //for test

	if (p_leds->getLEDStatus(qNewLEDs::LED_ISPTA_DONE) &&
		p_leds->getLEDStatus(qNewLEDs::LED_ISPPA_DONE) &&
		p_leds->getLEDStatus(qNewLEDs::LED_CHECK_V_DONE))
		return true;
	else
		return false;
}

void qCheckOutputVPage::updateVRecords(const QVector<qRecord>& records)
{
	for (const qRecord &record : records) {
		ui.listWidget->addItem(record.date + "\t" + record.time + "\t" + QString::number(record.checkV));
	}
}

void qCheckOutputVPage::onStartBtnClicked()
{
	if (!isDAQResAvailable())
	{
		QMessageBox::warning(this, "Warning",
			"Please wait until the current task is completed!");
		return;
	}

	btn = (QPushButton*) sender();
	if ((btn != p_leds->p_isptaStartBtn) &&
		(btn != p_leds->p_isppaStartBtn) &&
		(btn != p_leds->p_checkVStartBtn))
		return;

	p_aiConsumerThread->setPlotBuff(Q_NULLPTR, Q_NULLPTR, Q_NULLPTR);
	p_aiConsumerThread->breakRealy(true);
	p_aiConsumerThread->setISPTAThreshold(ISPTA_THRES);
	p_aiConsumerThread->setVolThreshold(V_THRES_HIGH);
	if (btn == p_leds->p_isptaStartBtn)
	{
		p_leds->turn(qNewLEDs::LED_ISPTA_READY, false);
		p_leds->turn(qNewLEDs::LED_ISPTA_DONE, false);
	}
	else if (btn == p_leds->p_isppaStartBtn)
	{
		p_leds->turn(qNewLEDs::LED_ISPPA_READY, false);
		p_leds->turn(qNewLEDs::LED_ISPPA_DONE, false);
	}
	else if (btn == p_leds->p_checkVStartBtn)
	{
		p_aiConsumerThread->setPlotBuff(&checkVBuff, Q_NULLPTR, Q_NULLPTR);
		p_aiConsumerThread->breakRealy(false); //for check V, don't break the realy
		p_aiConsumerThread->setISPTAThreshold(ISPTA_THRES);
		p_aiConsumerThread->setVolThreshold(V_THRES_LOW);
		p_leds->turn(qNewLEDs::LED_CHECK_V_READY, false);
		p_leds->turn(qNewLEDs::LED_CHECK_V_DONE, false);
	}
	else
		return;

	nThreads = 0;
	p_AIBuff->reset();
	checkVBuff.reset();
	startMonitoring();
}

bool qCheckOutputVPage::isDAQResAvailable()
{
	if (p_leds->p_isptaStartBtn->isEnabled() &&
		p_leds->p_isppaStartBtn->isEnabled() &&
		p_leds->p_checkVStartBtn->isEnabled())
		return true;
	else
		return false;
}

void qCheckOutputVPage::onStopBtnClicked()
{
	stopMonitoring();
	nThreads = 0;
	//m_curStatus = IDLE;
	p_leds->turnAll(false);
	p_leds->p_isptaStartBtn->setEnabled(true);
	p_leds->p_isppaStartBtn->setEnabled(true);
	p_leds->p_checkVStartBtn->setEnabled(true);
}

void qCheckOutputVPage::onAllThreadStarted()
{
	if (++nThreads == 3)
	{
		nThreads = 0;
		p_doWriteThread->resetRelay();
		if (btn == p_leds->p_isptaStartBtn)
		{
			p_leds->turn(qNewLEDs::LED_ISPTA_READY, true);
			p_leds->p_isptaStartBtn->setEnabled(false);
		}
		else if (btn == p_leds->p_isppaStartBtn)
		{
			p_leds->turn(qNewLEDs::LED_ISPPA_READY, true);
			p_leds->p_isppaStartBtn->setEnabled(false);
		}
		else if (btn == p_leds->p_checkVStartBtn)
		{
			p_leds->turn(qNewLEDs::LED_CHECK_V_READY, true);
			p_leds->p_checkVStartBtn->setEnabled(false);
		}
		else
		{
			return;
		}
	}
}

//TODO need to add timeout mechanism
void qCheckOutputVPage::acqDone()
{
	stopMonitoring();
	if (!p_leds->p_isptaStartBtn->isEnabled())
	{
		p_leds->turn(qNewLEDs::LED_ISPTA_DONE, true);
		p_leds->p_isptaStartBtn->setEnabled(true);
	}
	else if (!p_leds->p_isppaStartBtn->isEnabled())
	{
		p_leds->turn(qNewLEDs::LED_ISPPA_DONE, true);
		p_leds->p_isppaStartBtn->setEnabled(true);
	}
	else if (!p_leds->p_checkVStartBtn->isEnabled())
	{
		p_leds->turn(qNewLEDs::LED_CHECK_V_DONE, true);
		p_leds->p_checkVStartBtn->setEnabled(true);
		QString v = QString::number(checkVBuff.getAbsMax());
		p_VolLineEdit->setText(v);
		QDateTime dateTime = field(FIELD_DATE_TIME).toDateTime();
		ui.listWidget->addItem(dateTime.date().toString("yyyy-MM-dd") + "\t" +
			dateTime.time().toString("HH:mm:ss") + "\t" + v);
	}
	else
	{
		return;
	}
}

void qCheckOutputVPage::stopMonitoring()
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

void qCheckOutputVPage::startMonitoring()
{
	p_aiProducerThread->start();
	p_aiConsumerThread->start();
	p_doWriteThread->start();
}


//1) Before using the transducer on human subjects, please test the transducer in a  beake filled with water.
//2) Measure the output voltage delivered to the transducer.
//3) The output voltage  should be approximately the same as that measured last time. Otherwise, please check if correct ARB file is used and the BNC connections are good.
//Historical output voltage of the amplifier (date  and amplitude)
