#include "qV2IntensityDlg.h"

qV2IntensityDlg::qV2IntensityDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	//
	series = new QLineSeries();
	*series << QPointF(11, 1) << QPointF(13, 3)
		<< QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);
	//
	chart = new QChart();
	chart->legend()->hide();
	chart->addSeries(series);
	chart->createDefaultAxes();
	chart->setTitle("Voltage (V) to intensity (W/cm^2)");
	//
	chartView = new QChartView(chart);
	ui.verticalLayout->addWidget(chartView);
}

qV2IntensityDlg::~qV2IntensityDlg()
{
}
