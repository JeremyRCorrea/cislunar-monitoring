#include <iostream>
#include <fstream>
#include "src/Propagator.h"
#include "util/Math.h"
#include "src/Sensor.h"

#define pi 3.14159265358979323846

int main() {

   
    // ------------------------------------------------------------------
    //                         SIMULATION INPUTS
    // ------------------------------------------------------------------
    const double t0 = 0.0;
    const double tf = 24 * 60 * 60;
    const double step = 1;

    const double  sensorRangeThreshold = 2000.0;
    const double  sensorFov            = 40.0 * (pi / 180);
    const double3 sensorBoresight      = double3{ 0.0, 0.0, -1.0 };
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

    // Instantiate sensor object and set properties
    Sensor lunarSensor = Sensor(
        sensorState,
        sensorRangeThreshold,
        sensorFov,
        sensorBoresight
    );

    // Instantiate RSO object and set initial state
    RSO rso = RSO{};


    // ------------------------------------------------------------------
    //                         PROPAGATE STATES
    // ------------------------------------------------------------------

    // Propagate orbit of rso
    rso.propagate(targetState, timeArray);


    // ------------------------------------------------------------------
    //                         COMPUTE ACCESS
    // ------------------------------------------------------------------

    AccessResults accessResults = lunarSensor.getAccess(rso, timeArray);

    double fovAccessPercentage = Metrics::computePercentAccess(accessResults.inFieldOfView);
    double rangeAccessPercentage = Metrics::computePercentAccess(accessResults.inRange);
    double overallAccessPercentage = Metrics::computePercentAccess(accessResults.overallAccess);

    std::cout << "Field of View Access Percentage: " << fovAccessPercentage << "%\n";
    std::cout << "Range Threshold Access Percentage: " << rangeAccessPercentage << "%\n";
    std::cout << "Overall Access Percentage: " << overallAccessPercentage << "%\n";

    
    double avgGapDuration = Metrics::computeAverageGapDuration(accessResults.overallAccess);
    double gapDurationMinutes = (step * avgGapDuration) / 60.0;

    std::cout << "Average Duration of Gaps in Visibility: " << gapDurationMinutes << " minutes\n";


    // ------------------------------------------------------------------
    //                         OUTPUT RESULTS
    // ------------------------------------------------------------------

    std::vector<double> time_vector_days(0);
    for (double t = t0; t <= tf; t += step) {
        time_vector_days.push_back(t / 86400.0);
    }

    std::vector<double> time_vector_hours(0);
    for (double t = t0; t <= tf; t += step) {
        time_vector_hours.push_back(t / 3600.0 );
    }

    std::ofstream accessFile("AccessData.dat");

    for (size_t k = 0; k < accessResults.overallAccess.size(); ++k) {
        accessFile << time_vector_hours[k] << " "
            << 100.0 * accessResults.inFieldOfView[k] << " "
            << 100.0 * accessResults.inRange[k] << " "
            << 100.0 * accessResults.overallAccess[k] << "\n";
    }

    accessFile.close();

    

    std::vector<double> time_vector(0);
    for (double t = t0; t <= tf; t += step) {
        time_vector.push_back(t);
    }

    



    return 0;
}
