#pragma once

#include <QWizardPage>
#include "ui_qManualCheckPage.h"

class qManualCheckPage : public QWizardPage
{
	Q_OBJECT

public:
	qManualCheckPage(QWidget *parent = Q_NULLPTR);
	~qManualCheckPage();
	bool validatePage() override;
private:
	Ui::qManualCheckPage ui;
};
