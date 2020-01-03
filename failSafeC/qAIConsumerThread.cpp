#include "qAIConsumerThread.h"
#include "qDOWriteThread.h"
#include <algorithm>

using namespace std;

qAIConsumerThread::qAIConsumerThread(QObject *parent, qAICircularBuff *circularBuff)
	: QThread(parent)
{
	p_circularBuff = circularBuff;	
}

qAIConsumerThread::~qAIConsumerThread()
{
}

//NN is the length of the smoothing window to calcualte ISPPA and ISPTA.
//Since the sampling rate is 800,000 Hz (defined in producerThread), 
//for a 300kHz signal, one cycle should contain 800000/300000 = 2.67 points
// NN = 5 will be fine to capture the maxium point.
#define NN 8
void qAIConsumerThread::run()
{
	int blockSize = p_circularBuff->getBlockSize();
	double *pBlockBuffV = new double[blockSize]; //voltage
	double *pHead = new double[NN-1]; //for calculating Isppa and Ispta

	double *pBlockBuff_Isppa = new double[blockSize];
	double *pBlockBuff_Ispta = new double[blockSize];

	memset(pBlockBuffV, 0, sizeof(double)*(blockSize));
	memset(pBlockBuff_Isppa, 0, sizeof(double)*blockSize);
	memset(pBlockBuff_Ispta, 0, sizeof(double)*blockSize);
	memset(pHead, 0, sizeof(double)* (NN-1));

	double isi = m_isi;
	double lastIspta = 0;
	double deltaT = 1.0 / m_fs;
	double deltaT_over_ISI = deltaT/isi;

	int abnormalVIndex = -1;
	int abnormalISPTAIndex = -1;

	emit threadStarted();

	int nPointsAfterAbnorm = 0;
	m_quit = false;
	bool abnormVDetected = false;
	bool abnormISPTADetected = false;

	while (!m_quit)
	{
		//copy last NN-1 of old data
		for (int i=0; i<NN-1; i++)
			pHead[i] = pBlockBuffV[blockSize - (NN - 1) + i];
		
		//get new data
		if (!p_circularBuff->popData(pBlockBuffV))
			continue; //if timeout, then continue

		//scan voltages

		for (int i = 0; i < blockSize; i++)
		{
			if (!abnormVDetected)
			{
				if (abs(pBlockBuffV[i]) > m_volThres)
				{
					abnormalVIndex = i;
					if (m_break)
					{
						emit abnormVoltage(pBlockBuffV[i]); //  may consume memeory if emiting tens of thouands many times
					}
					abnormVDetected = true;
				}
			}
		}

		for (int i = 0; i < NN-1; i++)
		{
			double max1 = 0;
			for (int k = i; k < NN - 1; k++)
				max1 = std::max(abs(max1), abs(pHead[k]));

			for (int j = 0; j <= i; j++)
				max1 = std::max(abs(max1), abs(pBlockBuffV[j]));

			pBlockBuff_Isppa[i] = max1;
			//pBlockBuff_Ispta[i] = (lastIspta * (isi - deltaT) + pBlockBuff_Isppa[i] * deltaT) / isi; deltaT_over_ISI
			pBlockBuff_Ispta[i] = lastIspta * (1 - deltaT_over_ISI) + pBlockBuff_Isppa[i] * deltaT_over_ISI;
			lastIspta = pBlockBuff_Ispta[i];

			if (!abnormISPTADetected)
			{
				if (lastIspta > m_isptaThres)
				{
					abnormISPTADetected = true;
					abnormalISPTAIndex = i;
					if (m_break)
						emit abnormIspta(lastIspta);
				}
			}
		}

		for (int i = NN-1; i < blockSize; i++)
		{
			double sum1 = 0;
			//for(int j = i-(NN-1); j<=i; j++)
			//	sum1+= pBlockBuffV[j];

			double max1 = *std::max_element(pBlockBuffV+i-(NN-1), pBlockBuffV + i);
			double min1 = *std::min_element(pBlockBuffV + i - (NN - 1), pBlockBuffV + i);
			pBlockBuff_Isppa[i] = std::max(abs(max1), abs(min1));
			//
			//pBlockBuff_Ispta[i] = (lastIspta * (isi - deltaT) + pBlockBuff_Isppa[i] * deltaT) / isi;
			pBlockBuff_Ispta[i] = lastIspta * (1 - deltaT_over_ISI) + pBlockBuff_Isppa[i] * deltaT_over_ISI;
			lastIspta = pBlockBuff_Ispta[i];

			if (!abnormISPTADetected)
			{
				if (lastIspta > m_isptaThres)
				{
					abnormISPTADetected = true;
					abnormalISPTAIndex = i;
					if (m_break)
						emit abnormIspta(lastIspta);
				}
			}
		}

		if (abnormVDetected || abnormISPTADetected)
		{
			if ((p_AIPlotBuff == Q_NULLPTR) &&
				(p_isppaPlotBuff == Q_NULLPTR) &&
				(p_isptaPlotBuff == Q_NULLPTR))
			{
				m_quit = true; //Quit immediately
				emit allDataAcquired();
			}
			else
			{
				//copy data to buff for plotting
				if(p_AIPlotBuff)
					copyToPlotBuff(p_AIPlotBuff, pBlockBuffV, blockSize, abnormVDetected,
						abnormalVIndex);
				//Isppa uses same abnormal index as abnormal voltage. Maybe 0 if abrormal Ispta is deteched
				if(p_isppaPlotBuff)
					copyToPlotBuff(p_isppaPlotBuff, pBlockBuff_Isppa, blockSize, abnormVDetected,
						abnormalVIndex);
				if(p_isptaPlotBuff)
					copyToPlotBuff(p_isptaPlotBuff, pBlockBuff_Ispta, blockSize, abnormISPTADetected,
						abnormalISPTAIndex);
				nPointsAfterAbnorm += blockSize;
				if (nPointsAfterAbnorm >= m_totalRead)
				{
					m_quit = true; //get all the data, then quit
					emit allDataAcquired();
				}
			}
		}
	}

	delete[]pBlockBuffV;
	delete[]pHead;
	delete[]pBlockBuff_Isppa;
	delete[]pBlockBuff_Ispta;
}

void qAIConsumerThread::copyToPlotBuff(qPlotBuff * plotbuff, double * src,
	int newDataSize, bool abnormal, int abnormalIndex)
{
	QMutexLocker locker(&plotbuff->mutex);
	memcpy(plotbuff->y + plotbuff->wInd, src, sizeof(double)*newDataSize);
	plotbuff->wInd += newDataSize;
	plotbuff->abnormalDetected = abnormal;
	plotbuff->abnormalIndex = abnormalIndex;
	Q_ASSERT(plotbuff->wInd < plotbuff->size);
}
