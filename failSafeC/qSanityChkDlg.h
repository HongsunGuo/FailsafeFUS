#pragma once

#include <QDialog>
#include <QMutex>
#include <QTimer>

#include "ui_qSanityChkDlg.h"
#include "qLEDs.h"
#include "qAIConsumerThread.h"
#include "qAIProducerThread.h"
#include "qSigGenThread.h"
#include "qAICircularBuff.h"
#include "qDOWriteThread.h"
#include "failSafeGlobal.h"
#include "qContAcqAITrigThread.h"

class qSanityChkDlg : public QDialog
{
	Q_OBJECT

public:
	enum testID { BIGSPIKE, LARGEVOLTAGE};
	qSanityChkDlg(testID id, QWidget *parent = Q_NULLPTR);
	~qSanityChkDlg();

public slots:
	virtual void accept();
	virtual void reject();
	void onRelayOpened(bool isOpen);
	void onTestBtnClicked();
private:
	Ui::qSanityChkDlg ui;
	qLEDs *p_leds;
	int m_threadCount = 0;
	//threads
	qDOWriteThread *p_doWriteThread;
	qSigGenThread *p_sigGenThread;
	qAICircularBuff *p_AIBuff;
	qContAcqAITrigThread *p_ContAcqOnTrigThread;
	tOScopeData m_OScopeData;
	//
	void startAllThreads();
	void stopAllThreads();
	QMutex m_mutex;
	QTimer m_timer;
private slots:
	void handleErrMsg(const QString &errMsg);
	void handleInfoMsg(const QString &errMsg);
	void triggerNow();
	void onAllThreadsStarted();
	void timerTimeout();
	//
	void presentData();
};
