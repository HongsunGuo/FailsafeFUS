#pragma once

#include <QtWidgets/QDialog>
#include "ui_StimuliScheduler.h"
#include "Stimulus.h"
#include <vector>
#include <memory>
#include <algorithm>

class StimuliScheduler : public QDialog
{
    Q_OBJECT

public:
    StimuliScheduler(QWidget *parent = nullptr);
    ~StimuliScheduler();

private:
    Ui::StimuliSchedulerClass ui;
    std::vector<std::shared_ptr<Stimulus>> m_stimuli;
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
        std::shared_ptr<Stimulus> originalStimulus = m_stimuli[index];
        std::shared_ptr<Stimulus> deepCopyStimulus = std::make_shared<Stimulus>(*originalStimulus);
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

private slots:
    void onClickAddPushbutton();
    void onClickDeletePushbutton();
    void onClickDupPushbutton();
    void onClickMoveUpPushbutton();
    void onClickMoveDownPushbutton();
};
