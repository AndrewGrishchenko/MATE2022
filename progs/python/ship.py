import math

import pygame
import sys
import pygame as pg
from math import *

White = (255, 255, 255)
Black = (0, 0, 0)
brown = (180, 125, 90)
pos1 = 1000, 1000
pos2 = 1000, 1000
flag = 0
x = 0
y = 0
i = 2 / 1
i1 = 1 / 2
d = 0
len = 0
m = 50
pressed = 0, 0, 0
pygame.init()
screen = pygame.display.set_mode((874, 261))
font = pygame.font.SysFont('pricedown', 30)  # шрифт


# text = font.render("side of pool", True, Black)
# text = pygame.transform.rotate(text, -90)
# screen.blit(text, (100, 100))


# img1 = pygame.image.load('line.jpg')

def draw_pool():
    screen.fill((255, 255, 255))
    pygame.draw.line(screen, (250, 240, 50), (32, 261), (32, 0), 6)
    pygame.draw.line(screen, (250, 240, 50), (870, 261), (870, 0), 6)
    pygame.draw.line(screen, (0, 0, 255), (32, 87), (32, 174), 6)
    pygame.draw.line(screen, (0, 0, 255), (870, 87), (870, 174), 6)
    pygame.draw.line(screen, (0, 0, 0), (36, 257), (867, 257), 6)
    pygame.draw.line(screen, (0, 0, 0), (36, 3), (867, 3), 6)
    pygame.draw.line(screen, (220, 130, 180), (36, 130), (867, 130), 2)
    pygame.draw.line(screen, (220, 130, 180), (238, 254), (238, 7), 2)
    pygame.draw.line(screen, (220, 130, 180), (445, 254), (445, 7), 2)
    pygame.draw.line(screen, (220, 130, 180), (653, 254), (653, 7), 2)
    text = font.render("side of pool", True, Black)
    text = pygame.transform.rotate(text, -90)
    screen.blit(text, (5, 70))


def draw():
    pygame.draw.line(screen, brown, pos1, pos2, 6)
    # pygame.draw.circle(screen, (0, 255, 0), (x, y), 5)
    # pygame.draw.circle(screen, (0, 255, 0), (x1, y1), 5)
    pygame.draw.line(screen, brown, (x, y), (x + m * x2, y + m * y2), 6)
    pygame.draw.line(screen, brown, (x, y), (x + m * -x2, y + m * -y2), 6)
    pygame.draw.line(screen, brown, (x1, y1), (x1 + m * x2, y1 + m * y2), 6)
    pygame.draw.line(screen, brown, (x1, y1), (x1 + m * -x2, y1 + m * -y2), 6)


def scale_number(unscaled, to_min, to_max, from_min, from_max):
    return round((to_max - to_min) * (unscaled - from_min) / (from_max - from_min) + to_min, 2)


while True:
    # screen.blit(img1, (0, 0))
    draw_pool()
    pressed = pg.mouse.get_pressed()
    x, y = (pos1[0] + i * pos2[0]) / (1 + i), (pos1[1] + i * pos2[1]) / (1 + i)
    x1, y1 = (pos1[0] + i1 * pos2[0]) / (1 + i1), (pos1[1] + i1 * pos2[1]) / (1 + i1)

    if pos2[0] - pos1[0] != 0:
        d = round(math.atan((pos2[1] - pos1[1]) / (pos2[0] - pos1[0])) / 0.0174533)
    else:
        d = 0
    if d <= 100 and d >= 80:
        pygame.draw.line(screen, (100, 65, 30), pos1, pos2, 3)
    y2, x2 = round(1 - abs(d) * 0.0111, 2), round(0 + abs(d) * 0.0111, 2)
    if d > 0:
        x2 = x2 * -1
        y2 = y2 * 1
    draw()
    # if pressed[1]:
    #     flag = 0
    # if pressed[0] == 1 and flag == 0:
    #     pos1 = pg.mouse.get_pos()
    #     flag = 1
    #     pg.time.delay(200)
    # if pressed[2] == 1 and flag == 1:
    #     pos2 = pg.mouse.get_pos()
    #     flag = 2
    #     pg.time.delay(200)

    if pressed[0] == 1:
        if flag == 0:
            pos1 = pg.mouse.get_pos()
            pos2 = pg.mouse.get_pos()
            flag = 1
        else:
            pos2 = pg.mouse.get_pos()
    if pressed[1] == 1 and flag == 1:
        pos1 = 1000, 1000
        pos2 = 1000, 1000
        flag = 0
        draw_pool()

    len = ((pos2[1] - pos1[1]) * 2) + ((pos2[0] - pos1[0]) * 2)
    #print(len)
    if len < 121000:
        m = scale_number(len, 25, 50, 0, 121000)
    else:
        m = scale_number(len, 50, 140, 121000, 746593)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.image.save(screen, "/home/andrew/window.png")
            print("eee")
            pygame.quit()
            sys.exit()
    pygame.display.flip()