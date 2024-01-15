#pragma once
#include "Stimulus.h"
#include <string>

class FlashingLEDStimulus :
    public Stimulus
{
public:
    FlashingLEDStimulus(int durationMS = 80)
        :m_dur_msec(durationMS){}
    void setDur(int durationMS) { m_dur_msec = durationMS; }
    std::string toString() override {
        return "Visual stimulus (flashing LED), duration = " +
            std::to_string(m_dur_msec) + " ms";
    }
private:
    int m_dur_msec;
};
