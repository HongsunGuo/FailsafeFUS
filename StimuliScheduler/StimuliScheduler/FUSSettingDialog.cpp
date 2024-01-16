#include "FUSSettingDialog.h"

using namespace std;

FUSSettingDialog::FUSSettingDialog(shared_ptr<FUSStimulus> ptr, QWidget *parent)
	:m_FUSPtr(ptr), QDialog(parent)
{
	ui.setupUi(this);
	if (m_FUSPtr->m_fusParameter.mode == FUSStimulus::FUSParameter::Pulsed) {
		ui.PRFLineEdit->setEnabled(true);
		ui.modeComboBox->setCurrentText("Pulsed");
	}
	else {
		ui.PRFLineEdit->setEnabled(false);
		ui.modeComboBox->setCurrentText("Continuous");
	}

	ui.cFreqLineEdit->setText(QString::number(m_FUSPtr->m_fusParameter.cFreq_MHz));
	ui.nPulseLineEdit->setText(QString::number(m_FUSPtr->m_fusParameter.n_pulses));
	ui.PDLineEdit->setText(QString::number(m_FUSPtr->m_fusParameter.dur_ms));
	ui.PRFLineEdit->setText(QString::number(m_FUSPtr->m_fusParameter.PRF_kHz));
	ui.pressureLineEdit->setText(QString::number(m_FUSPtr->m_fusParameter.pressure_kPa));

	connect(ui.modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxChange()));
	connect(ui.okPushButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui.cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

FUSSettingDialog::~FUSSettingDialog()
{}


void FUSSettingDialog::accept() {
	//Update FUSParameter
	if (ui.modeComboBox->currentText() == "Pulsed")
		m_FUSPtr->m_fusParameter.mode = FUSStimulus::FUSParameter::Pulsed;
	else
		m_FUSPtr->m_fusParameter.mode = FUSStimulus::FUSParameter::Continuous;

	m_FUSPtr->m_fusParameter.cFreq_MHz = ui.cFreqLineEdit->text().toDouble();
	m_FUSPtr->m_fusParameter.n_pulses = ui.nPulseLineEdit->text().toInt();
	m_FUSPtr->m_fusParameter.dur_ms = ui.PDLineEdit->text().toInt();
	m_FUSPtr->m_fusParameter.PRF_kHz = ui.PRFLineEdit->text().toInt();
	m_FUSPtr->m_fusParameter.pressure_kPa = ui.pressureLineEdit->text().toInt();
	QDialog::accept();
}

void FUSSettingDialog::reject() {
	QDialog::reject();
}

void FUSSettingDialog::onComboBoxChange() {
	QString selectedItem = ui.modeComboBox->currentText();
	if(selectedItem == "Pulsed")
		ui.PRFLineEdit->setEnabled(true);
	else
		ui.PRFLineEdit->setEnabled(false);
}
