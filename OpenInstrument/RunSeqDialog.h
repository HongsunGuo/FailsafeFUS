#pragma once
#include <memory>
#include <vector>
#include <QDialog>
#include <QTimer>
#include "ui_RunSeqDialog.h"

class SequenceListItem;
class Task;
class QTimer;
class RunSeqDialog : public QDialog
{
	Q_OBJECT

public:
	RunSeqDialog(std::shared_ptr<std::vector<Task>> taskListPtr,
		const SequenceListItem& m_selectedSeq, QWidget *parent = nullptr);
	~RunSeqDialog();
	void start();
private:
	Ui::RunSeqDialogClass ui;
	const SequenceListItem& m_selectedSeq;
	std::shared_ptr<std::vector<Task>> m_taskListPtr;
	QTimer m_timer;
	int m_curSeqItemID = 0;
	void updateSeqListWidget();
private slots:
	void onTimeout();
	void onFinishPushButtonClicked();
};
