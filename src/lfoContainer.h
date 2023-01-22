#pragma once
#ifndef LFOCONTAINER_H
#define LFOCONTAINER_H

#include "lfo.h"

namespace LfoContainer
{
    const int kLfoCount = 6;
    const float kLfoFrequencyRatios[kLfoCount] = {
        1,        // Unison
        0.42045f, // 15 semitones down (2^(-15/12))
        0.16685f, // 31 semitones down (2^(-31/12))
        0.0625f,  // 48 semitones down (2^(-48/12))
        0.04171f, // 55 semitones down (2^(-55/12))
        0.00984f, // 80 semitones down (2^(-80/12))
    };
    const float kLfoFrequencyBounds[2] = {
        0.06773f, // ~15 sec
        100.0f    // 0.01 sec
    };
    const float kFrequencyScalerOnePoleCoeff = 0.2f;

    class LfoContainer
    {
    public:
        void Init()
        {
            frequencyScaler_ = kLfoFrequencyBounds[0];
            UpdateLfoFrequencies();
            for (i_ = 0; i_ < kLfoCount; i_++)
            {
                lfos_[i_].Init(kLfoFrequencyRatios[i_] * frequencyScaler_);
                processValues_[i_] = 0;
            }
        }

        void Process(float delta)
        {
            for (i_ = 0; i_ < kLfoCount; i_++)
                processValues_[i_] = lfos_[i_].Process(delta);
        }

        void SetFrequencyScaler(float scaler)
        {
            scaler = fmin(fmax(scaler, 0), 1);
            float remaped = kLfoFrequencyBounds[0] + (scaler * scaler) * (kLfoFrequencyBounds[1] - kLfoFrequencyBounds[0]);
            frequencyScaler_ += kFrequencyScalerOnePoleCoeff * (remaped - frequencyScaler_);
            UpdateLfoFrequencies();
        }

        float Get(int index)
        {
            return processValues_[index];
        }

    private:
        Lfo lfos_[kLfoCount];
        float processValues_[6];
        int i_;
        float frequencyScaler_;

        void UpdateLfoFrequencies()
        {
            for (i_ = 0; i_ < kLfoCount; i_++)
                lfos_[i_].SetFrequency(kLfoFrequencyRatios[i_] * frequencyScaler_);
        }
    };
}

#endif
