import cv2
import numpy as np

HUE_MIN = 0
HUE_MAX = 256
SAT_MIN = 0
SAT_MAX = 256
VAL_MIN = 0
VAL_MAX = 256

trackbar_vals = {'min': np.array([HUE_MIN, SAT_MIN, VAL_MIN]),
                 'max': np.array([HUE_MAX, SAT_MAX, VAL_MAX])}

def make_setter(param_name, param_pos):
    def set_value(value):
        trackbar_vals[param_name][param_pos] = value
    return set_value

cv2.namedWindow("HSV trackbars")
cv2.createTrackbar("Hue min", "HSV trackbars", HUE_MIN, HUE_MAX, make_setter('min', 0))
cv2.createTrackbar("Hue max", "HSV trackbars", HUE_MAX, HUE_MAX, make_setter('max', 0))
cv2.createTrackbar("Saturation min", "HSV trackbars", SAT_MIN, SAT_MAX, make_setter('min', 1))
cv2.createTrackbar("Saturation max", "HSV trackbars", SAT_MAX, SAT_MAX, make_setter('max', 1))
cv2.createTrackbar("Value min", "HSV trackbars", VAL_MIN, VAL_MAX, make_setter('min', 2))
cv2.createTrackbar("Value max", "HSV trackbars", VAL_MAX, VAL_MAX, make_setter('max', 2))

cap = cv2.VideoCapture(0)

while True:
    # ret, img = cap.read()
    img = cv2.imread("work.jpg")
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, trackbar_vals['min'], trackbar_vals['max'])
    print(trackbar_vals['min'], trackbar_vals['max'])
    cv2.imshow("mask", mask)
    cv2.waitKey(5)