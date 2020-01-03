#include "qContAcqAITrigThread.h"
#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else
static int32 CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);

qContAcqAITrigThread::qContAcqAITrigThread(double vTrigThreshold, qAICircularBuff *pAIBuff, QObject *parent)
	: m_vTrigThreshold(vTrigThreshold), m_pAIBuff(pAIBuff), QThread(parent)
{
	data = new double[8000];
}

qContAcqAITrigThread::~qContAcqAITrigThread()
{
	delete[]data;
}

void qContAcqAITrigThread::cleanDAQTask()
{
	if (taskHandle != 0) {
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}
}

void qContAcqAITrigThread::run()
{
	int error = 0;
	char        errBuff[2048] = { '\0' };
	int32 DAQBuffSize = 2000000;
	int32 NSample = 2000; //read may be slightly larger than readExpected. so expect less to avoid outflow.
	int sampleRate = 800000;

	DAQmxErrChk(DAQmxCreateTask("", &taskHandle));
	DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandle, "Dev1/ai0", "", DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL));
	DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandle, "", sampleRate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 1000000));
	DAQmxErrChk(DAQmxCfgAnlgEdgeStartTrig(taskHandle, "APFI0", DAQmx_Val_Rising, m_vTrigThreshold));
	DAQmxErrChk(DAQmxSetAnlgEdgeStartTrigHyst(taskHandle, 0.1));
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
		emit response("qContAcqAITrigThread finished!");
}

int32 CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
{
	int32       error = 0;
	char        errBuff[2048] = { '\0' };
	static int  totalRead = 0;
	static int nEntry = 0;
	int32       read = 0;

	qContAcqAITrigThread *pAcqThread = (qContAcqAITrigThread *)callbackData;

	/*********************************************/
	// DAQmx Read Code
	/*********************************************/
	DAQmxErrChk(DAQmxReadAnalogF64(taskHandle, DAQmx_Val_Auto, 10.0, DAQmx_Val_GroupByChannel,
		pAcqThread->getDataPtr(), 8000, &read, NULL));
	if (read > 0)
	{
		if (++nEntry == 1)
			pAcqThread->notifyDAQStarted();
		totalRead += read;
		pAcqThread->pushToBuff(pAcqThread->getDataPtr(), read);
		if (totalRead>= pAcqThread->getRecordMaxLen())
		{
			pAcqThread->notifyAllDataAcquired();
			/*pAcqThread->quit();
			pAcqThread->wait();
			pAcqThread->cleanDAQTask();*/
			totalRead = 0;
			nEntry = 0;
		}
		//printf("Acquired %d samples. Total %d\r", (int)read, (int)(totalRead += read));
		//fflush(stdout);
	}

Error:
	if (DAQmxFailed(error)) {
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		// DAQmx Stop Code
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);

		pAcqThread->notifyErrorMsg(QString(errBuff));
		//printf("DAQmx Error: %s\n", errBuff);
	}
	return 0;
}
