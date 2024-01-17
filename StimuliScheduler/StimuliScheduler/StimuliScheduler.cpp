#include <QMessageBox>
#include <QItemSelectionModel>
#include <random>
#include "ui_SelectStimulusDialog.h"
#include "StimuliScheduler.h"
#include "FUSSettingDialog.h"
#include "SoundSettingDialog.h"

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
    ui.progressBar->setValue(0);
    ui.ISILineEdit->setText(QString::number(m_setting.ISIsec));
    ui.nBlockLineEdit->setText(QString::number(m_setting.nBlocks));
    ui.randomCheckBox->setChecked(m_setting.random);
    ui.curBlockListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.curBlockListWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui.curBlockListWidget->setEnabled(false);
    connect(ui.addPushButton, SIGNAL(clicked()), this, SLOT(onClickAddPushbutton()));
    connect(ui.deletePushButton, SIGNAL(clicked()), this, SLOT(onClickDeletePushbutton()));
    connect(ui.dupPushButton, SIGNAL(clicked()), this, SLOT(onClickDupPushbutton()));
    connect(ui.moveUpPushButton, SIGNAL(clicked()), this, SLOT(onClickMoveUpPushbutton()));
    connect(ui.moveDownPushButton, SIGNAL(clicked()), this, SLOT(onClickMoveDownPushbutton()));
    connect(ui.runPushButton, SIGNAL(clicked()), this, SLOT(onClickRunPushbutton()));
    connect(ui.blockListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onItemDoubleClicked(QListWidgetItem*)));
    connect(ui.nBlockLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(updateSetting()));
    connect(ui.ISILineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(updateSetting()));
    connect(ui.randomCheckBox, SIGNAL(toggled(bool)), this, SLOT(updateSetting()));
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeoutCallback()));
}

StimuliScheduler::~StimuliScheduler()
{

}

void StimuliScheduler::onClickAddPushbutton()
{
    QDialog dialog;
    Ui::SelectStimulusDialog ui_dialog;
    ui_dialog.setupUi(&dialog);
    dialog.setWindowTitle("Select stimulus type:");
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

void StimuliScheduler::onClickRunPushbutton() {
    if (m_stimuli.empty() || m_setting.nBlocks <=0)
        return;
    
    m_setting.reset();
    vector<int> originalOrder = {};
    for (int i = 0; i < m_stimuli.size(); i++)
        originalOrder.push_back(i);
    random_device rd;
    mt19937 rng(rd());
    for (int i = 0; i < m_setting.nBlocks; i++) {
        if(m_setting.random)
            shuffle(originalOrder.begin(), originalOrder.end(), rng);
        m_setting.order.push_back(originalOrder);
    }
    ui.progressBar->setRange(0, m_setting.nBlocks * m_stimuli.size());
    ui.progressBar->setValue(0);
    m_timer.setInterval(m_setting.ISIsec * 1000);
    m_timer.start();
 }

void StimuliScheduler::updateSetting() {
    m_setting.ISIsec = ui.ISILineEdit->text().toInt();
    m_setting.nBlocks = ui.nBlockLineEdit->text().toInt();
    m_setting.random = ui.randomCheckBox->isChecked();
    m_setting.order.clear();
}

void StimuliScheduler::timeoutCallback() {
    updateCurBlockView(m_setting.order[m_setting.curBlockID], m_setting.curStimID);
    ui.progressBar->setValue(m_setting.curBlockID * m_stimuli.size() + m_setting.curStimID + 1);
    setStatus("Current block id = " + QString::number(m_setting.curBlockID + 1)
        + ", current stimulus id = " + QString::number(m_setting.curStimID + 1));
    if (++m_setting.curStimID == m_stimuli.size()) {
        if (++m_setting.curBlockID == m_setting.nBlocks) {
            QMessageBox::information(this, "Operation Successful", "The session was completed successfully!");
            m_timer.stop();
            setStatus("Done!");
        }
        else {
            m_setting.curStimID = 0;
        }
    }
}

void StimuliScheduler::updateCurBlockView(const vector<int> &curBlock, int curStimID)
{
    ui.curBlockListWidget->clear();
    int i = 1;
    for (auto& id : curBlock) {
        ui.curBlockListWidget->addItem(QString::number(i++) + ": " +
            QString::fromStdString(m_stimuli[id]->toString()));
    }
    ui.curBlockListWidget->setCurrentRow(curStimID);
}

void StimuliScheduler::onItemDoubleClicked(QListWidgetItem* item) {
    if (item == nullptr)
        return;

    int doubleClickedIndex = ui.blockListWidget->row(item);
    shared_ptr<Stimulus> stimPtr = m_stimuli[doubleClickedIndex];
    const std::type_info& typeInfo = typeid(*stimPtr);
    if (typeInfo == typeid(FUSStimulus)) {
        shared_ptr<FUSStimulus> fUSPtr = dynamic_pointer_cast<FUSStimulus>(stimPtr);
        FUSSettingDialog fUSDialog(fUSPtr);
        if (QDialog::Accepted == fUSDialog.exec())
            updateBlockEditor();
    }else if (typeInfo == typeid(SoundStimulus)) {
        shared_ptr<SoundStimulus> soundPtr = dynamic_pointer_cast<SoundStimulus>(stimPtr);
        SoundSettingDialog soundDialog(soundPtr);
        if (QDialog::Accepted == soundDialog.exec())
            updateBlockEditor();
    }
    else {
        ;
    }
}
