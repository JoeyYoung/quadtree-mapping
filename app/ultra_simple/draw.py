import cv2
import numpy as np
import os

def read_points(inputdirs):
    res = []

    for inputdir in inputdirs:
        files = os.listdir(inputdir)
        for file in files:
            # skip dir
            name = str(file.title()).lower()

            if os.path.isdir(file) or name[:6] != 'points':
                continue

            with open(os.path.join(inputdir, file), 'r') as f:
                lines = f.readlines()
                for i in range(len(lines)):
                    line = lines[i]
                    xy = line.split(";")
                    temp = [float(xy[0]), float(xy[1])]
                    res.append(temp)
    return res

def read_tree():
    res = []
    with open("/Users/xyzhao/Desktop/quadtree-mapping/lidar_tree", 'r') as f:
        lines = f.readlines()
        for i in range(len(lines)):
            line = lines[i]
            xy = line.split(";")
            temp = [float(xy[0]), float(xy[1]), float(xy[2]), float(xy[3])]
            res.append(temp)
    return res

if __name__ == '__main__':
    unit = 1000
    img = np.zeros([unit, unit])

    for i in range(unit):
        for j in range(unit):
            img[i][j] = 255

    # dirs = ["./test_step1"]
    # points = read_points(dirs)

    # for point in points:
    #     x = int(point[0] * 25)
    #     y = int(point[1] * 25)

    #     if x > 1000 or y > 1000:
    #         continue
    #     img[x][y] = 0

    grids = read_tree()
    for grid in grids:
        x1 = round(grid[0] * 25)
        x2 = round(grid[1] * 25)
        y1 = round(grid[2] * 25)
        y2 = round(grid[3] * 25)

        for x in range(x1, x2+1):
            for y in range(y1, y2+1):
                if x > 1000 or y > 1000:
                    continue
                img[x][y] = 0

    cv2.namedWindow("contour", cv2.WINDOW_NORMAL)
    cv2.imshow("contour", img)

    cv2.waitKey()