#pragma once

#include <QDialog>
#include <memory>
#include "SoundStimulus.h"
#include "ui_SoundSettingDialog.h"


class SoundSettingDialog : public QDialog
{
	Q_OBJECT

public:
	SoundSettingDialog(std::shared_ptr<SoundStimulus> ptr, QWidget *parent = nullptr);
	~SoundSettingDialog();

private:
	Ui::SoundSettingDialogClass ui;
	std::shared_ptr<SoundStimulus> m_soundPtr;
private slots:
	void accept() override;
	void reject() override;
	void onComboBoxChange();
};
