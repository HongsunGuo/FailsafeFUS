#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_text.h>
#include <qwt_plot_canvas.h>
#include <qwt_scale_engine.h>
#include <algorithm>
#include "qUSIntensityPlot.h"

qUSIntensityPlot::qUSIntensityPlot(QWidget *parent, QString title, qPlotBuff *pPlotBuff, int axisID)
	: QwtPlot(parent), p_plotBuff(pPlotBuff)
{
	setAutoReplot(true);
	setTitle(title);
	QwtPlotCanvas *canvas = new QwtPlotCanvas();
	canvas->setBorderRadius(10);
	setCanvas(canvas);
	setCanvasBackground(Qt::darkBlue);
	//
	enableAxis(QwtPlot::yLeft, false);
	enableAxis(QwtPlot::yRight, false);
	enableAxis(axisID);
	setAxisScale(QwtPlot::xBottom, 0, p_plotBuff->size-1);

	//
	m_xLimit[0] = 1;
	m_xLimit[1] = pPlotBuff->size;
	memset(m_yLimit, 0, sizeof(double) * 2);
	p_limitLine = new QwtPlotCurve("Limit line");
	p_limitLine->setPen(Qt::green, 3, Qt::DashLine);
	p_limitLine->setYAxis(axisID);
	p_limitLine->setRawSamples(m_xLimit, m_yLimit, 2);
	p_limitLine->attach(this);
	//
	p_curve = new QwtPlotCurve("");
	p_curve->setPen(Qt::yellow);
	p_curve->setLegendAttribute(QwtPlotCurve::LegendShowLine);
	p_curve->setYAxis(axisID);
	p_curve->setRawSamples(pPlotBuff->x, pPlotBuff->y, pPlotBuff->size);
	p_curve->setRenderHint(QwtPlotItem::RenderAntialiased);
	p_curve->attach(this);
	//
	p_verticalLine = new QwtPlotCurve("");
	p_verticalLine->setPen(Qt::red, 3, Qt::DashLine);
	p_verticalLine->setYAxis(axisID);
	

	//clear all the x labels
	setAxisScaleEngine(QwtPlot::xBottom, new QwtLinearScaleEngine );
	//setAxisAutoScale(QwtPlot::xBottom);
	//setAxisScaleDraw(QwtPlot::yLeft, &m_realVolScaleDraw);
	//
	//QList<double> minorTicks;
	//QList<double> mediumTicks;
	//QList<double> majorTicks;
	//for (int i = -3; i <= 3; i++)
	//	majorTicks.append(i* 50.0/ NI_ATTEN_FACTOR);
	//p_scaleDiv = new QwtScaleDiv(-3, 3, minorTicks, mediumTicks, majorTicks);
	//setAxisScaleDiv(QwtPlot::yLeft, *p_scaleDiv);
}

qUSIntensityPlot::~qUSIntensityPlot()
{
}

//axisID = QwtPlot::yRight or QwtPlot::yLeft
void qUSIntensityPlot::setYAxisMax(int axisID, double yMax)
{
	enableAxis(axisID);
	m_yMax = yMax;
	setAxisScale(axisID, -1 * yMax, yMax);
}

void qUSIntensityPlot::replotCurve(int nDataSize)
{
	QMutexLocker locker(&p_plotBuff->mutex);
	if (p_plotBuff->abnormalDetected)
	{
		//highlight the abnormal points
		double dt = 1000 / SAMPLE_RATE; //in ms
		double x[2] = {dt*(double) p_plotBuff->abnormalIndex, dt*(double) p_plotBuff->abnormalIndex };
		double y[2] = {-1*m_yMax, m_yMax};
		p_verticalLine->setSamples(x,y,2);
		p_verticalLine->attach(this);
	}

	p_curve->setRawSamples(p_plotBuff->x, p_plotBuff->y, nDataSize);
	setAxisScale(QwtPlot::xBottom, 0, p_plotBuff->x[nDataSize-1]);
	replot();
}

void qUSIntensityPlot::eraseAllPlots()
{
	QMutexLocker locker(&p_plotBuff->mutex);
	double x[2] = { 0,0 };
	double y[2] = { 0,0 };
	p_verticalLine->setSamples(x, y, 0);
	p_curve->setRawSamples(p_plotBuff->x, p_plotBuff->y, 0);
	replot();
}