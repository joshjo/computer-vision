#ifndef CIRCLEPROCESS_H
#define CIRCLEPROCESS_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <limits>
#include <math.h>

using namespace std;
using namespace cv;

typedef float number;
typedef pair<number, number> point;

number inf = std::numeric_limits<number>::infinity();

class CircleGroup {
public:
    vector<Vec3f> circles;
    point bounding_x;
    point bounding_y;
    point bounding_r;

    CircleGroup() {
        bounding_x.first = inf;
        bounding_x.second = 0;

        bounding_y.first = inf;
        bounding_y.second = 0;

        bounding_r.first = inf;
        bounding_r.second = 0;
    }

    point getCenter() {
        number x = 0;
        number y = 0;
        for (auto& it : circles) {
            x += it[0];
            y += it[1];
        }
        return point(x, y);
    }

    bool add(Vec3f & circle) {
        if (circles.size() > 0 && (! isInside(circle))) {
            return false;
        }

        number x = circle[0];
        number y = circle[1];
        number r = circle[2];

        if ((x - r) < bounding_x.first) {
            bounding_x.first = (x - r);
        }
        if ((x + r) > bounding_x.second) {
            bounding_x.second = (x + r);
        }
        if ((y - r) < bounding_y.first) {
            bounding_y.first = (y - r);
        }
        if ((y + r) > bounding_y.first) {
            bounding_y.second = (y + r);
        }

        if (r < bounding_r.first) {
            bounding_r.first = r;
        }
        if (r > bounding_r.second) {
            bounding_r.second = r;
        }

        circles.push_back(circle);
        return true;
    }

    bool isInside(Vec3f & circle) {
        number x = circle[0];
        number y = circle[1];

        number center_x = (bounding_x.first + bounding_x.second) / 2;
        number center_y = (bounding_y.first + bounding_y.second) / 2;
        number center_r = (bounding_r.first + bounding_r.second) / 2;

        return sqrt(
            pow(abs(x - center_x), 2) + pow(abs(y - center_y), 2)
        ) <= center_r;
    }

};

class ProcessCircles {

};

#endif // CIRCLEPROCESS_H
