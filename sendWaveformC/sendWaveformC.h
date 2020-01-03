#pragma once

#include <QtWidgets/QDialog>
#include <QThread>
#include "ui_sendWaveformC.h"
#include "keysight33503A.h"

class sendWaveformC : public QDialog
{
	Q_OBJECT

public:
	sendWaveformC(QWidget *parent = Q_NULLPTR);
public slots:
	void done(int r) override;
private:
	Ui::sendWaveformCClass ui;
	keysight33503A *pFGThread;
	void setAllButtons(bool on);
private slots:
	void testIsppa();
	void testIspta();
	void checkVoltage();
	void currentThreadFinished();
	void showErrMsg(QString msg);
	//
};
