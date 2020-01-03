#pragma once

#include "qPlotBuff.h"
#include "qwt_plot.h"
#include <QTimer>

class QwtPlotCurve;

class qAIPlot : public QwtPlot
{
	Q_OBJECT

public:
	qAIPlot(QWidget *parent, QString title, qPlotBuff *pAIPlotBuff, int axisID);
	~qAIPlot();

	void setYAxisMax(double yMax);
	void startPlotting();
	void stopPlotting();
	void setYLimitLine(double y)
	{
		m_yLimit[0] = y;
		m_yLimit[1] = y;
		replot();
	}

private:
	QwtPlotCurve *p_curve;
	QwtPlotCurve *p_limitLine;
	double m_xLimit[2];
	double m_yLimit[2];
	QTimer m_timer;
	//
	qPlotBuff *p_AIPlotBuff;
	qPlotBuff *p_isppaPlotBuff;
	qPlotBuff *p_isptaPlotBuff;

private slots:
	void replotCurve();
	void stopPlotting();
};
