#pragma once
#include "Stimulus.h"
class VibrationStimulus :
    public Stimulus
{
public:
    VibrationStimulus(int durationMS = 80): m_dur_msec(durationMS){}
    void setDur(int durationMS) { m_dur_msec = durationMS; }
    std::string toString() override {
        return "Somatosensory stimulus (vibrating device), duration = " +
            std::to_string(m_dur_msec) + " ms";
    }
private:
    int m_dur_msec;
};

