#ifndef PATTERN_MATRIX_H
#define PATTERN_MATRIX_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <limits>
#include <math.h>
#include <cmath>

class PatternMatrix {
    int rows;
    int cols;

public:

    PatternMatrix(int rows, int cols) {
        this->rows = rows;
        this->cols = cols;
    }

    void find(vector<Vec4f> & circles) {
        for (auto it: circles) {
            cout << it[0] << ", " << it[1] << ", " << it[2] << ", " << it[3] << endl;
        }
    }

};

#endif // PATTERN_MATRIX_H
