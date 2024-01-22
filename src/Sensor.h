#pragma once
#include "RSO.h"
#include "util/Math.h"
#include "util/CoverageMetrics.h"

struct AccessResults {
    std::vector<bool> inFieldOfView;
    std::vector<bool> inRange;
    std::vector<bool> overallAccess;
};

class Sensor {

public:

    bool isStationary = false;
    std::vector<State> state;

    double fov = 0.0;
    double rangeThreshold = 0.0;
    double3 boresight = double3{ 0.0, 0.0, 0.0 };

    Sensor() {}
    Sensor(const State& position, double rangeThreshold, double fov, double3 boresight)
        : isStationary(true), rangeThreshold(rangeThreshold), fov(fov), boresight(boresight) {
        state.push_back(position);
    }

    AccessResults getAccess(const RSO& rso, const TimeArray& timeArray);

private:

    // Scenario specific constraints
    bool inFov       (const double3& sensorPosition, const double3& targetPosition);
    bool isDetectable(const double3& sensorPosition, const double3& targetPosition);

};

inline AccessResults Sensor::getAccess(const RSO& rso, const TimeArray& timeArray) {

    AccessResults results;
    results.inFieldOfView.resize(timeArray.size());
    results.inRange.resize(timeArray.size());
    results.overallAccess.resize(timeArray.size());


    std::vector<bool> access(timeArray.size());

    for (int k = 0; k < timeArray.size(); k++) {

        // Get State of actors
        State sensorState = state[0];
        if (!(this->isStationary)) {
            sensorState = state[k];
        }

        State targetState = rso.state[k];

        // Get positions
        double3 sensorPosition{ sensorState[0], sensorState[1], sensorState[2] };
        double3 targetPosition{ targetState[0], targetState[1], targetState[2] };

        // Evaluate constraints
        results.inFieldOfView[k] = inFov(sensorPosition, targetPosition);
        results.inRange[k] = isDetectable(sensorPosition, targetPosition);
        results.overallAccess[k] = results.inFieldOfView[k] && results.inRange[k];
    }

    return results;
}

inline bool Sensor::inFov(const double3& sensorPosition, const double3& targetPosition) {

    // Get position of target relative to sensor
    double3 R_sensor2target = targetPosition - sensorPosition;

    // Compute angle between boresight and direction to target
    double angleOffBoresight = angle(R_sensor2target, this->boresight);

    // Compare angle off boresight with circular FOV
    return angleOffBoresight < this->fov;
}

inline bool Sensor::isDetectable(const double3& sensorPosition, const double3& targetPosition) {

    // Compute distance between target and sensor
    double rangeToTarget = distance(sensorPosition, targetPosition);

    // Compare against static range threshold
    return rangeToTarget < this->rangeThreshold;
}