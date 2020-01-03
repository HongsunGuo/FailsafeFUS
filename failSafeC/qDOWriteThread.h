#pragma once

#include <QObject>
#include <NIDAQmx.h>

//Don't inherit from thread to see if it can close the realy faster
class qDOWriteThread : public QObject //QThread
{
	Q_OBJECT

public:
	qDOWriteThread(QObject *parent = nullptr);
	~qDOWriteThread();
	void writeDO(unsigned char value);

	void start();
	bool isRunning() {return m_running;}
	void setQuit(bool b) { quitNow(); }
	void quitNow();
	void wait() {}
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
	bool m_running = false;
	unsigned char m_DOValue;
	int32       error = 0;
	TaskHandle  taskHandle = 0;
	char        errBuff[2048] = { '\0' };
};
