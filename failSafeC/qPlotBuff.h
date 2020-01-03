#pragma once
#include <QMutex>
#include <failSafeGlobal.h>
#include <algorithm>


class qPlotBuff
{
public:
	qPlotBuff() {}
	~qPlotBuff()
	{
		clear();
	}

	double *x = NULL;
	double *y = NULL;
	int size = 0; //total size
	int wInd = 0; //how many points it has now
	int abnormalIndex = -1;
	bool abnormalDetected = false;
	QMutex mutex;

	double getAbsMax()
	{
		double max1 = *std::max_element(y, y+wInd);
		double min1 = *std::min_element(y, y + wInd);
		return std::max(abs(max1), abs(min1));
	}

	void init(int _size)
	{
		size = _size;
		x = new double[size];
		y = new double[size];
		reset();
	}

	void reset()
	{
		wInd = 0;
		abnormalDetected = false;
		abnormalIndex = -1;
		memset(x, 0, sizeof(double)*size);
		memset(y, 0, sizeof(double)*size);
		double dt = 1000 / SAMPLE_RATE; //in ms
		for (int i = 0; i < size; i++)
			x[i] = i * dt;
	}

	void clear()
	{
		delete[]x;
		delete[]y;
	}
};
