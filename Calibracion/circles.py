import cv2 as cv
import numpy as np

import math
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import matplotlib as mpl

from itertools import cycle



circles = [
    [114, 252, 8],
    [114, 252, 16],
    [172, 452, 8],
    [172, 452, 16],
    [284, 196, 8],
    [284, 196, 16],
    [368, 378, 8],
    [368, 378, 16],
    [454, 148, 8],
    [454, 148, 16],
    [522, 304, 8],
    [522, 304, 16],
]


PI = 3.1415926535897932384626433832795

cycol = cycle('bgrcmyo')


class CircleGroup(object):

    def __init__(self):
        self.circles = []
        self.bounding_x = [math.inf, 0]
        self.bounding_y = [math.inf, 0]
        self.bounding_radius = [math.inf, 0]

    def get_center(self):
        x = sum([circle[0] for circle in self.circles]) / len(self.circles)
        y = sum([circle[1] for circle in self.circles]) / len(self.circles)
        return [x, y]

    def add(self, circle):
        if len(self.circles) > 0 and not self.is_inside(circle):
            return False

        x, y, radius = circle
        if x - radius < self.bounding_x[0]:
            self.bounding_x[0] = x - radius
        if x + radius > self.bounding_x[1]:
            self.bounding_x[1] = x + radius
        if y - radius < self.bounding_y[0]:
            self.bounding_y[0] = y - radius
        if y + radius > self.bounding_y[1]:
            self.bounding_y[1] = y + radius

        if radius < self.bounding_radius[0]:
            self.bounding_radius[0] = radius
        if radius > self.bounding_radius[1]:
            self.bounding_radius[1] = radius
        self.circles.append(circle)
        return True

    def is_inside(self, circle):
        x, y, radius = circle

        center_x = sum(self.bounding_x) / 2
        center_y = sum(self.bounding_y) / 2
        center_radius = (self.bounding_x[1] - self.bounding_x[0]) / 2
        return math.sqrt(abs(
            (x - center_x) ** 2) + abs((y - center_y) ** 2)) <= center_radius


class Circle(object):

    def __init__(self, x, y, r, ri=0):
        """
        ri = Inner radius
        """

        self.id = -1
        self.x = x
        self.y = y
        self.r = r
        self.ri = ri

        self.xr = x
        self.yr = y

    def radius_diff(self):
        return self.r - self.ri

    def __repr__(self):
        return '%d (%.2f, %.2f)' % (self.id, self.x, self.y)


class ProcessCircles(object):
    def __init__(self):
        self.circle_groups = []
        self.circles = []

    def add(self, circle):
        for cg in self.circle_groups:
            has_add = cg.add(circle)

            if has_add:
                return
        cg = CircleGroup()
        cg.add(circle)
        self.circle_groups.append(cg)

    def get_circles(self):
        self.circles = []
        for i in self.circle_groups:
            x = i.get_center() + i.bounding_radius[::-1]
            self.circles.append(Circle(*x))
        return self.circles


class PatternMatrix(object):
    def __init__(self, rows, cols):
        self.rows = rows
        self.cols = cols
        self.matrix = []
        for i in range(rows):
            self.matrix.append([None] * cols)
        self.rect = []
        self.rotated_rect = []
        self.circles = []

    def run(self, pc):
        circles = pc.get_circles()

        avg_radius = sum([
            i.radius_diff() for i in circles
        ]) / len(circles)

        new_circles = []
        for c in circles:
            if c.r >= avg_radius:
                new_circles.append(c)
        circles = new_circles

        bounding_points = np.array(
            [(a.x, a.y) for a in circles], dtype=np.float32)
        rrect = cv.minAreaRect(bounding_points)

        x0, y0 = rrect[0]
        hr, wr = rrect[1]
        angle = 90 - rrect[2]


        if hr > wr:
            angle += 90

        angle = math.radians(angle)
        cos = math.cos(angle)
        sin = math.sin(angle)
        # print('==>', wr, hr)

        for e, circle in enumerate(circles):
            circle.xr -= x0
            circle.yr -= y0
            x1, y1 = circle.xr, circle.yr
            circle.xr = x1 * cos - y1 * sin
            circle.yr = x1 * sin + y1 * cos

        bounding_points = np.array([
            (i.xr, i.yr) for i in circles
        ], dtype=np.float32)

        rect = cv.boundingRect(bounding_points)

        x1, y1, w, h = rect
        dx = w / self.cols
        dy = h / self.rows
        # print('circles', circles)
        # ax.add_artist(
        #     plt.plot([x1], [y1], marker='o', markersize=3, color="red")
        # )
        # print(rrect)

        for e, circle in enumerate(circles):
            i = int((circle.xr - x1) / dx)
            j = int((circle.yr - y1) / dy)
            circle.id = (self.cols * self.rows) - (i + (j * self.cols))
            self.matrix[j][i] = circle

        self.circles = circles
        self.rect = rect
        self.rrect = rrect

        # print(self.matrix)


def process_image(img):
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    thresh = cv.GaussianBlur(gray, (9, 9), 2)
    thresh = cv.adaptiveThreshold(thresh, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY_INV, 41, 12)
    im2, contours, hierarchy = cv.findContours(thresh, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)

    hierarchy = hierarchy[0]

    print('contours', contours)
    print('hierarchy', hierarchy[0])

    for component in zip(contours, hierarchy):
        currentContour = component[0]
        currentHierarchy = component[1]
        x,y,w,h = cv.boundingRect(currentContour)
        if currentHierarchy[2] < 0:
            pass
        elif currentHierarchy[3] < 0:
            
            indexCircularity = (4 * PI * contourArea(
                currentContour))/pow(arcLength(currentContour, true), 2);
            if indexCircularity > 0.65:
                cv.rectangle(img,(x,y),(x+w,y+h),(0,255,0),3)

    # cv.drawContours(img, contours, -1, (0,255,0), 3)
    cv.imshow('image', img)
    
    cv.waitKey(0)
    cv.destroyAllWindows()
    return im2

if __name__ == '__main__':
    img = cv.imread('snap.png')
    result = process_image(img)
    
    
    # fig, ax = plt.subplots()
    # ax.set_xlim([-600, 600])
    # ax.set_ylim([-600, 600])

    # cg = CircleGroup()
    # pc = ProcessCircles()
    
    # for circle in circles:
    #     x, y, r = circle
    #     # ax.add_artist(plt.Circle(
    #     #     (x, y), r, color='r', alpha=0.5, linewidth=1, fill=False)
    #     # )
    #     pc.add(circle)

    # pm = PatternMatrix(2, 3)
    # pm.run(pc)
    # rect = pm.rect
    # rrect = pm.rrect

    # for circle in pm.circles:
    #     xr, yr = circle.xr, circle.yr
    #     x, y = circle.x, circle.y
    #     r1, r2 = circle.r, circle.ri
    #     ax.scatter(x, y, color='b')
    #     ax.scatter(xr, yr, color='r')
        
    #     # ax.add_artist(
    #     #     plt.Circle((x, y), r1, color='k', linewidth=1, fill=False))
    #     # ax.add_artist(
    #     #     plt.Circle((x, y), r2, color='k', linewidth=1, fill=False))

    #     ax.text(xr - 20, yr - 20, '%d' % circle.id)
    #     ax.text(x - 20, y - 20, '%d' % circle.id)

    # plt.show()
