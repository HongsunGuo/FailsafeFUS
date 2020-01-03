#pragma once

#include "qwt_plot.h"
#include "qPlotBuff.h"
#include <QTimer>
#include <qwt_scale_draw.h>

class QwtPlotCurve;

//class realVolScaleDraw : public QwtScaleDraw
//{
//public:
//	realVolScaleDraw() :
//		QwtScaleDraw()
//	{
//	}
//	virtual QwtText label(double v) const
//	{
//		return QString::number(v*NI_ATTEN_FACTOR);
//	}
//};

class qUSIntensityPlot : public QwtPlot
{
	Q_OBJECT

public:
	qUSIntensityPlot(QWidget *parent, QString title, qPlotBuff *pPlotBuff, int axisID);
	~qUSIntensityPlot();
	void setYAxisMax(int axisID, double yMax);
	void setYLimitLine(double y)
	{
		m_yLimit[0] = y;
		m_yLimit[1] = y;
		replot();
	}

	void replotCurve(int nDataSize);
	void eraseAllPlots();
private:
	QwtPlotCurve *p_curve;
	QwtPlotCurve *p_limitLine;
	QwtPlotCurve *p_verticalLine;
	//realVolScaleDraw m_realVolScaleDraw;
	QwtScaleDiv *p_scaleDiv;

	qPlotBuff *p_plotBuff;
	double m_xLimit[2];
	double m_yLimit[2];
	double m_yMax;
};
