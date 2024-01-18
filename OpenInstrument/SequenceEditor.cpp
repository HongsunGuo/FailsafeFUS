#include "SequenceEditor.h"
#include "Task.h"

using namespace std;

SequenceEditor::SequenceEditor(shared_ptr<vector<SequenceListItem>> seqListPtr,
	int curSeqID, QWidget *parent)
	:m_seqListPtr(seqListPtr), m_curSeqID(curSeqID), QDialog(parent)
{
	ui.setupUi(this);
}

SequenceEditor::~SequenceEditor()
{}
