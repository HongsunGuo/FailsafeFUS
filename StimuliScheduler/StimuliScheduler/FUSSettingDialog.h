#pragma once

#include <QDialog>
#include <memory>
#include "ui_FUSSettingDialog.h"
#include "FUSStimulus.h"


class FUSSettingDialog : public QDialog
{
	Q_OBJECT

public:
	FUSSettingDialog(std::shared_ptr<FUSStimulus> ptr, QWidget *parent = nullptr);
	~FUSSettingDialog();

private:
	Ui::FUSSettingDialogClass ui;
	std::shared_ptr<FUSStimulus> m_FUSPtr;

private slots:
	void accept() override;
	void reject() override;
	void onComboBoxChange();
};
