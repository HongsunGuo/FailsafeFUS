#pragma once
#include "Stimulus.h"
class SpontStimulus :
    public Stimulus
{
public:
    SpontStimulus(){}
    std::string toString() override {
        return "Spontaneous  stimulus (Do nothing...)";
    }
};

