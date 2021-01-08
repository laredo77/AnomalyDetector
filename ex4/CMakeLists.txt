cmake_minimum_required(VERSION 3.16)
project(ex4)

set(CMAKE_CXX_STANDARD 11)

add_executable(headers_include anomaly_detection_util.cpp anomaly_detection_util.h MainTrain.cpp
        AnomalyDetector.h SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h
        timeseries.cpp timeseries.h minCircle.cpp minCircle.h
        HybridAnomalyDetector.cpp HybridAnomalyDetector.h CLI.cpp CLI.h commands.h expectedOutput.txt
        input.txt)