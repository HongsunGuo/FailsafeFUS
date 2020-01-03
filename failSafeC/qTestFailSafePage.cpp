#include "qTestFailSafePage.h"
#include "qSanityChkDlg.h"

qTestFailSafePage::qTestFailSafePage(QWidget *parent)
	: QWizardPage(parent)
{
	ui.setupUi(this);
	//
	setTitle("Sanity test for the Failsafe");
	setSubTitle(" ");
	//
	connect(ui.bigSpikeBtn, SIGNAL(clicked()), this, SLOT(testBigSpike()));
	connect(ui.highVBtn, SIGNAL(clicked()), this, SLOT(testBigVoltage()));
}

qTestFailSafePage::~qTestFailSafePage()
{
}

void qTestFailSafePage::testBigSpike()
{
	qSanityChkDlg sanityDlg(qSanityChkDlg::BIGSPIKE);
	sanityDlg.exec();
}

void qTestFailSafePage::testBigVoltage()
{
	qSanityChkDlg sanityDlg(qSanityChkDlg::LARGEVOLTAGE);
	sanityDlg.exec();
}
