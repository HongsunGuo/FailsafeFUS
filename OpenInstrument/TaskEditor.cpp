#include <QMessageBox>
#include "TaskEditor.h"
#include "Task.h"

using namespace std;

TaskEditor::TaskEditor(std::shared_ptr<std::vector<Task>> ptr, QWidget *parent)
	:taskListPtr(ptr), QDialog(parent)
{
	ui.setupUi(this);
	updateListWidget();
	connect(ui.okPushButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui.cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(ui.deletePushButton, SIGNAL(clicked()), this, SLOT(deleteATask()));
	connect(ui.addPushButton, SIGNAL(clicked()), this, SLOT(addATask()));
	connect(ui.taskListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onListWidgetDoubleClicked(QListWidgetItem*)));
}

TaskEditor::~TaskEditor()
{}


void TaskEditor::accept() {

	QDialog::accept();
}

void TaskEditor::addATask() {
	string newShortName = ui.shortnameLineEdit->text().toStdString();
	for (const auto &cur: *taskListPtr) {
		if (cur.shortName == newShortName){
			QMessageBox::warning(nullptr, "No add", "Duplicated short name.");
			return;
		}
	}

	Task newTask(ui.shortnameLineEdit->text().toStdString(),
		ui.serialPortComboBox->currentText().toStdString(),
		ui.descriptionPlainTextEdit->toPlainText().toStdString(),
		ui.commandLineEdit->text().toStdString());
	taskListPtr->push_back(newTask);
	updateListWidget();
}

void TaskEditor::deleteATask() {
	int index = ui.taskListWidget->currentRow();
	if (index < 0) {
		QMessageBox::warning(nullptr, "No delete", "No item selected.");
		return;
	}
	
	taskListPtr->erase(taskListPtr->begin() + index);
	updateListWidget();
	clearInputPanel();
}

void TaskEditor::onListWidgetDoubleClicked(QListWidgetItem* item)
{
	int index = item->listWidget()->row(item);
	if (index < 0 || index > taskListPtr->size())
		return;
	
	ui.commandLineEdit->setText(QString::fromStdString((*taskListPtr)[index].command));
	ui.descriptionPlainTextEdit->setPlainText(QString::fromStdString((*taskListPtr)[index].description));
	ui.shortnameLineEdit->setText(QString::fromStdString((*taskListPtr)[index].shortName));
	ui.serialPortComboBox->setCurrentText(QString::fromStdString((*taskListPtr)[index].serialPortName));
}

void TaskEditor::updateListWidget() {
	ui.taskListWidget->clear();
	for (const auto& cur : *taskListPtr) {
		ui.taskListWidget->addItem(QString::fromStdString(cur.shortName));
	}
}

void TaskEditor::clearInputPanel() {
	ui.shortnameLineEdit->setText("");
	ui.descriptionPlainTextEdit->setPlainText("");
	ui.commandLineEdit->setText("");
	ui.serialPortComboBox->setCurrentIndex(0);
}