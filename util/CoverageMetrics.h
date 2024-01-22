#pragma once
#include <vector>

namespace Metrics {


    double computePercentAccess(const std::vector<bool>& accessData) {
        int count = std::count(accessData.begin(), accessData.end(), true);
        return 100.0 * count / accessData.size();
    }

    double computeAverageGapDuration(const std::vector<bool>& overallAccess) {
        int gapCount = 0;
        int totalGapLength = 0;
        int currentGapLength = 0;
        bool inGap = false;

        for (bool access : overallAccess) {
            if (!access) {
                // Currently in a gap
                if (!inGap) {
                    // Gap has just started
                    inGap = true;
                    gapCount++;
                    currentGapLength = 1;
                }
                else {
                    // Gap continues
                    currentGapLength++;
                }
            }
            else {
                // Not in a gap, add the length of the gap that just ended
                if (inGap) {
                    totalGapLength += currentGapLength;
                    inGap = false;
                }
            }
        }

        // Check if the last gap goes until the end
        if (inGap) {
            totalGapLength += currentGapLength;
        }

        // Compute and return the average gap length
        return (gapCount > 0) ? static_cast<double>(totalGapLength) / gapCount : 0.0;
    }

};