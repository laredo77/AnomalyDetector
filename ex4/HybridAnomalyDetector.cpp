
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
    cf = this->getNormalModel();
    v_ar = this->getVar();
}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

void HybridAnomalyDetector::set_threshold(float threshold) {
    for(int i = 0; i < cf.size(); i++) {
        this->cf[i].threshold = threshold;
    }
}

float HybridAnomalyDetector::get_threshold() {
    if (!this->cf.empty()) {
        return this->cf[0].threshold;
    }
    return 0.9;
}

vector<AnomalyReport> HybridAnomalyDetector::get_vAr() {
    return this->v_ar;
}

