#pragma once

#include <QThread>
#include <NIDAQmx.h>
#include "qAICircularBuff.h"

class qContAcqAITrigThread : public QThread
{
	Q_OBJECT

public:
	qContAcqAITrigThread(double vTrigThreshold, qAICircularBuff *pAIBuff, QObject *parent = Q_NULLPTR);
	~qContAcqAITrigThread();
	double *getDataPtr()
	{
		return data;
	}
	void setRecordMaxLen(int totalRead)
	{
		m_totalRead = totalRead;
	}

	int getRecordMaxLen()
	{
		return m_totalRead;
	}

	void notifyAllDataAcquired()
	{
		emit allDataAcquired();
	}

	void notifyErrorMsg(const QString &msg)
	{
		emit errorMsg(msg);
	}

	void notifyDAQStarted()
	{
		emit acqStarted();
	}

	void pushToBuff(double *data, int len)
	{
		m_pAIBuff->pushData(data, len);
	}

	void cleanDAQTask();
private:
	void run() override;
	TaskHandle  taskHandle;
	double m_vTrigThreshold;
	double *data;
	int m_totalRead = 50000;
	qAICircularBuff *m_pAIBuff;
signals:
	void errorMsg(const QString &msg);
	void response(const QString &msg);
	void threadStarted();
	void allDataAcquired();
	void acqStarted();
};
