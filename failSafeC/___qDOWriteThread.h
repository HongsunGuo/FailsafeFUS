#pragma once

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class qDOWriteThread : public QThread
{
	Q_OBJECT

public:
	explicit qDOWriteThread(QObject *parent = nullptr);
	~qDOWriteThread();
	void writeDO(unsigned char value);
	void quitNow()
	{
		m_quit = true;
	}

	void setQuit(bool b)
	{
		m_quit = b;
	}

public slots:
	void breakRelay()
	{
		writeDO(1);
	}
	void resetRelay()
	{
		writeDO(0);
	}
signals:
	void threadStarted();
	void response(const QString &s);
	void errorMsg(const QString &s);
	void relayOpen(bool v);
private:
	void run() override;
	unsigned char m_DOValue;
	QMutex m_mutex;
	QWaitCondition m_cond;
	bool m_quit = false;
};
