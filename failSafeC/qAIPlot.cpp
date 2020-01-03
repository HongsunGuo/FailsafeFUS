#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_text.h>
#include <qwt_plot_canvas.h>
#include "qAIPlot.h"

qAIPlot::qAIPlot(QWidget *parent, QString title, qPlotBuff *pAIPlotBuff, int axisID)
	: QwtPlot(parent), p_AIPlotBuff(pAIPlotBuff)
{
	setAutoReplot(false);
	setTitle(title);
	enableAxis(QwtPlot::yLeft, false);
	enableAxis(QwtPlot::yRight, false);
	enableAxis(axisID, true);
	//
	QwtPlotCanvas *canvas = new QwtPlotCanvas();
	canvas->setBorderRadius(10);
	setCanvas(canvas);
	setCanvasBackground(Qt::darkBlue);
	//
	
	//
	m_xLimit[0] = 1;
	m_xLimit[1] = pAIPlotBuff->size;
	memset(m_yLimit, 0, sizeof(double) * 2);
	p_limitLine = new QwtPlotCurve("Limit line");
	p_limitLine->setPen(Qt::yellow, 3, Qt::DashLine);
	p_limitLine->setYAxis(axisID);
	p_limitLine->setRawSamples(m_xLimit, m_yLimit, 2);
	p_limitLine->attach(this);
	//
	p_curve = new QwtPlotCurve("");
	p_curve->setPen(Qt::yellow);
	//p_curve->setLegendAttribute(QwtPlotCurve::LegendShowLine);
	p_curve->setYAxis(axisID);
	p_curve->setRawSamples(pAIPlotBuff->x, pAIPlotBuff->y, pAIPlotBuff->size);
	p_curve->attach(this);
}


qAIPlot::~qAIPlot()
{
	p_aiProducerThread->exit();
	p_aiConsumerThread->quitNow();
	p_aiProducerThread->wait();
	p_aiConsumerThread->wait();
}

void qAIPlot::setYAxisMax(double yMax)
{
	setAxisScale(QwtPlot::yLeft, -1* yMax, yMax);
}

void qAIPlot::startPlotting()
{
	m_timer.start(100);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(replotCurve()));
}

void qAIPlot::stopPlotting()
{
	m_timer.stop();
	p_aiProducerThread->exit();
	p_aiConsumerThread->quitNow();
	p_aiProducerThread->wait();
	p_aiConsumerThread->wait();
}

void qAIPlot::replotCurve()
{
	QMutexLocker locker(&p_AIPlotBuff->mutex);
	setAxisScale(QwtPlot::yLeft, -1 * 0.1, 0.1);
	setAxisScale(QwtPlot::xBottom, 0, p_AIPlotBuff->size);
	replot();
}

void qAIPlot::stopPlotting()
{
	stopMonitoring();

}