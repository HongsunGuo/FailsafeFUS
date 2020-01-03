#include "qDOWriteThread.h"

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto DAQError; else

qDOWriteThread::qDOWriteThread(QObject *parent)
	: QObject(parent)
{
}

qDOWriteThread::~qDOWriteThread()
{
	if (taskHandle != 0) {
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}
}

void qDOWriteThread::writeDO(unsigned char value)
{
	float64 timeout = 2; //in sec
	m_DOValue = value;
	DAQmxErrChk(DAQmxWriteDigitalLines(taskHandle, 1, 1, timeout,
		DAQmx_Val_GroupByChannel, &m_DOValue, NULL, NULL));
	emit relayOpen(m_DOValue > 0);
DAQError:
	if (DAQmxFailed(error))
	{
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		emit errorMsg(QString(errBuff));
	}
}

void qDOWriteThread::start()
{
	m_DOValue = 0;
	float64 timeout = 5; //in sec

	// DAQmx Configure Code
	DAQmxErrChk(DAQmxCreateTask("taskDO", &taskHandle));
	DAQmxErrChk(DAQmxCreateDOChan(taskHandle, "Dev1/port0/line0", "", DAQmx_Val_ChanForAllLines));
	// DAQmx Start Code
	DAQmxErrChk(DAQmxStartTask(taskHandle));
	DAQmxErrChk(DAQmxWriteDigitalLines(taskHandle, 1, 1, timeout, DAQmx_Val_GroupByChannel, &m_DOValue, NULL, NULL));

	emit threadStarted();
	m_running = true;
	return;
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

void qDOWriteThread::quitNow()
{
	if (taskHandle != 0) {
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}
	m_running = false;
}