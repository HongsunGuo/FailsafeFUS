#pragma once

#include <QWizardPage>
#include <QVector>
#include "ui_qCheckUSimpedancePage.h"

class qRecord;
class qCheckUSImpedancePage : public QWizardPage
{
	Q_OBJECT

public:
	qCheckUSImpedancePage(QWidget *parent = Q_NULLPTR);
	~qCheckUSImpedancePage();
	bool validatePage() override;
	void updateRecords(const QVector<qRecord> &records);
private:
	Ui::qCheckUSimpedancePage ui;
	void addLogItem(QString date, QString time, QString experimenter,
		QString subjectID, QString zMag, QString phase);
	bool m_loged = false;
};
