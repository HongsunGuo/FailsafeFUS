#pragma once
#include "Stimulus.h"
class SoundStimulus :
    public Stimulus
{
public:
    class SoundParameter {
    public:
        SoundParameter(){}
        enum SoundMode {BN, Tone};
        SoundMode mode = BN;
        int dur_ms = 80;
        double freq_kHz = 1; //for Tone mode only
    };

    SoundStimulus(SoundParameter para = SoundParameter()):
        m_soundParameter(para){}

    std::string toString() override {
        std::string res = "Audible sound stimulus: ";
        if (m_soundParameter.mode = SoundParameter::BN) {
            res += "mode = BN";
            res += ", Duration (ms) = ";
            res += std::to_string(m_soundParameter.dur_ms);
        }
        else {
            res += "mode = Tone";
            res += ", Duration (ms) = ";
            res += std::to_string(m_soundParameter.dur_ms);
            res += ", Freq (kHz) = ";
            res += std::to_string(m_soundParameter.freq_kHz);
        }
        return res;
    }

private:
    SoundParameter m_soundParameter;
};

