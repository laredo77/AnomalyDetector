/*
 * animaly_detection_util.cpp
 *
 * Author: 311547087, Itamar Laredo
 */

#include <cmath>
#include "anomaly_detection_util.h"

float avg(float* x, int size) {
    // Initialize variables
    float avg;
    float sum = 0;

    // Summing up the array numbers
    for (int i = 0; i < size; i++) {
        sum = sum + x[i];
    }

    // Making an average, sum divided by size
    avg = sum / (float) size;

    return avg;
}

// returns the variance of X and Y
float var(float* x, int size) {
    // Initialize variables
    float sum = 0;
    float sum2 = 0;

    float coefficient = 1 / (float) size;

    // Summing up the array numbers in purpose to declare mu
    for (int i = 0; i < size; i++) {
        sum = sum + x[i];
    }

    // Mu variable be like
    float mu = coefficient * sum;

    // Summarize the numbers as shown in the equation
    for (int i = 0; i < size; i++) {
        sum2 = sum2 + (x[i] * x[i]);
    }

    // Variance as shown in the equation
    float variance = (coefficient * sum2) - (mu * mu);

    return variance;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size) {
    // Initialize variables
    float avg_x = avg(x, size);
    float avg_y = avg(y, size);
    float sum = 0;

    // Summarize the numbers as shown in the equation
    for (int i = 0; i < size; i++) {
        sum = sum + ((x[i] - avg_x) * (y[i] - avg_y));
    }

    // Covariance as shown in the equation
    float covariance = sum / (float) size;

    return covariance;
}

// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size) {
    // Initialize variables
    float covariance = cov(x, y, size);
    float standard_deviation_x = std::sqrt(var(x, size));
    float standard_deviation_y = std::sqrt(var(y, size));

    // Pearson as shown in the equation
    float pearson = covariance / (standard_deviation_x * standard_deviation_y);

    return pearson;
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size) {

    // Initialize arrays
    float xArr[size];
    float yArr[size];

    // Placing the values into the correct array
    for (int i = 0; i < size; i++) {
        xArr[i] = points[i]->x;
        yArr[i] = points[i]->y;
    }

    // Initialize variables as shown in the equation
    float numerator = cov(xArr, yArr, size);
    float denominator = var(xArr, size);
    float a = numerator / denominator;
    float avg_x = avg(xArr, size);
    float avg_y = avg(yArr, size);
    float b = avg_y - (a * avg_x);
    // Returning a new line with a and b variables
    return Line(a, b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
    // Initialize new line from the points array and its size
    Line line = linear_reg(points, size);
    // Deviation as shown in the equation
    float deviation = std::abs(line.f(p.x) - p.y);

    return deviation;
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
    // Deviation as shown in the equation
    float deviation = std::abs(l.f(p.x) - p.y);

    return deviation;
}




