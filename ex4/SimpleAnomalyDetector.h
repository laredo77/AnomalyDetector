/*
 * SimpleAnomalyDetector.h
 *
 * Author: 311547087, Itamar Laredo
 */
#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures {
    string feature1, feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    float x;
    float y;
    float radius;
};

class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector {
protected:
    vector<correlatedFeatures> cf;
    vector<AnomalyReport> v_ar;
public:
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();

    virtual void learnNormal(const TimeSeries &ts);
    virtual vector<AnomalyReport> detect(const TimeSeries &ts);

    // correlated features getter
    vector<correlatedFeatures> getNormalModel() {
        return cf;
    }
    // anomaly reports getter
    vector<AnomalyReport> getVar() {
        return v_ar;
    }
};

#endif /* SIMPLEANOMALYDETECTOR_H_ */
