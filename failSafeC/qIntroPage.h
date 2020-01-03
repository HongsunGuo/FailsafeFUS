#pragma once

#include <QWizardPage>
#include <QDateTimeEdit>
#include "ui_qIntroPage.h"

class qIntroPage : public QWizardPage
{
	Q_OBJECT

public:
	qIntroPage(QWidget *parent = Q_NULLPTR);
	~qIntroPage();
	void getUserInfo(QString &experimenter, QString &subjectID);
	bool validatePage() override;
private:
	Ui::qIntroPage ui;
	QDateTimeEdit  *p_dateTime;
};
