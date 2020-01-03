#pragma once

#include "qwt_plot.h"
#include "failSafeGlobal.h"

class QwtPlotCurve;
class qOscilloscope : public QwtPlot
{
	Q_OBJECT

public:
	qOscilloscope(tOScopeData *pOScopeData, QString title, QWidget *parent = Q_NULLPTR);
	~qOscilloscope();
	void setYAxis(int axisID, double yLimit);
	//void startMonitoring();
private slots:
	void replotOScope();
private:
	tOScopeData *m_pOScopeData;
	QwtPlotCurve *p_curve;
};
