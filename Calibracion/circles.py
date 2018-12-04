import math
import matplotlib.pyplot as plt
from itertools import cycle


circles = [
    [20, 3, 2],
    [1, 2, 3],
    [2, 3, 4],
    [2, 3, 2],
    [10, 5, 2],
    [11, 4, 5],
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


class ProcessCircles(object):
    def __init__(self):
        self.circle_groups = []

    def add(self, circle):
        for cg in self.circle_groups:
            has_add = cg.add(circle)

            if has_add:
                return
        cg = CircleGroup()
        cg.add(circle)
        self.circle_groups.append(cg)


if __name__ == '__main__':
    # cg = CircleGroup()
    pc = ProcessCircles()
    fig, ax = plt.subplots()
    ax.set_xlim([-5, 30])
    ax.set_ylim([-5, 30])
    for circle in circles:
        x, y, r = circle
        ax.add_artist(plt.Circle(
            (x, y), r, color=next(cycol), alpha=0.5, linewidth=1, fill=False)
        )
        pc.add(circle)

    for cg in pc.circle_groups:
        x, y = cg.get_center()
        r1, r2 = cg.bounding_radius
        print(x, y, r1, r2)
        ax.scatter(x, y, color='k')
        ax.add_artist(
            plt.Circle((x, y), r1, color='k', linewidth=1, fill=False))
        ax.add_artist(
            plt.Circle((x, y), r2, color='k', linewidth=1, fill=False))
    plt.show()
