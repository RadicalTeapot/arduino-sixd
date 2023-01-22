#include "lfo.h"
#include <math.h>

float Lfo::Process(float delta) {
    phase_ += delta * frequency_;
    phase_ = phase_ - static_cast<int>(phase_);     // Keep only fractional part (delta is assumed to be positive)

    return 2 * amplitude_ * fabs((phase_ + phaseShift_) - floor((phase_ + phaseShift_)+0.5f));
}
