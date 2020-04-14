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


if __name__ == '__main__':
    unit = 1000
    img = np.zeros([unit, unit])

    for i in range(unit):
        for j in range(unit):
            img[i][j] = 255

    dirs = ["./test_step1"]
    points = read_points(dirs)

    for point in points:
        x = int(point[0] * 25)
        y = int(point[1] * 25)

        if x > 1000 or y > 1000:
            continue
        img[x][y] = 0

    cv2.namedWindow("contour", cv2.WINDOW_NORMAL)
    cv2.imshow("contour", img)

    cv2.waitKey()