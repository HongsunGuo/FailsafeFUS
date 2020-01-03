#pragma once

#include <QWidget>
#include <QWizard>
#include "qCheckUSImpedancePage.h"
#include "qCheckOutputVPage.h"
#include "qTestFailSafePage.h"
#include "qIntroPage.h"
#include "qFailSafeWin.h"
#include "qManualCheckPage.h"

class qRecord
{
	Q_GADGET
public:
	qRecord();
	qRecord(QString _date, QString _time, QString _subjectID,
		QString _experimenter, double _checkV, bool _isAbnormalV,
		bool _isAbnormalIspta, double _zMag, double _phase);
	void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    QString date = "";
	QString time = "";
	QString subjectID = "";
	QString experimenter ="";
	double checkV = 0.0;
	bool isAbnormalV = false;
	bool isAbnormalIspta = false;
	double zMag = 0.0;
	double phase = 0.0;
};

class qFailSafeWizard : public QWizard
{
	Q_OBJECT

public:
	qFailSafeWizard(QWidget *parent = Q_NULLPTR);
	~qFailSafeWizard();
private:
	QWizardPage *checkBNCPage();
	QWizardPage *groundInstrumentsPage();
	QWizardPage *connectPowerPage();
	QWizardPage *checkErrorCodesPage();
	QWizardPage *testFailSafePage();
	//
	qManualCheckPage *p_manualCheckPage;
	qIntroPage *p_introPage;
	qCheckUSImpedancePage *p_checkUSImpPage;
	//
	qCheckOutputVPage *p_checkOutputVPage;
	//
	qTestFailSafePage *p_testFailsafePage;
	//
	qFailSafeWin *p_failSafeWin;
	void showDAQDialog();
	//JSON read write
	bool loadJSONFile();
	bool writeJSONFile(const qRecord &newRecord);
	QVector<qRecord> m_records;
public slots:
	void done(int result) override;
};
