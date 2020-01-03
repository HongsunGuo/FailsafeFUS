#pragma once

#include <QWizardPage>
#include "ui_qTestFailSafePage.h"

class qTestFailSafePage : public QWizardPage
{
	Q_OBJECT

public:
	qTestFailSafePage(QWidget *parent = Q_NULLPTR);
	~qTestFailSafePage();

private:
	Ui::qTestFailSafePage ui;
private slots:
	void testBigSpike();
	void testBigVoltage();
};
