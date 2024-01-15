#pragma once
#include "Stimulus.h"

class FUSStimulus :
    public Stimulus
{
public:
    class FUSParameter {
    public:
        FUSParameter() {}
        enum FUSMode {Pulsed, Continuous};
        FUSMode mode = Continuous;
        int dur_ms = 80;
        int pressure_kPa = 400;
        double cFreq_MHz = 0.916; //center freq
        int n_pulses = 1;
        int PRF_kHz = 1;
        int duty_cycle = 0.5; //for Pulsed mode only
    };

    FUSStimulus(FUSParameter para = FUSParameter()) :
        m_fusParameter(para) {}

    std::string toString() override {
        std::string res = "Ultrasound stimulus: ";
        if (m_fusParameter.mode == FUSParameter::Pulsed) {
            res += "mode = pulsed, ";
            res += "center freq (MHz) = ";
            res += std::to_string(m_fusParameter.cFreq_MHz);
            res += " , Pulse duration (ms) = ";
            res += std::to_string(m_fusParameter.dur_ms);
        }
        else {
            res += "mode = continuous, ";
            res += "center freq (MHz) = ";
            res += std::to_string(m_fusParameter.cFreq_MHz);
            res += " , Pressure (kPa) = ";
            res += std::to_string(m_fusParameter.pressure_kPa);
            res += " , Pulse duration (ms) = ";
            res += std::to_string(m_fusParameter.dur_ms);
        }
        return res; }
private:
    FUSParameter m_fusParameter;
};

