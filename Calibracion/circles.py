import cv2 as cv
import numpy as np

import math
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import matplotlib as mpl

from itertools import cycle

circles = [
    [343.262, 367.539, 8],
    [343.181, 367.706, 16],
    [295.516, 357.516, 8],
    [295.487, 357.538, 16],
    [248.286, 347.307, 8],
    [248.215, 347.246, 16],
    [202.037, 336.895, 7],
    [491.936, 330.887, 5],
    [201.998, 336.861, 16],
    [157.261, 326.337, 7],
    [353.526, 319.629, 8],
    [157.321, 326.442, 15],
    [353.461, 319.71, 16],
    [305.808, 309.663, 8],
    [305.742, 309.829, 16],
    [258.701, 299.8, 8],
    [258.656, 299.849, 16],
    [212.368, 289.997, 7],
    [212.374, 290.051, 16],
    [167.489, 280.242, 7],
    [363.13, 271.776, 8],
    [167.396, 280.299, 15],
    [363.049, 271.922, 16],
    [315.756, 262.303, 8],
    [315.72, 262.237, 16],
    [268.945, 252.775, 8],
    [268.865, 252.794, 16],
    [222.955, 243.449, 8],
    [222.754, 243.579, 16],
    [177.986, 234.467, 7],
    [178.046, 234.567, 16],
    [372.359, 225.089, 7],
    [372.458, 225.098, 16],
    [325.288, 215.76, 7],
    [325.386, 215.656, 16],
    [279.086, 206.645, 7],
    [278.915, 206.711, 16],
    [233.282, 198.11, 7],
    [188.727, 189.879, 7],
    [233.337, 197.971, 16],
    [188.904, 189.72, 16],
    [381.024, 179.251, 7],
    [380.964, 179.215, 16],
    [334.648, 170.321, 7],
    [334.629, 170.235, 16],
    [288.749, 161.739, 7],
    [243.775, 153.949, 7],
    [288.786, 161.846, 16],
    [199.425, 146.372, 6],
    [243.718, 153.944, 16],
    [199.825, 146.332, 16],
    [388.713, 135.081, 7],
    [388.953, 135.136, 16],
    [343.408, 126.394, 7],
    [298.155, 118.473, 6],
    [343.239, 126.496, 16],
    [490.551, 114.973, 4],
    [253.947, 111.356, 7],
    [298.141, 118.6, 16],
    [210.634, 104.652, 6],
    [253.82, 111.382, 16],
    [210.474, 104.741, 15],
]

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
        return '%d' % self.id


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
        wr, hr = rrect[1]
        angle = 90 - rrect[2]

        if self.rows < self.cols and wr > hr:
            angle -= 90

        if angle > 0:
            angle += 180

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

        for e, circle in enumerate(circles):
            i = int((circle.xr - x1) / dx)
            j = int((circle.yr - y1) / dy)
            circle.id = i + (j * self.cols)
            self.matrix[j][i] = circle

        self.circles = circles
        self.rect = rect
        self.rrect = rrect

        # print(self.matrix)


if __name__ == '__main__':
    cg = CircleGroup()
    pc = ProcessCircles()
    fig, ax = plt.subplots()
    ax.set_xlim([0, 600])
    ax.set_ylim([0, 600])
    for circle in circles:
        x, y, r = circle
        # ax.add_artist(plt.Circle(
        #     (x, y), r, color='r', alpha=0.5, linewidth=1, fill=False)
        # )
        pc.add(circle)

    # all_circles = np.array([
    #     i.get_center() + i.bounding_radius for i in pc.circle_groups],
    #     dtype=np.float32)
    # print(cv.boundingRect(np.array([[1.1, 2.9], [0, 0]], dtype=np.float32)))
    # print('all_circles', all_circles)

    pm = PatternMatrix(5, 6)
    pm.run(pc)
    rect = pm.rect
    rrect = pm.rrect

    print(rect)

    # q = pm.matrix[1][5]
    # print('**', q.xr, q.yr, q.r, q.ri)

    for circle in pm.circles:
        x, y = circle.x, circle.y
        r1, r2 = circle.r, circle.ri
        ax.scatter(x, y, color='k')
        ax.add_artist(
            plt.Circle((x, y), r1, color='k', linewidth=1, fill=False))
        ax.add_artist(
            plt.Circle((x, y), r2, color='k', linewidth=1, fill=False))
        ax.text(x - 20, y - 20, '%d' % circle.id)

    # ax.add_patch(
    #     patches.Rectangle(
    #         (rect[0], rect[1]), rect[2], rect[3], fill=False, color='r'))
    # r2 = patches.Rectangle(
    #     (rrect[0][0] - (rrect[1][0] / 2), rrect[0][1] - (rrect[1][1] / 2)), rrect[1][0], rrect[1][1], fill=False, color='b')
    # t2 = mpl.transforms.Affine2D().rotate_deg_around(rrect[0][0], rrect[0][1], rrect[2]) + ax.transData
    # # r2 = patches.Rectangle(
    # #     (0, 0), rrect[1][0], rrect[1][1], fill=False, color='b')
    # r2.set_transform(t2)
    # ax.add_patch(r2)
    plt.show()
