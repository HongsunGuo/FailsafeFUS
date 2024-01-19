#pragma once

#include <QDialog>
#include <vector>
#include "ui_TaskEditor.h"

class Task;
class QListWidgetItem;
class TaskEditor : public QDialog
{
	Q_OBJECT

public:
	TaskEditor(std::shared_ptr<std::vector<Task>> ptr, QWidget* parent = nullptr);
	~TaskEditor();

private:
	Ui::TaskEditorClass ui;
	std::shared_ptr<std::vector<Task>> taskListPtr;
	void updateListWidget();
	void clearInputPanel();
private slots:
	void accept();
	void reject() {
		QDialog::reject();
	}
	void deleteATask();
	void addATask();
	void onListWidgetDoubleClicked(QListWidgetItem * item);
};

