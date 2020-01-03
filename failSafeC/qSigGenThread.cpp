#include "qSigGenThread.h"

qSigGenThread::qSigGenThread(QObject *parent)
	: QThread(parent)
{
}

qSigGenThread::~qSigGenThread()
{
}

void qSigGenThread::run()
{
	emit ARBSelected();
	emit threadStarted();
}
