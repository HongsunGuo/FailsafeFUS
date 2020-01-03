#include "qIntroPage.h"
#include <QMessageBox>

extern const QString FIELD_DATE_TIME = "dateTimefield";
extern const QString FIELD_XPMTR_iD = "experimenter";
extern const QString FIELD_SUBJECT_iD = "subjectID";

qIntroPage::qIntroPage(QWidget *parent)
	: QWizardPage(parent)
{
	ui.setupUi(this);
	ui.calendarWidget->showToday();
	ui.calendarWidget->setDateEditEnabled(false);
	ui.calendarWidget->setSelectionMode(QCalendarWidget::NoSelection);
	setTitle("Introduction");
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark1.png"));
	setSubTitle("This wizard will help you configure the failsafe for "
		"ultrasound neuromodualtion on humans. Please follow this instruction step by step to complete settings.");
	//
	p_dateTime = new QDateTimeEdit(this);
	p_dateTime->setVisible(false);
	registerField(FIELD_DATE_TIME, p_dateTime);
	registerField(FIELD_XPMTR_iD, ui.exptLineEdit);
	registerField(FIELD_SUBJECT_iD, ui.subjectIDLineEdit);
}

qIntroPage::~qIntroPage()
{
}

void qIntroPage::getUserInfo(QString & experimenter, QString &subjectID)
{
	experimenter = ui.exptLineEdit->text();
	subjectID = ui.subjectIDLineEdit->text();
}

bool qIntroPage::validatePage()
{
	if (ui.exptLineEdit->text().isEmpty() || ui.subjectIDLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Warning", "Experimenter or subject ID cannot be empty!");
		return false;
	}
	else
	{
		p_dateTime->setDate(QDate::currentDate());
		p_dateTime->setTime(QTime::currentTime());
		return true;
	}
}
