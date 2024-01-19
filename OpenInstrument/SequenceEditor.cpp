#include <QMessageBox>
#include "SequenceEditor.h"
#include "Task.h"
#include "ui_NewSeqListDialog.h"
#include "ui_AddTaskToSequenceDialog.h"

using namespace std;

SequenceEditor::SequenceEditor(shared_ptr<vector<SequenceListItem>> seqListPtr,
	shared_ptr<vector<Task>> taskListPtr,
	int selectedSeqID, QWidget *parent)
	:m_seqListPtr(seqListPtr), m_taskListPtr(taskListPtr), m_selectedSeqID(selectedSeqID), QDialog(parent)
{
	ui.setupUi(this);
	updateSeqListWidget();

	connect(ui.addSeqListButton, SIGNAL(clicked()), this, SLOT(onAddSeqListButtonClicked()));
	connect(ui.deleteSeqListButton, SIGNAL(clicked()), this, SLOT(onDeleteSeqListButtonClicked()));
	connect(ui.selectSeqListButton, SIGNAL(clicked()), this, SLOT(onSelectSeqListButton()));

	connect(ui.addSeqButton, SIGNAL(clicked()), this, SLOT(onAddSeqButtonClicked()));
	connect(ui.deleteSeqButton, SIGNAL(clicked()), this, SLOT(onDeleteSeqButtonClicked()));
	connect(ui.moveupSeqButton, SIGNAL(clicked()), this, SLOT(onMoveupButtonClicked()));
	connect(ui.movedownSeqButton, SIGNAL(clicked()), this, SLOT(onMovedownButtonClicked()));
	connect(ui.seqListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onListWidgetDoubleClicked(QListWidgetItem*)));
	connect(ui.okPushButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui.cancelPushbutton, SIGNAL(clicked()), this, SLOT(reject()));
}

SequenceEditor::~SequenceEditor()
{}

void SequenceEditor::onAddSeqListButtonClicked() {
	QDialog dialog;
	Ui::NewSeqListDialog ui_dialog;
	ui_dialog.setupUi(&dialog);
	int result = dialog.exec();
	if (result == QDialog::Rejected)
		return;

	string seqShortName = ui_dialog.seqListShortNameLineEdit->text().toStdString();
	for (const auto& p : *m_seqListPtr) {
		if (p.shortName == seqShortName) {
			QMessageBox::warning(nullptr, "No add", "Duplicated sequence short name.");
			return;
		}
	}

	SequenceListItem newItem(seqShortName);
	m_seqListPtr->push_back(newItem);
	updateSeqListWidget();
}

void SequenceEditor::onDeleteSeqListButtonClicked() {
	int index = ui.seqListWidget->currentRow();
	if (index < 0) {
		QMessageBox::warning(nullptr, "No selection", "No sequence list item is selected.");
		return;
	}
	m_seqListPtr->erase(m_seqListPtr->begin() + index);
	updateSeqListWidget();
}

void SequenceEditor::onSelectSeqListButton() {
	int index = ui.seqListWidget->currentRow();
	if (index < 0) {
		QMessageBox::warning(nullptr, "No selection", "No sequence list item is selected.");
			return;
	}

	m_selectedSeqID = index;
	updateSeqListWidget();
}

void SequenceEditor::updateSeqListWidget() {
	ui.seqListWidget->clear();
	for (int i = 0; i < m_seqListPtr->size(); i++) {
		QString str = "";
		if (i == m_selectedSeqID) {
			str = "#Selected seq#: ";
		}

		ui.seqListWidget->addItem(str + QString::fromStdString((*m_seqListPtr)[i].shortName));
	}
}

void SequenceEditor::onAddSeqButtonClicked() {
	if (m_curEditSeqID < 0) {
		QMessageBox::warning(nullptr, "No active seq item", "Please double click a seq item.");
		return;
	}

	QDialog dialog;
	Ui::AddTaskToSeqDialog ui_dialog;
	ui_dialog.setupUi(&dialog);
	//
	for (const auto& t : (*m_taskListPtr)) {
		ui_dialog.tasksComboBox->addItem(QString::fromStdString(t.shortName));
	}
	
	int result = dialog.exec();
	if (result == QDialog::Rejected)
		return;

	SequenceItem newItem(ui_dialog.tasksComboBox->currentText().toStdString(),
		ui_dialog.delayLineEdit->text().toInt());
	(*m_seqListPtr)[m_curEditSeqID].sequenceList.push_back(newItem);
	updateCurSeqEditListWidget();
}

void SequenceEditor::updateCurSeqEditListWidget() {
	ui.curSeqEditlistWidget->clear();
	int i = 0;
	for (const auto& cur : (*m_seqListPtr)[m_curEditSeqID].sequenceList) {
		ui.curSeqEditlistWidget->addItem(QString::number(++i) + ": " +
			QString::fromStdString(cur.taskShortName) + ",\t delay(sec) = " +
			QString::number(cur.delaySec));
	}
}

void SequenceEditor::onDeleteSeqButtonClicked() {
	int index = ui.curSeqEditlistWidget->currentRow();
	if (index < 0) {
		QMessageBox::warning(nullptr, "No selection", "No item is selected.");
		return;
	}

	(*m_seqListPtr)[m_curEditSeqID].sequenceList.erase((*m_seqListPtr)[m_curEditSeqID].sequenceList.begin() + index);
}

void SequenceEditor::onMoveupButtonClicked() {

}

void SequenceEditor::onMovedownButtonClicked() {

}

void SequenceEditor::onListWidgetDoubleClicked(QListWidgetItem* item)
{
	int index = item->listWidget()->row(item);
	if (index < 0 || index > m_seqListPtr->size())
		return;

	m_curEditSeqID = index;
	updateCurSeqEditListWidget();
}
