#pragma once

#include <QWizardPage>
#include <QTimer>
#include <QLineEdit>
#include "ui_qCheckOutputVPage.h"
#include "qLEDs.h"
#include "qAIConsumerThread.h"
#include "qAIProducerThread.h"
#include "qAICircularBuff.h"
#include "qDOWriteThread.h"
#include "failSafeGlobal.h"
#include "qPlotBuff.h"

class qRecord;
class qCheckOutputVPage : public QWizardPage
{
	Q_OBJECT

public:
	qCheckOutputVPage(QWidget *parent = Q_NULLPTR);
	~qCheckOutputVPage();
	//bool isComplete() const override;
	bool validatePage() override;
	void updateVRecords(const QVector<qRecord>& records);
private:
	Ui::qCheckOutputVPage ui;
	QLineEdit *p_VolLineEdit;
	qNewLEDs *p_leds;
	int nThreads = 0;
	qPlotBuff checkVBuff;
	QPushButton* btn;
	//
	qAIProducerThread *p_aiProducerThread;
	qAIConsumerThread *p_aiConsumerThread;
	qDOWriteThread *p_doWriteThread;
	qAICircularBuff *p_AIBuff;
	void stopMonitoring();
	void startMonitoring();
	bool isDAQResAvailable();
signals:
	void parentCompleteChanged();
private slots:
	void onAllThreadStarted();
	void onStartBtnClicked();
	void onStopBtnClicked();
	void acqDone();
};
