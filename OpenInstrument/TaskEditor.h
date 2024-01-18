#pragma once

#include <QDialog>
#include "ui_TaskEditor.h"

class TaskEditor : public QDialog
{
	Q_OBJECT

public:
	TaskEditor(QWidget *parent = nullptr);
	~TaskEditor();

private:
	Ui::TaskEditorClass ui;
};
