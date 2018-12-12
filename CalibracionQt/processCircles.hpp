#ifndef CIRCLEPROCESS_H
#define CIRCLEPROCESS_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui_c.h>
//#include <opencv2/core/core.hpp>
#include <limits>
#include <iostream>

using namespace cv;
using namespace std;
typedef float number;
typedef pair<number, number> point;

//number a = numeric_limits<number>::infinity();

class CircleGroup {
public:
    vector<Vec3f> circles;
    point bounding_x;
    point bounding_y;
    point bounding_r;

    CircleGroup() {
        bounding_x.first = numeric_limits<number>::infinity();
        bounding_x.second = 0;

        bounding_y.first = numeric_limits<number>::infinity();
        bounding_y.second = 0;

        bounding_r.first = numeric_limits<number>::infinity();
        bounding_r.second = 0;
    }

    point getCenter() {
        number x = 0;
        number y = 0;
        for (auto& it : circles) {
            x += it[0];
            y += it[1];
        }
        return point(x / circles.size(), y / circles.size());
    }

    Vec4f getPoint() {
        point center = getCenter();
        return Vec4f(
                    center.first,
                    center.second,
                    bounding_r.first,
                    bounding_r.second
                    );
    }

    bool add(Vec3f circle) {
        if ((circles.size() > 0) && (! isInside(circle))) {
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
public:
    vector<CircleGroup> circleGroups;

    ProcessCircles() {

    }

    void add(Vec3f &circle) {
        for (auto& it : circleGroups) {
            bool hasAdded = it.add(circle);
            if (hasAdded) {
                return;
            }
        }

        CircleGroup cg;
        cg.add(circle);
        circleGroups.push_back(cg);
    }

};

#endif // CIRCLEPROCESS_H
