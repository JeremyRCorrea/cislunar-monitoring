#pragma once
#include "Propagator.h"


class MoonOrbit : public Propagator {

public:
    MoonOrbit() {
        // Initialize parameters like gravitational constants
    }

    // Propagate initial state to the times in the time array
    std::vector<State> propagate(const State& initialState, const TimeArray& timeArray);

    // Compute derivative of state
    void computeDerivative(const State& X, State& dX, double t);

};

inline std::vector<State> MoonOrbit::propagate(const State& initial_state, const TimeArray& timeArray) {
    using namespace boost::numeric::odeint;

    // Dormand prince integrator with controlled stepping
    auto stepper = make_controlled(abs_tol, rel_tol, runge_kutta_dopri5<State>());

    // Create lambda function for recording the state at desired time steps
    std::vector<State> trajectory;
    auto observer = [&trajectory](const State& state, double t) {
        trajectory.push_back(state);
    };

    // 
    State x0 = initial_state;
    integrate_times(stepper, [this](const State& X, State& dX, double t) {
        computeDerivative(X, dX, t);
        }, x0, timeArray.val.begin(), timeArray.val.end(), timeArray.step, observer);

    return trajectory;
}




inline void MoonOrbit::computeDerivative(const State& X, State& dX, double t) {

    // Compute contributions from moon
    double3 R_tm = double3{ X[0], X[1], X[2] };
    double r3_tm = computeR3(R_tm);
    double3 g_m = -mu_Moon * (R_tm / r3_tm);

    // State derivative
    dX = State{
        X[3],
        X[4],
        X[5],
        g_m[0],
        g_m[1],
        g_m[2]
    };
}



