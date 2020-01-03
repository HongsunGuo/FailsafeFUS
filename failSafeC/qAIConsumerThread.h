#pragma once

#include <QThread>
#include "qAICircularBuff.h"
#include "qPlotBuff.h"
#include "failSafeGlobal.h"
class qDOWriteThread;

class qAIConsumerThread : public QThread
{
	Q_OBJECT

public:
	qAIConsumerThread(QObject *parent, qAICircularBuff *circularBuff);
	~qAIConsumerThread();
	//void setDO(qDOWriteThread *_pDO)
	//{
	//	pDO = _pDO;
	//}
	void breakRealy(bool _break)
	{
		m_break = _break;
	}
	void quitNow()
	{
		m_quit = true;
	}
	void setQuit(bool b)
	{
		m_quit = b;
	}
	void setPlotBuff(qPlotBuff *pAIPlotBuff, qPlotBuff *pIsppaPlotBuff, qPlotBuff *pIsptaPlotBuff)
	{
		p_AIPlotBuff = pAIPlotBuff;
		p_isppaPlotBuff = pIsppaPlotBuff;
		p_isptaPlotBuff = pIsptaPlotBuff;
	}

	void setRecordMaxLen(int totalRead)
	{
		m_totalRead = totalRead;
	}

	int getRecordMaxLen()
	{
		return m_totalRead;
	}

	void setFs(double fs)
	{
		m_fs = fs;
	}

	void setISI(double isi)
	{
		m_isi = isi;
	}
	double getISI() { return m_isi; }
	
	void setVolThreshold(double volThres)
	{
		m_volThres = volThres;
	}
	double getVolThreshold() { return m_volThres; }

	void setISPTAThreshold(double isptaThres)
	{
		m_isptaThres = isptaThres;
	}
	double getISPTAThreshold() { return m_isptaThres; }

private:
	qAICircularBuff *p_circularBuff;
	//qDOWriteThread *pDO;
	void run() override;
	bool m_quit = false;
	double m_volThres = 0.9;
	double m_isptaThres = 0.55;
	double m_isi = 1.0;
	double m_fs = SAMPLE_RATE;
	qPlotBuff *p_AIPlotBuff = Q_NULLPTR;
	qPlotBuff *p_isppaPlotBuff = Q_NULLPTR;
	qPlotBuff *p_isptaPlotBuff = Q_NULLPTR;
	int m_totalRead = 20000;
	bool m_break = true;
	void copyToPlotBuff(qPlotBuff * plotbuff, double * src, int newDataSize, bool abnormal, int abnormalIndex);
signals:
	void abnormVoltage(double v);
	void abnormIspta(double ispta);
	void threadStarted();
	void allDataAcquired();
};
