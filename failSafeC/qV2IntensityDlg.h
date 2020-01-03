#pragma once

#include <QDialog>
#include <qchart.h>
#include <QLineSeries>
#include <QChartView>
#include "ui_qV2IntensityDlg.h"

using namespace QtCharts;
class qV2IntensityDlg : public QDialog
{
	Q_OBJECT

public:
	qV2IntensityDlg(QWidget *parent = Q_NULLPTR);
	~qV2IntensityDlg();

private:
	Ui::qV2IntensityDlg ui;
	QLineSeries *series;
	QChart *chart;
	QChartView *chartView;
};
