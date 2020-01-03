#pragma once

#include <QMutex>
#include <QWaitCondition>

class qAICircularBuff
{
public:
	qAICircularBuff(int __buffsize, int readBlockSize);
	~qAICircularBuff();
	void pushData(double *srcData, int size);
	bool popData(double * dstData);
	int getBlockSize()
	{
		return blocksize;
	}

	void saveDataToCSV(const char * fileName);
	void reset();
	//void getData();
	double *pData;
	int buffSize;
	int writeInd;
	int readInd;
	int nSpaceAvailable;
	QMutex mutex;
	QWaitCondition moreSpace;
	QWaitCondition moreData;
	int blocksize;//number of bytes read
};
