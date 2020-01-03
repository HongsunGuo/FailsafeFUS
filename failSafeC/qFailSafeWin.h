#pragma once

#include <QMainWindow>
#include <QGridLayout>
#include <QListWidget>
#include <QDir>
#include "ui_qFailSafeWin.h"
#include "qUSIntensityPlot.h"
#include "qOscilloscope.h"
#include "qPlotBuff.h"
#include "qAIConsumerThread.h"
#include "qAIProducerThread.h"
#include "qAICircularBuff.h"
#include "qDOWriteThread.h"

class qFailSafeWin : public QMainWindow
{
	Q_OBJECT

public:
	qFailSafeWin(QWidget *parent = Q_NULLPTR);
	~qFailSafeWin();

	bool isAbnormalV() const
	{
		return m_abnormalV;
	}

	bool isAbnormalISPTA() const
	{
		return m_abnormalIspta;
	}

	void setDataDir(const QDir &dir)
	{
		m_dir = dir;
	}

private:
	QDir m_dir;//current data dir
	Ui::qFailSafeWin ui;
	qUSIntensityPlot *p_isppaPlot;
	qUSIntensityPlot *p_isptaPlot;
	qUSIntensityPlot *p_AIPlot;
	qOscilloscope *p_oscilloscope;
	QGridLayout  *p_gridLayout;
	QAction *startAction;
	QAction *stopAction;
	QAction *calibrationAction;
	QAction * isiAction;
	QAction * vThresAction;
	QAction * isptaThresAction;
	QListWidget *p_logListWidget;
	//
	//buffers
	qPlotBuff m_AIPlotBuff;
	qPlotBuff m_isppaPlotBuff;
	qPlotBuff m_isptaPlotBuff;

	//threads
	qAIProducerThread *p_aiProducerThread;
	qAIConsumerThread *p_aiConsumerThread;
	qDOWriteThread *p_doWriteThread;
	qAICircularBuff *p_AIBuff;
	tOScopeData m_OScopeData;
	//
	bool m_abnormalV = false;
	bool m_abnormalIspta = false;
	//
	void startDAQThreads();
	void stopDAQThreads();
private slots:
	void onStartMonitoring();
	void onStopMonitoring();
	void updatePlots();
	void onAllThreadStarted();
	void onCalibrActionClicked();
	void onISIActionClicked();
	void onVThresActionClicked();
	void onIsptaThresActionClicked();
	void onStopActionClicked();
	void logAbnormalVoltage(double v);
	void logAbnormalIspta(double ispta);
private:
	void saveData(const char * filename, qPlotBuff *plotBuff);
	void addLog(QString);
	bool checkPassword();
protected:
	virtual void closeEvent(QCloseEvent *event);
};
