#include <iostream>
#include <fstream>
#include "src/Propagator.h"
#include "src/MoonOrbit.h"

#include "util/rv2coe.h"


#define pi 3.14159265358979323846

std::vector<OE> computeOrbitalElements(const std::vector<State>& traj, const double mu) {

    std::vector<OE> orbital_elements(0);

    for (const auto& state : traj) {
        OE oe = rv2coe(mu, state);
        orbital_elements.push_back(oe);
    }

    return orbital_elements;
}


int main() {


    // ------------------------------------------------------------------
    //                         SIMULATION INPUTS
    // ------------------------------------------------------------------
    const double t0 = 0.0;
    const double tf = 24 * 60 * 60;
    const double step = 1;

    const double  sensorRangeThreshold = 2000.0;
    const double  sensorFov = 40.0 * (pi / 180);
    const double3 sensorBoresight = double3{ 0.0, 0.0, -1.0 };
    const State   sensorState = {
        0.0, 0.0, -1736.0,         // Position [km]
        0.0, 0.0, 0.0              // Velocity [km/s]
    };

    const State targetState = {
        325.59, 446.71, 1791.67,   // Position [km]
         -1.78,   0.08,    0.30    // Velocity [km/s]
    };


    // ------------------------------------------------------------------
    //                         SET UP SCENARIO
    // ------------------------------------------------------------------

    // Instantiate object
    TimeArray timeArray(t0, tf, step);


    MoonOrbit moonOrbit;
    auto moon_traj = moonOrbit.propagate(targetState, timeArray);



    Propagator propagator;
    auto trajectory = propagator.propagate(targetState, timeArray);

    // ------------------------------------------------------------------
    //                         RESULTS
    // ------------------------------------------------------------------


    std::vector<double> time_vector_days(0);
    for (double t = t0; t <= tf; t += step) {
        time_vector_days.push_back(t / 86400.0);
    }

    std::vector<double> time_vector_hours(0);
    for (double t = t0; t <= tf; t += step) {
        time_vector_hours.push_back(t / 3600.0);
    }



    std::ofstream TwoBdyEphem("TwoBodyEphemeris.dat");
    std::ofstream ThreeBdyEphem("ThreeBodyEphemeris.dat");

    for (size_t k = 0; k < trajectory.size(); k++) {
        // Writing data for MoonOrbit
        TwoBdyEphem << time_vector_days[k] << " " << moon_traj[k][0] << " " << moon_traj[k][1] << " " << moon_traj[k][2] << "\n";

        // Writing data for Propagator
        ThreeBdyEphem << time_vector_days[k] << " " << trajectory[k][0] << " " << trajectory[k][1] << " " << trajectory[k][2] << "\n";
    }

    TwoBdyEphem.close();
    ThreeBdyEphem.close();






    std::vector<OE> coe_moon = computeOrbitalElements(moon_traj, 4902.8695);
    std::vector<OE> coe_3body = computeOrbitalElements(trajectory, 4902.8695);


    std::vector<OE> delta;
    for (size_t k = 0; k < trajectory.size(); k++) {
        OE delta_oe = {
            coe_3body[k].a - coe_moon[k].a,
            coe_3body[k].e - coe_moon[k].e,
            coe_3body[k].i - coe_moon[k].i,
            coe_3body[k].w - coe_moon[k].w,
            coe_3body[k].O - coe_moon[k].O,
            coe_3body[k].f - coe_moon[k].f
        };
        delta.push_back(delta_oe);
    }


    std::ofstream moonFile("TwoBodyOrbitalElements.dat");
    std::ofstream propFile("ThreeBodyOrbitalElements.dat");

    for (size_t k = 0; k < trajectory.size(); k++) {
        // Writing data for MoonOrbit
        moonFile << time_vector_days[k] << " " << coe_moon[k].a << " " << coe_moon[k].e << " "
            << coe_moon[k].i << " " << coe_moon[k].O << "\n";

        // Writing data for Propagator
        propFile << time_vector_days[k] << " " << coe_3body[k].a << " " << coe_3body[k].e << " "
            << coe_3body[k].i << " " << coe_3body[k].O << "\n";
    }

    moonFile.close();
    propFile.close();

    return 0;
}
