#include "SoundSettingDialog.h"

using namespace std;

SoundSettingDialog::SoundSettingDialog(shared_ptr<SoundStimulus> ptr, QWidget *parent)
	: m_soundPtr(ptr), QDialog(parent)
{
	ui.setupUi(this);

	if (m_soundPtr->m_soundParameter.mode == SoundStimulus::SoundParameter::BN) {
		ui.modeComboBox->setCurrentText("BN");
		ui.freqLineEdit->setEnabled(false);
	}
	else {
		ui.modeComboBox->setCurrentText("Tone");
		ui.freqLineEdit->setEnabled(true);
	}

	ui.durLineEdit->setText(QString::number(m_soundPtr->m_soundParameter.dur_ms));
	ui.freqLineEdit->setText(QString::number(m_soundPtr->m_soundParameter.freq_kHz));

	connect(ui.modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxChange()));
	connect(ui.okPushButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui.cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

SoundSettingDialog::~SoundSettingDialog()
{}

void SoundSettingDialog::accept() {
	if (ui.modeComboBox->currentText() == "BN")
		m_soundPtr->m_soundParameter.mode = SoundStimulus::SoundParameter::BN;
	else
		m_soundPtr->m_soundParameter.mode = SoundStimulus::SoundParameter::Tone;

	m_soundPtr->m_soundParameter.dur_ms = ui.durLineEdit->text().toInt();
	m_soundPtr->m_soundParameter.freq_kHz = ui.freqLineEdit->text().toInt();
	QDialog::accept();
}

void SoundSettingDialog::reject() {
	QDialog::reject();
}

void SoundSettingDialog::onComboBoxChange() {
	QString selectedItem = ui.modeComboBox->currentText();
	if (selectedItem == "BN")
		ui.freqLineEdit->setEnabled(false);
	else
		ui.freqLineEdit->setEnabled(true);
}
