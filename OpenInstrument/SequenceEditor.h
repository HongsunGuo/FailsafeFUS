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
	SequenceEditor(std::shared_ptr<std::vector<SequenceListItem>> seqListPtr = nullptr,
		int curSeqID = 0,
		QWidget* parent = nullptr);
	~SequenceEditor();

private:
	Ui::SequenceEditorClass ui;
	std::shared_ptr<std::vector<SequenceListItem>> m_seqListPtr;
	int m_curSeqID = 0;
};
