#include <qwt_plot_canvas.h>
#include <qwt_plot_curve.h>
#include "qOscilloscope.h"

qOscilloscope::qOscilloscope(tOScopeData *pOScopeData, QString title, QWidget *parent)
	: QwtPlot(parent)
{
	m_pOScopeData = pOScopeData;
	setAutoReplot(false);
	setTitle(title);
	QwtPlotCanvas *canvas = new QwtPlotCanvas();
	canvas->setBorderRadius(10);
	setCanvas(canvas);
	setCanvasBackground(Qt::darkBlue);
	//
	enableAxis(QwtPlot::yLeft, false);
	enableAxis(QwtPlot::yRight, false);
	//
	//
	p_curve = new QwtPlotCurve("");
	p_curve->setPen(Qt::yellow);
	p_curve->setLegendAttribute(QwtPlotCurve::LegendShowLine);
	//p_curve->setYAxis(axisID);
	p_curve->setRawSamples(m_pOScopeData->xData, m_pOScopeData->yData, m_pOScopeData->size);
	p_curve->attach(this);
}

qOscilloscope::~qOscilloscope()
{
}

void qOscilloscope::setYAxis(int axisID, double yLimit)
{
	enableAxis(axisID);
	setAxisScale(axisID, -1 * yLimit, yLimit);
}

void qOscilloscope::replotOScope()
{
	m_pOScopeData->mutex.lock();
	p_curve->setRawSamples(m_pOScopeData->xData, m_pOScopeData->yData, m_pOScopeData->size);
	replot();
	m_pOScopeData->mutex.unlock();
}
