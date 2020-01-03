#include "qCheckUSImpedancePage.h"
#include "failSafeGlobal.h"
#include "qFailSafeWizard.h"
#include <QMessageBox>
#include <QDateTime>

qCheckUSImpedancePage::qCheckUSImpedancePage(QWidget *parent)
	: QWizardPage(parent)
{
	ui.setupUi(this);
	setTitle("Check the impedance of the ultraound transducer");
	setSubTitle(" ");
	ui.listWidget->addItem("Date\tTime\tExperimenter\tSubjectID\tzMag (ohm)\tPhase (degree)");
//	ui.listWidget->addItem(formatToListItem("2019-08-15", "50.7", "19"));
	registerField(FIELD_ZMAG, ui.zmagEdit);
	registerField(FIELD_PHASE, ui.phaseEdit);
}

qCheckUSImpedancePage::~qCheckUSImpedancePage()
{
}

void qCheckUSImpedancePage::addLogItem(QString date, QString time, QString experimenter,
	QString subjectID, QString zMag, QString phase)
{
	ui.listWidget->addItem(date + "\t" + time + "\t" + experimenter + "\t"
		+ subjectID + "\t" + zMag + "\t" + phase);
}

bool qCheckUSImpedancePage::validatePage()
{
	double zmag = ui.zmagEdit->text().toDouble();
	double phase = ui.phaseEdit->text().toDouble();
	QDateTime dateTime = field(FIELD_DATE_TIME).toDateTime();
	if (zmag<35 || zmag>65 ||
		phase<-50 || phase>50)
	{
		QMessageBox::warning(this, "Warning",
			"Zmag and/or Phase are wrong! Please check the transducer!");
		return false;
	}
	else
	{
		//TODO: need to check if zmagEdit and phase were changed
		if (!m_loged)
		{
			addLogItem(dateTime.date().toString("yyyy-MM-dd"),
				dateTime.time().toString("HH:mm:ss"),
				field(FIELD_XPMTR_iD).toString(),
				field(FIELD_SUBJECT_iD).toString(),
				QString::number(zmag),
				QString::number(phase));
			m_loged = true;
		}
		return true;
	}
}

void qCheckUSImpedancePage::updateRecords(const QVector<qRecord>& records)
{
	for (const qRecord &record : records) {
		addLogItem(record.date, record.time, record.experimenter,
			record.subjectID, QString::number(record.zMag), QString::number(record.phase));
	}
}
