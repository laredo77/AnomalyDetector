/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: 311547087, Itamar Laredo
 */
#include <iostream>
#include <vector>
#include "AnomalyDetector.h"
#include "SimpleAnomalyDetector.h"
#include "minCircle.h"
#include <sstream>

// Constructor
SimpleAnomalyDetector::SimpleAnomalyDetector() {
    vector<correlatedFeatures> cf;
}

// Destructor
SimpleAnomalyDetector::~SimpleAnomalyDetector() {
}

// Offline learning new data as normal values
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    TimeSeries time_series = ts;
    map<std::string, vector<float>> data_s = time_series.get_data_structure();

    // Checks all the correlation combinations
    for (int i = 0; i < data_s.size() - 1; i++) {
        for (int j = i + 1; j < data_s.size(); j++) {

            float correlation = std::abs(pearson(
                    &data_s[time_series.get_features()[i]][0],
                    &data_s[time_series.get_features()[j]][0],
                    data_s[time_series.get_features()[i]].size()));
            // relevant correlation
            if (correlation > 0.5) {
                // Initialize Points array
                Point *ps[data_s[time_series.get_features()[i]].size()];
                for (int k = 0; k < data_s[time_series.get_features()[i]].size(); k++) {
                    ps[k] = new Point(data_s[time_series.get_features()[i]][k],
                                      data_s[time_series.get_features()[j]][k]);
                }
                // get linear reg
                Line lin_reg = linear_reg(ps, data_s[time_series.get_features()[i]].size());
                // Initialize correlatedFeatures struct with data,
                // and adding it to cf vector
                correlatedFeatures cfs = {};
                cfs.x = 0;
                cfs.y = 0;
                cfs.radius = 0;
                // Focuses on correlations higher than 0.9
                if (correlation > 0.9) {
                    // Calculates the max deviation
                    float max_dev = 0.0;
                    for (int l = 0; l < data_s[time_series.get_features()[i]].size(); l++) {
                        if (max_dev < dev(*ps[l], lin_reg)) {
                            max_dev = dev(*ps[l], lin_reg);
                        }
                    }
                    cfs.threshold = max_dev * 1.1;
                    // otherwise correlations higher than 0.5
                } else {
                    // get minimal circle
                    Circle c = findMinCircle(ps, data_s[time_series.get_features()[i]].size());
                    cfs.threshold = c.radius * 1.1;
                    cfs.x = c.center.x;
                    cfs.y = c.center.y;
                    cfs.radius = c.radius;
                }
                // Initialize the rest of the correlationFeatures struct
                cfs.feature1 = time_series.get_features()[i];
                cfs.feature2 = time_series.get_features()[j];
                cfs.corrlation = correlation;
                cfs.lin_reg = lin_reg;
                cf.push_back(cfs);

                // Deleting points
                for (int m = 0; m < data_s[time_series.get_features()[i]].size(); m++) {
                    delete ps[m];
                }
            }
        }
    }
}

// Online detecting anomaly, and report it
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    // TODO Auto-generated destructor stub
    TimeSeries time_series = ts;
    map<std::string, vector<float>> data_s = time_series.get_data_structure();
    //vector<AnomalyReport> v_ar;

    for (int i = 0; i < cf.size(); i++) {
        Point *ps[data_s[cf[i].feature1].size()];
        for (int j = 0; j < data_s[cf[i].feature1].size(); j++) {
            ps[j] = new Point(data_s[cf[i].feature1][j], data_s[cf[i].feature2][j]);
        }

        // Checks deviation to each point
        float dev_p = 0.0;
        for (int k = 0; k < data_s[cf[i].feature1].size(); k++) {
            if (cf[i].corrlation > 0.9) {
                // Case used simple detector
                dev_p = dev(*ps[k], cf[i].lin_reg);
            } else {
                // case used min circle detector
                dev_p = dist(Point(cf[i].x, cf[i].y), *ps[k]);
            }
            // Checks whether the deviation of the current point
            // is larger than the max deviation
            if (dev_p > cf[i].threshold) {
                // If so, reported as anomaly!
                string desc = cf[i].feature1 + "-" + cf[i].feature2;
                AnomalyReport aReport = AnomalyReport(desc, (k + 1));
                this->v_ar.push_back(aReport);
            }
        }
        // Deleting points
        for (int m = 0; m < data_s[time_series.get_features()[i]].size(); m++) {
            delete ps[m];
        }
    }
    return this->v_ar;
}