/*
 * HybridAnomalyDetector.cpp
 *
 * Author: 311547087, Itamar Laredo
 */
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
}

HybridAnomalyDetector::~HybridAnomalyDetector() {
}

/*
 * Set new threshold
 */
void HybridAnomalyDetector::set_threshold(float threshold) {
    for(int i = 0; i < this->cf.size(); i++) {
        this->cf[i].threshold = threshold;
    }
}

/*
 * Get current threshold
 */
float HybridAnomalyDetector::get_threshold() {
    if (!this->cf.empty()) {
        return this->cf[0].threshold;
    }
    return 0.9;
}

/*
 * Get anomaly report vector
 */
vector<AnomalyReport> HybridAnomalyDetector::get_vAr() {
    return this->v_ar;
}

