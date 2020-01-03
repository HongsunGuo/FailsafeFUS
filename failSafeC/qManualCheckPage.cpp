#include "qManualCheckPage.h"
#include <QMessageBox>

qManualCheckPage::qManualCheckPage(QWidget *parent)
	: QWizardPage(parent)
{
	ui.setupUi(this);
	setTitle("Checklist");
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark1.png"));
	setSubTitle("Please complete the following checklist!");
}

qManualCheckPage::~qManualCheckPage()
{
}

bool qManualCheckPage::validatePage()
{
	if ((Qt::Unchecked == ui.bncCheckbox->checkState()) || 
		(Qt::Unchecked == ui.errorCodeCheckbox->checkState()) ||
		(Qt::Unchecked == ui.groudCheckbox->checkState()) ||
		(Qt::Unchecked == ui.powerCheckbox->checkState()))
	{
		QMessageBox::warning(this, "Warning", "Please complete the checklist!");
		return false;
	}
	else
		return true;
}
