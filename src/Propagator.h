#pragma once
#include <boost/numeric/odeint.hpp>
#include "util/Cartesian.h"
#include "util/TimeArray.h"

// Gravitational constants
const double mu_Earth = 398600.4418; // km3/s2
const double mu_Moon = 4902.8695; // km3/s2

// Mean distance to moon and approx lunar period
const double d_moon = 384400; // km
const double T_moon = 27.32 * 24 * 60 * 60; // seconds

// Tolerances for numerical integrator
const double abs_tol = 1e-10;  // Adjust absolute tolerance
const double rel_tol = 1e-10;  // Adjust relative tolerance

class Propagator {

public:
    Propagator() {}

    // Propagate initial state to the times in the time array
    std::vector<State> propagate(const State& initialState, const TimeArray& timeArray);

    // Compute derivative of state
    void computeDerivative(const State& X, State& dX, double t);

};

inline std::vector<State> Propagator::propagate(const State& initial_state, const TimeArray& timeArray) {
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


double computeR3(const double3& r);
double3 calculateMoonPosition(double t);

inline void Propagator::computeDerivative(const State& X, State& dX, double t) {

    // Compute position vectors
    double3 R_tm = double3{ X[0], X[1], X[2] };
    double3 R_me = calculateMoonPosition(t);
    double3 R_te = R_tm + R_me;

    // Compute divisors
    double r3_tm = computeR3(R_tm);
    double r3_te = computeR3(R_te);
    double r3_me = computeR3(R_me);

    // Calculate gravitational acceleration due to the Moon
    double3 g_m = - mu_Moon * (R_tm / r3_tm);

    // Calculate gravitational acceleartion due to the Earth
    double3 g_e = - mu_Earth * (R_te / r3_te) + mu_Earth * (R_me / r3_me);

    // State derivative
    dX = State{
        X[3],
        X[4],
        X[5],
        g_e[0] + g_m[0],
        g_e[1] + g_m[1],
        g_e[2] + g_m[2]
    };
}

double computeR3(const double3& r) {
    const double rn = std::sqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2]);
    return rn * rn * rn;
}

double3 calculateMoonPosition(double t) {

    double omega = 2 * 3.14 / T_moon;

    // Simple model of the moon's orbit
    double3 r_moon = {
        d_moon * cos(omega * t),
        d_moon * sin(omega * t),
        0.0
    };

    return r_moon;
}