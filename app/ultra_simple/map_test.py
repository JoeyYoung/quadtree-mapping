from ctypes import *
import os
import sys
import threading
import time

step_size = 3.0
forward_seconds = 5.0
pi = 3.1415926

# define start position and theta
# todo, current only apply to theta=0
start_x = 20.0
start_y = 5.0
# walker_theta = 0.0
walker_theta = 180.0

def loop_record(control):
    def print_odo():
        print("odometry x y theta: ", round(control.odo.X, 3), round(control.odo.Y, 3), round(control.odo.THETA, 3))

    add_plus = CDLL('./libmap.so')
    step = 0

    while True:
        temp = input()
        # scan and store
        print_odo()
        if walker_theta == 0.0:
            x = c_float(control.odo.X + start_x)
            y = c_float(control.odo.Y + start_y)

            if control.odo.THETA > 0:
                theta = 360 - control.odo.THETA / pi * 180
            else:
                theta = control.odo.THETA / pi * 180

        elif walker_theta == 180.0:
            x = c_float(start_x - control.odo.X)
            y = c_float(start_y - control.odo.Y)

            if control.odo.THETA > 0:
                theta = 180 - control.odo.THETA / pi * 180
            else:
                theta = 180 - control.odo.THETA / pi * 180

        theta = c_float(theta)
        add_plus.run_main_plus(x, y, theta, c_int(step))
        step += 1


if __name__ == '__main__':
    cd = CD.ControlDriver(record_mode=True)
    p1 = threading.Thread(target=loop_record, args=(cd,))
    p2 = threading.Thread(target=cd.control_part, args=())

    p2.start()
    p1.start()

    loop_record(cd)