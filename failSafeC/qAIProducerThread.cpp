#include<QDebug>
#include "failSafeGlobal.h"
#include "qAIProducerThread.h"

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else
static int32 CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);

qAIProducerThread::qAIProducerThread(QObject *parent, qAICircularBuff *pCircularBuff)
	: QThread(parent), p_circularBuff(pCircularBuff)
{
	m_ReadBuffSize =10000;
	p_ReadBuff = new double[m_ReadBuffSize];
	memset(p_ReadBuff, 0, sizeof(double)*m_ReadBuffSize);
	//connect(this, SIGNAL(finished()), this, SLOT(clearDAQTask()));
}

qAIProducerThread::~qAIProducerThread()
{
	delete[]p_ReadBuff;
}

void qAIProducerThread::run()
{
	int error = 0;
	char        errBuff[2048] = { '\0' };
	int32 DAQBuffSize = 1000000;
	int32 NSample =5000; //read may be slightly larger than readExpected. so expect less to avoid outflow.
	TaskHandle  taskHandle;

	//TaskHandle taskHandle = m_TaskHandle;
	DAQmxErrChk(DAQmxCreateTask("task0", &taskHandle));
	DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandle, "Dev1/ai0", "a0", DAQmx_Val_RSE, -1.0*V_MAX, V_MAX, DAQmx_Val_Volts, NULL));
	DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandle, "", SAMPLE_RATE, DAQmx_Val_Rising, DAQmx_Val_ContSamps, DAQBuffSize));
	DAQmxErrChk(DAQmxRegisterEveryNSamplesEvent(taskHandle, DAQmx_Val_Acquired_Into_Buffer, NSample, 0, EveryNCallback, (void *)this));
	DAQmxErrChk(DAQmxStartTask(taskHandle));

	emit threadStarted();
	QThread::exec();
Error:
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
	else
		emit response("qAIProducerThread finished!");
}

int32 CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
{
	int32   error = 0;
	int32   read = 0;
	char    errBuff[2048] = { '\0' };

	qAIProducerThread *pAIThread = (qAIProducerThread *)callbackData;
	qAICircularBuff *pCircularBuff = pAIThread->getCircularBuff();
	float64 *pData = pCircularBuff->pData;

	// DAQmx Read Code DAQmx_Val_Auto
	DAQmxErrChk(DAQmxReadAnalogF64(taskHandle, DAQmx_Val_Auto, 10.0, DAQmx_Val_GroupByChannel,
		pAIThread->getReadBuff(), pAIThread->getReadBuffSize(), &read, NULL));
	if (read > 0)
		pCircularBuff->pushData(pAIThread->getReadBuff(), read);

Error:
	if (DAQmxFailed(error)) {
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		// DAQmx Stop Code
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
		pAIThread->notifyErrorMsg(QString(errBuff));
	}
	return 0;
}

void qAIProducerThread::clearDAQTask()
{
		//stop and close
		//if (DAQmxFailed(error))
		//	DAQmxGetExtendedErrorInfo(errBuff, 2048);
	//if (taskHandle != 0)
	//{
	//	// DAQmx Stop Code
	//	DAQmxStopTask(taskHandle);
	//	DAQmxClearTask(taskHandle);
	//}
}
