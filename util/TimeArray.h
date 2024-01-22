#pragma once
#include <vector>

class TimeArray {
public:
    double start;
    double end;
    double step;
    std::vector<double> val;

    TimeArray(double start, double end, double step)
        : start(start), end(end), step(step) {
        for (double t = start; t <= end; t += step) {
            val.push_back(t);
        }
    }

    int size() const {
        return val.size();
    }
};