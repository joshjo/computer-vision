#ifndef CIRCLEPROCESS_H
#define CIRCLEPROCESS_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
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

struct Circle {
    int id;
    number x;
    number y;
    number r;
    number ri;

    number rx;
    number ry;

    Circle() {
        x = y = r = ri = rx = ry = 0;
    }

    Circle(number x, number y, number r, number ri = 0) {
        this->id = -1;
        this->x = x;
        this->y = y;
        this->r = r;
        this->ri = ri;

        this->rx = x;
        this->ry = y;
    }

    number radius_diff() {
        return r - ri;
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

    vector<Circle*> get_circles() {
        vector<Circle*> circles;
        for (auto& it : circleGroups) {
            if (it.circles.size() == 2) {
                point center = it.getCenter();
                point radius = it.bounding_r;
                circles.push_back(
                    new Circle(center.first, center.second, radius.second, radius.first)
                );
            }
        }
        return circles;
    }

};


struct PatternMatrix {
    int rows;
    int cols;
    vector<Circle *> circles;
    bool isValid;
    vector<Circle *> * matrix;

    PatternMatrix(int cols, int rows) {
        this->cols = cols;
        this->rows = rows;
        isValid = false;
        matrix = new vector<Circle *>(cols * rows, NULL);
    }

    int numberValids(){
        int count = 0;
        for (int i = 0; i < matrix->size(); i++) {
            if (matrix->at(i) != NULL) {
                count++;
            }
        }
        return count;
    }

    void run(ProcessCircles & pc) {
        vector<Circle *> xircles = pc.get_circles();

        number avg_radius = 0;

        for (auto& it: xircles) {
            avg_radius += it->radius_diff();
        }

        for (int i = 0; i < xircles.size(); i++) {
            avg_radius  += xircles[i]->radius_diff();
//            if (xircles[i]->r >= avg_radius) {
//                circles.push_back(xircles[i]);
//            }
        }

        avg_radius = avg_radius / xircles.size();

//        for(auto& it: xircles) {
//            if(it->r >= avg_radius) {
//                circles.push_back(it);
//            }
//        }
        for (int i = 0; i < xircles.size(); i++) {
            if (xircles[i]->r >= avg_radius) {
                circles.push_back(xircles[i]);
            }
        }

        if(circles.size() != (cols * rows)) {
            return;
        }
        isValid = true;

        vector <Point2f> points;

        for(auto& it: circles) {
            Point2f p(it->x, it->y);
            points.push_back(p);
        }
        RotatedRect rrect = minAreaRect(Mat(points));

        Point2f rxy = rrect.center;
        Size2f rs = rrect.size;
        number angle = 90 - rrect.angle;

        if (angle > 0) {
            angle += 180;
        }
        if ((rows < cols) && (rs.width >rs.height)) {
            angle -= 90;
        }
        angle = angle * (3.14159265359 / 180.0);

        number cos_a = cos(angle);
        number sin_a = sin(angle);

        for(auto& it: circles) {
            it->rx -= rxy.x;
            it->ry -= rxy.y;

            number x1 = it->rx;
            number y1 = it->ry;

            it->rx = x1 * cos_a - y1 * sin_a;
            it->ry = x1 * sin_a + y1 * cos_a;
        }

        vector <Point2f> bounding_points;

        for(auto& it: circles) {
            Point p(it->rx, it->ry);
            bounding_points.push_back(p);
        }

        Rect rect = boundingRect(Mat(bounding_points));

        number x1 = rect.x;
        number y1 = rect.y;

        number w = rect.width;
        number h = rect.height;

        number dx = w / cols;
        number dy = h / rows;

//        for(auto& it: circles) {
        for (int e = 0; e < circles.size(); e++) {
            Circle * c = circles[e];
            int i = (int) ((c->rx - x1) / dx);
            int j = (int) ((c->ry - y1) / dy);
            c->id = i + (j * cols);
            matrix->at(c->id) = c;
        }

    }
};


#endif // CIRCLEPROCESS_H
