#pragma once

#include <QThread>
#include <NIDAQmx.h>
#include "qAICircularBuff.h"

class qAIProducerThread : public QThread //QThread
{
	Q_OBJECT

public:
	qAIProducerThread(QObject *parent, qAICircularBuff *pCircularBuff);
	~qAIProducerThread();
	qAICircularBuff *getCircularBuff()
	{
		return p_circularBuff;
	}
	float64 *getReadBuff()
	{
		return p_ReadBuff;
	}
	int32 getReadBuffSize()
	{
		return m_ReadBuffSize;
	}

	void notifyErrorMsg(const QString &msg)
	{
		emit errorMsg(msg);
	}

signals:
	void threadStarted();
	void DAQError();
	void errorMsg(const QString &msg);
	void response(const QString &msg);

private:
	float64 *p_ReadBuff; //To store data read from
	int32 m_ReadBuffSize;
	qAICircularBuff *p_circularBuff;
	void run();// override;
private slots:
	void clearDAQTask();
};
