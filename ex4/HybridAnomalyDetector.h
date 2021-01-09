

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {

public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
    virtual float get_threshold();
    virtual void set_threshold(float threshold);
    virtual vector<AnomalyReport> get_vAr();
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
