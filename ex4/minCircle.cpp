/*
 * minCircle.cpp
 *
 * Author: 311547087, Itamar Laredo
 */
#include "minCircle.h"
#include <math.h>
#include <vector>
using namespace std;

/*
 * A function that returns the distance between two given points
 */
float dist(Point a, Point b) {
    return sqrt(pow(a.x - b.x,2) + pow(a.y - b.y, 2));
}
/*
 * Calculating the middle point between those two points
 * and make it the center point. In addition the radius
 * will be the half distance between the points.
 */
Circle from2points(Point a, Point b) {
    float midX = (a.x + b.x) / 2;
    float midY = (a.y + b.y) / 2;

    Point mid_point = {midX, midY};
    float mid_distance = dist(a, b) / 2;

    return {mid_point, mid_distance};
}
/*
 * Base on the equation x^2 + y^2 + 2*g*x + 2*f*y + c = 0
 * we can calculate circle from 3 given points.
 * initialize the equation with 3 points will give 3 equations to solve.
 * (1): x1^2 + y1^2 +2gx1 + 2fy1 + c = 0
 * (2): x2^2 + y2^2 +2gx2 + 2fy2 + c = 0
 * (3): x3^2 + y3^2 +2gx3 + 2fy3 + c = 0
 */
Circle from3Points(Point a, Point b, Point c) {

    float x12 = a.x - b.x;
    float x13 = a.x - c.x;
    float x21 = b.x - a.x;
    float x31 = c.x - a.x;

    float y12 = a.y - b.y;
    float y13 = a.y - c.y;
    float y21 = b.y - a.y;;
    float y31 = c.y - a.y;

    // x1^2 - x3^2
    float sx13 = pow(a.x, 2) - pow(c.x, 2);
    // y1^2 - y3^2
    float sy13 = pow(a.y, 2) - pow(c.y, 2);
    // x2^2 - x1^2
    float sx21 = pow(b.x, 2) - pow(a.x, 2);
    // y2^2 - y1^2
    float sy21 = pow(b.y, 2) - pow(a.y, 2);

    float f = ((sx13 * x12) + (sy13 * x12) + (sx21 * x13) + (sy21 * x13))
              / (2 * ((y31 * x12) - (y21 * x13)));

    float g = ((sx13 * y12) + (sy13 * y12) + (sx21 * y13) + (sy21 * y13))
              / (2 * ((x31 * y12) - (x21 * y13)));

    // Base on the equation of circle (center_x)^2 + (center_y)^2 -c = r^2
    // we will fine the center point where center_x = -g and center_y = -f
    float cVar = -pow(a.x, 2) - pow(a.y, 2)
              - (2 * g * a.x) - (2 * f * a.y);

    float center_x = -g;
    float center_y = -f;
    float radius = sqrt((pow(center_x, 2)) + (pow(center_y, 2)) - cVar);
    return {{center_x, center_y}, radius};
}

/*
 * return circle center from two points
 */
Point circumcenter(Point b, Point c) {
    float B = b.x * b.x + b.y * b.y;
    float C = c.x * c.x + c.y * c.y;
    float D = b.x * c.y - b.y * c.x;

    return { (c.y * B - b.y * C) / (2 * D), (b.x * C - c.x * B) / (2 * D) };
}

Circle trivial(vector<Point>& P) {

    if (P.empty()) {
        return { { 0, 0 }, 0 };
    }
    else if (P.size() == 1) {
        return { P[0], 0 };
    }
    else if (P.size() == 2) {
        return from2points(P[0], P[1]);
    }

    // maybe 2 of the points define a small circle that contains the 3ed point
    Circle c = from2points(P[0],P[1]);
    if(dist(P[2],c.center) <= c.radius)
        return c;
    c = from2points(P[0],P[2]);
    if(dist(P[1],c.center) <= c.radius)
        return c;
    c = from2points(P[1],P[2]);
    if(dist(P[0],c.center) <= c.radius)
        return c;

    // else find the unique circle from 3 points
    return from3Points(P[0], P[1], P[2]);
}
/* This algorithm works recursively when its stop condition is if there are no more
 * given points or if the size of the vector of boundary points 3.
 * If we enter the stop condition we call function get_circle otherwise return the call
 * to this function with smaller input (without the last point in the array).
 */
Circle welzl(Point** P,vector<Point> R, size_t n) {
    // stop condition
    if (R.size() == 3 || n == 0) {
        return trivial(R);
    }
    // Initialize new circle with the return value of the recursive call
    Circle new_circle = welzl(P, R, n - 1);
    // Check whether the last point in the array is inside the new circle
    // or outside. If inside --> return the new circle
    if (dist(new_circle.center, *P[n - 1]) <= new_circle.radius) {
        return new_circle;
    }
    // The last point is outside the boundary, than it push it to the
    // boundary vector and call recursively to welzl_algorithm function.
    R.push_back(*P[n - 1]);
    return welzl(P, R, n - 1);
}
/*
 * Using Welzl algorithm recursively to find the smallest circle
 * that contains all the points. Any point that is outside a given circle
 * will be on the boundary of the new circle that includes it.
 * This process continue recursively for all given points.
 */
Circle findMinCircle(Point** points,size_t size) {
    // Initialize vector of points to hold all the boundary points
    vector<Point> R;
    return welzl(points, R, size);
}
