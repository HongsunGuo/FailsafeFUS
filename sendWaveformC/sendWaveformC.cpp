#include "sendWaveformC.h"
#include <QMessageBox>

sendWaveformC::sendWaveformC(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.testIsptaBtn, SIGNAL(clicked()), this, SLOT(testIspta()));
	connect(ui.testIsppaBtn, SIGNAL(clicked()), this, SLOT(testIsppa()));
	connect(ui.checkVBtn, SIGNAL(clicked()), this, SLOT(checkVoltage()));
	//
	pFGThread = new keysight33503A(this);
	connect(pFGThread, SIGNAL(finished()), this, SLOT(currentThreadFinished()));
	connect(pFGThread, SIGNAL(errMsg(QString)), this, SLOT(showErrMsg(QString)));
	

}

void sendWaveformC::setAllButtons(bool on)
{
	ui.testIsptaBtn->setEnabled(on);
	ui.testIsppaBtn->setEnabled(on);
	ui.checkVBtn->setEnabled(on);
}

void sendWaveformC::testIspta()
{
	setAllButtons(false);
	pFGThread->send(keysight33503A::SMALLSINE);
}

void sendWaveformC::testIsppa()
{
	setAllButtons(false);
	pFGThread->send(keysight33503A::BIGSINE);
}

void sendWaveformC::checkVoltage()
{
	setAllButtons(false);
	pFGThread->send(keysight33503A::NORMSINE);
}

void sendWaveformC::currentThreadFinished()
{
	setAllButtons(true);
}

void sendWaveformC::done(int r)
{
	if (pFGThread->isRunning())
	{
		pFGThread->terminate();
		pFGThread->wait();
	}
	QDialog::done(r);
}

void sendWaveformC::showErrMsg(QString msg)
{
	QMessageBox::warning(Q_NULLPTR, "warning", msg);
}