#pragma once
#include <QThread>

class keysight33503A : public QThread
{
	Q_OBJECT
public:
	keysight33503A(QObject *parent = nullptr);
	typedef enum _mode {SMALLSINE = 0, BIGSINE, NORMSINE} mode;
	void send(mode m);
protected:
	void run() override;
signals:
	void errMsg(QString msg);
private:
	mode m_mode = SMALLSINE;
	void sendLowAmplitudeSine();
	void sendHighAmplitudeSine();
	void sendNormalAmplitudeSine();
};
