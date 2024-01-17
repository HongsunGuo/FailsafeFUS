#pragma once

#include <QtWidgets/QDialog>
#include "ui_StimuliScheduler.h"
#include "Stimulus.h"
#include "VibrationStimulus.h"
#include "FlashingLEDStimulus.h"
#include "SpontStimulus.h"
#include "FUSStimulus.h"
#include "SoundStimulus.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <QTimer>
#include <QListWidgetItem>

class StimuliScheduler : public QDialog
{
    Q_OBJECT

public:
    StimuliScheduler(QWidget *parent = nullptr);
    ~StimuliScheduler();
private:
    class Setting {
    public:
        Setting(){}
        void reset() {
            curBlockID = 0;
            curStimID = 0;
            order.clear();
        }
        int nBlocks = 5;
        int ISIsec = 20;
        int curBlockID = 0;
        int curStimID = 0;
        bool random = true;
        std::vector<std::vector<int>> order = {};
    };

    Setting m_setting;
    Ui::StimuliSchedulerClass ui;
    std::vector<std::shared_ptr<Stimulus>> m_stimuli;
    QTimer m_timer;
    void addStimulus(std::shared_ptr<Stimulus> newStimulus) {
        m_stimuli.push_back(newStimulus);
    }
    void deleteStimulus(int index) {
        if (index < 0 || index >= m_stimuli.size())
            return;
        //index start with 0
        m_stimuli.erase(m_stimuli.begin() + index);
    }

    void duplicateStimulus(int index) {
        if (index < 0 || index >= m_stimuli.size())
            return;
        std::shared_ptr<Stimulus> basePtr = m_stimuli[index];
        std::shared_ptr<Stimulus> deepCopyStimulus;
        const std::type_info& typeInfo = typeid(*basePtr);
        if (typeInfo == typeid(VibrationStimulus))
            deepCopyStimulus = std::make_shared<VibrationStimulus>(*std::dynamic_pointer_cast<VibrationStimulus>(basePtr));
        else if (typeInfo == typeid(SoundStimulus))
            deepCopyStimulus = std::make_shared<SoundStimulus>(*std::dynamic_pointer_cast<SoundStimulus>(basePtr));
        else if (typeInfo == typeid(SpontStimulus))
            deepCopyStimulus = std::make_shared<SpontStimulus>(*std::dynamic_pointer_cast<SpontStimulus>(basePtr));
        else if (typeInfo == typeid(FlashingLEDStimulus))
            deepCopyStimulus = std::make_shared<FlashingLEDStimulus>(*std::dynamic_pointer_cast<FlashingLEDStimulus>(basePtr));
        else if (typeInfo == typeid(FUSStimulus))
            deepCopyStimulus = std::make_shared<FUSStimulus>(*std::dynamic_pointer_cast<FUSStimulus>(basePtr));
        else
            deepCopyStimulus = std::make_shared<Stimulus>(*basePtr);
        m_stimuli.push_back(deepCopyStimulus);
    }

    void moveUpStimulus(int i) {
        if (i == 0)
            return;

        std::swap(m_stimuli[i], m_stimuli[i - 1]);
    }

    void moveDownStimulus(int i) {
        if (i == m_stimuli.size() - 1)
            return;

        std::swap(m_stimuli[i], m_stimuli[i + 1]);
    }
    void updateBlockEditor();
    void updateCurBlockView(const std::vector<int>& curBlock, int curStimID);
    void setStatus(const QString& status) {
        ui.statusLabel->setText(status);
    }

private slots:
    void onClickAddPushbutton();
    void onClickDeletePushbutton();
    void onClickDupPushbutton();
    void onClickMoveUpPushbutton();
    void onClickMoveDownPushbutton();
    void onClickRunPushbutton();
    void updateSetting();
    void timeoutCallback();
    void onItemDoubleClicked(QListWidgetItem* item);
};
