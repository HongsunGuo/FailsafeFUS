#include <NIDAQmx.h>
#include "qDOWriteThread.h"

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto DAQError; else

qDOWriteThread::qDOWriteThread(QObject *parent)
	: QThread(parent)
{
}

qDOWriteThread::~qDOWriteThread()
{
	m_mutex.lock();
	m_quit = true;
	m_cond.wakeOne();
	m_mutex.unlock();
	wait();
}

void qDOWriteThread::writeDO(unsigned char value)
{
	const QMutexLocker locker(&m_mutex);
	m_DOValue = value;
	//
	if (!isRunning())
		start();
	else
		m_cond.wakeOne();
}

void qDOWriteThread::run()
{
	int32       error = 0;
	TaskHandle  taskHandle = 0;
	char        errBuff[2048] = { '\0' };
	m_DOValue = 0;
	float64 timeout = 5; //in sec

	// DAQmx Configure Code
	DAQmxErrChk(DAQmxCreateTask("", &taskHandle));
	DAQmxErrChk(DAQmxCreateDOChan(taskHandle, "Dev1/port0/line0", "", DAQmx_Val_ChanForAllLines));
	// DAQmx Start Code
	DAQmxErrChk(DAQmxStartTask(taskHandle));
	DAQmxErrChk(DAQmxWriteDigitalLines(taskHandle, 1, 1, timeout, DAQmx_Val_GroupByChannel, &m_DOValue, NULL, NULL));

	emit threadStarted();
	while (!m_quit)
	{
		//
		m_mutex.lock();
		if (m_cond.wait(&m_mutex, 1000)) //release m_mutex and waiting for the wake of the condition...
		{
			// DAQmx Write Code
			DAQmxErrChk(DAQmxWriteDigitalLines(taskHandle, 1, 1, timeout, DAQmx_Val_GroupByChannel, &m_DOValue, NULL, NULL));
			emit relayOpen(m_DOValue > 0);
		}
		m_mutex.unlock();
	}

DAQError:
	if (DAQmxFailed(error))
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
	if (taskHandle != 0) {
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}
	if (DAQmxFailed(error))
		emit errorMsg(QString(errBuff));
}
