#pragma once
#ifndef LFO_H
#define LFO_H

#include <math.h>

class Lfo
{
public:

    void Init(float frequency) {
        frequency_ = frequency;
        phaseShift_ = 0;
        amplitude_ = 1;
    }

    void SetFrequency(float frequency) {
        // TODO Phase should be recalculated here
        frequency_ = frequency;
    }

    void SetPhaseShift(float phaseShift) {
        phaseShift_ = phaseShift;
    }

    void SetAmplitude(float amplitude) {
        amplitude_ = fmin(fmax(amplitude, 0), 1);
    }

    float Process(float delta);
private:
    float frequency_;
    float phase_;
    float phaseShift_;
    float amplitude_;
};

#endif
