#include <QMessageBox>
#include "ui_SelectStimulusDialog.h"
#include "StimuliScheduler.h"

#define CHECK_ITEM_SELECTED do { \
    int index = ui.blockListWidget->currentRow(); \
    if (index < 0) { \
        QMessageBox::warning(nullptr, "No selection", "No stimulus item is selected."); \
        return; \
    }\
 } while(0)

using namespace std;

StimuliScheduler::StimuliScheduler(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.addPushButton, SIGNAL(clicked()), this, SLOT(onClickAddPushbutton()));
    connect(ui.deletePushButton, SIGNAL(clicked()), this, SLOT(onClickDeletePushbutton()));
    connect(ui.dupPushButton, SIGNAL(clicked()), this, SLOT(onClickDupPushbutton()));
    connect(ui.moveUpPushButton, SIGNAL(clicked()), this, SLOT(onClickMoveUpPushbutton()));
    connect(ui.moveDownPushButton, SIGNAL(clicked()), this, SLOT(onClickMoveDownPushbutton()));
}

StimuliScheduler::~StimuliScheduler()
{

}

void StimuliScheduler::onClickAddPushbutton()
{
    QDialog dialog;
    Ui::SelectStimulusDialog ui_dialog;
    ui_dialog.setupUi(&dialog);
    int result = dialog.exec();
    if (result == QDialog::Rejected)
        return;

    QString stimulusType = ui_dialog.comboBox->currentText();
    shared_ptr<Stimulus> newStimulusPtr;
    if (stimulusType == "Spontaneous")
        newStimulusPtr = make_shared<SpontStimulus>();
    else if (stimulusType == "Visual")
        newStimulusPtr = make_shared<FlashingLEDStimulus>();
    else if (stimulusType == "Somatosensory")
        newStimulusPtr = make_shared<VibrationStimulus>();
    else if (stimulusType == "Ultrasound")
        newStimulusPtr = make_shared<FUSStimulus>();
    else if (stimulusType == "Sound")
        newStimulusPtr = make_shared<SoundStimulus>();
    else
    return;

    addStimulus(newStimulusPtr);
    updateBlockEditor();
}

void StimuliScheduler::updateBlockEditor()
{
    ui.blockListWidget->clear();
    int i = 1;
    for (auto& ptr : m_stimuli) {
        ui.blockListWidget->addItem(QString::number(i++) + ": " +
            QString::fromStdString(ptr->toString()));
    }
}

void StimuliScheduler::onClickDeletePushbutton() {
    CHECK_ITEM_SELECTED;
    deleteStimulus(ui.blockListWidget->currentRow());
    updateBlockEditor();
}

void StimuliScheduler::onClickDupPushbutton() {
    CHECK_ITEM_SELECTED;
    duplicateStimulus(ui.blockListWidget->currentRow());
    updateBlockEditor();
}

void StimuliScheduler::onClickMoveUpPushbutton() {
    CHECK_ITEM_SELECTED;
    moveUpStimulus(ui.blockListWidget->currentRow());
    updateBlockEditor();
}

void StimuliScheduler::onClickMoveDownPushbutton() {
    CHECK_ITEM_SELECTED;
    moveUpStimulus(ui.blockListWidget->currentRow());
    updateBlockEditor();
}
