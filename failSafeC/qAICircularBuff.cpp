#include "qAICircularBuff.h"


qAICircularBuff::qAICircularBuff(int __buffsize, int readBlockSize)
{
	buffSize = __buffsize;
	blocksize = readBlockSize;
	pData = new double[buffSize];
	memset(pData, 0, sizeof(double)*buffSize);
	//
	nSpaceAvailable = buffSize;
	readInd = 0;
	writeInd = 0;
}

qAICircularBuff::~qAICircularBuff()
{
	delete[]pData;
}

void qAICircularBuff::pushData(double * srcData, int size)
{
	//push new data to circular buff
	const QMutexLocker locker(&mutex);
	if (nSpaceAvailable < size)
		moreSpace.wait(&mutex); //waiting for more space

	
	if (writeInd + size > buffSize)
	{
		int nMore = writeInd + size - buffSize;
		memcpy(pData + writeInd, srcData, sizeof(double)*(size - nMore));
		memcpy(pData, srcData + size - nMore, sizeof(double)*nMore); //write to the front
	}
	else
		memcpy(pData + writeInd, srcData, sizeof(double)*size);
	
	nSpaceAvailable -= size;
	//
	writeInd = (writeInd + size) % buffSize;
	if ((buffSize - nSpaceAvailable) >= blocksize)
		moreData.wakeAll(); //at least blocksize data available, wake up popData
}

//return false if moreData.wait timeout
bool qAICircularBuff::popData(double * dstData)
{
	const QMutexLocker locker(&mutex);
	bool isTimeout = false;
	if (buffSize - nSpaceAvailable < blocksize)
		isTimeout = moreData.wait(&mutex, 500); //waiting for more data

	if (!isTimeout)
		return false;

	int nMore = readInd + blocksize - buffSize;
	if (readInd + blocksize > buffSize)
	{
		memcpy(dstData, pData + readInd, sizeof(double)*(buffSize - readInd));
		memcpy(dstData + buffSize - readInd, pData, sizeof(double)*nMore);
	}
	else
		memcpy(dstData, pData + readInd, sizeof(double)*blocksize);

	readInd = (readInd + blocksize) % buffSize;
	nSpaceAvailable += blocksize;

	if(nSpaceAvailable > blocksize) //assuming blocksize bigger than the size of uncoming data
		moreSpace.wakeAll();
	return true;
}

void qAICircularBuff::saveDataToCSV(const char *fileName)
{
	const QMutexLocker locker(&mutex);
	FILE *fp;
	/* Open file for output. */
	fp = fopen(fileName, "wb");
	/* Output waveform data in CSV format. */
	
	if (writeInd > readInd)
	{
		for (int i = readInd; i < writeInd; i++)
		{
			/* Write voltage value. */
			fprintf(fp, "%6f\n", pData[i]);
		}
	}
	/* Close output file. */
	fclose(fp);
}

void qAICircularBuff::reset()
{
	memset(pData, 0, sizeof(double)*buffSize);
	//
	nSpaceAvailable = buffSize;
	readInd = 0;
	writeInd = 0;
}
