#pragma once
#include "src/Propagator.h"

class RSO {

public:
    std::vector<State> state;
    Propagator propagator;

    RSO() {}

    void propagate(const State& initialState, const TimeArray& timeArray) {
        this->state = propagator.propagate(initialState, timeArray);
    }

};