#pragma once

#include <QDialog>
#include "ui_SequenceEditor.h"
#include <memory>

class Task;
class SequenceItem;
class SequenceListItem;

class SequenceEditor : public QDialog
{
	Q_OBJECT

public:
	SequenceEditor(std::shared_ptr<std::vector<SequenceListItem>> = nullptr,
		std::shared_ptr<std::vector<Task>> taskListPtr = nullptr,
		int selectedSeqID = 0,
		QWidget* parent = nullptr);
	~SequenceEditor();

	int getSelectedSeqList() { return m_selectedSeqID;  }
private:
	Ui::SequenceEditorClass ui;
	std::shared_ptr<std::vector<SequenceListItem>> m_seqListPtr;
	std::shared_ptr<std::vector<Task>> m_taskListPtr;
	int m_selectedSeqID = 0;
	int m_curEditSeqID = -1;
	void updateSeqListWidget();
	void updateCurSeqEditListWidget();
private slots:
	void onAddSeqListButtonClicked();
	void onDeleteSeqListButtonClicked();
	void onSelectSeqListButton();
	void reject() { QDialog::reject(); }
	void accept() { QDialog::accept(); }
	void onAddSeqButtonClicked();
	void onDeleteSeqButtonClicked();
	void onMoveupButtonClicked();
	void onMovedownButtonClicked();
	void onListWidgetDoubleClicked(QListWidgetItem* item);
};
