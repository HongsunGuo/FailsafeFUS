#include <QMessageBox>
#include "RunSeqDialog.h"
#include "Task.h"

using namespace std;
RunSeqDialog::RunSeqDialog(shared_ptr<vector<Task>> taskListPtr, const SequenceListItem& seq, QWidget *parent)
	:m_taskListPtr(taskListPtr), m_selectedSeq(seq), QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.finishPushButton, SIGNAL(clicked()), this, SLOT(onFinishPushButtonClicked()));
	updateSeqListWidget();
	ui.seqListWidget->setEnabled(false);
}

RunSeqDialog::~RunSeqDialog()
{}

void RunSeqDialog::updateSeqListWidget() {
	int i = 0;
	for (const auto& sItem: m_selectedSeq.sequenceList) {
		ui.seqListWidget->addItem(QString::number(++i) + ": " +
			QString::fromStdString(sItem.taskShortName) + ",\t delay(sec) = " +
			QString::number(sItem.delaySec));
	}
}

void RunSeqDialog::start() {
	if (m_selectedSeq.sequenceList.size() == 0)
		return;
	QTimer::singleShot(500, this, SLOT(onTimeout()));
}

void RunSeqDialog::onTimeout() {
	if (m_curSeqItemID == m_selectedSeq.sequenceList.size()) {
		//Done!
		QMessageBox::information(nullptr, "", "Completed!");
		return;
	}

	ui.seqListWidget->setCurrentRow(m_curSeqItemID);
	QTimer::singleShot(1000 * m_selectedSeq.sequenceList[m_curSeqItemID].delaySec, this, SLOT(onTimeout()));
	m_curSeqItemID++;
}

void RunSeqDialog::onFinishPushButtonClicked() {
	if (m_curSeqItemID != m_selectedSeq.sequenceList.size()) {
		QMessageBox::warning(nullptr, "", "Not finished!");
		return;
	}

	QDialog::accept();
}
