#pragma once

#include <QThread>

class qSigGenThread : public QThread
{
	Q_OBJECT

public:
	qSigGenThread(QObject *parent);
	~qSigGenThread();
	void selectARB(QString arbName)
	{
		;
	}
signals:
	void ARBSelected();
	void threadStarted();
private:
	void run();// override;
};
