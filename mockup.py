#!/usr/bin/python

import math
import sys
import pygame

pygame.init()

window = pygame.display.set_mode((144, 168))

dial_radius = 70
inner_radius1 = 68
inner_radius2 = 63
mid_tick_length = 5
big_tick_length = 10
num_hour_ticks = 8
num_minute_ticks = 7
hour_centre = (2, 72)
minute_centre = (142, 96)

rads = 2*math.pi

white = (255, 255, 255)
black = (0, 0, 0)

# Dials
pygame.draw.circle(window, white, hour_centre, dial_radius, 0)
pygame.draw.circle(window, white, minute_centre, dial_radius, 0)

pygame.draw.circle(window, black, hour_centre, inner_radius1, 1)
pygame.draw.circle(window, black, minute_centre, inner_radius1, 1)

# little ticks
for i in range(0, 25):
    if (i % 3 != 0):
        angle = rads / 48 * i
        posy = -(math.cos(angle)) * inner_radius1 + hour_centre[1]
        posx = math.sin(angle) * inner_radius1 + hour_centre[0]
        pygame.draw.line(window, black, hour_centre, (posx, posy))

for i in range(0, 61):
    if (i % 5 != 0):
        angle = rads / 120 * i
        posy = -(math.cos(angle)) * inner_radius1 + minute_centre[1]
        posx = -(math.sin(angle)) * inner_radius1 + minute_centre[0]
        pygame.draw.line(window, black, minute_centre, (posx, posy))

pygame.draw.circle(window, white, hour_centre, inner_radius2, 0)
pygame.draw.circle(window, white, minute_centre, inner_radius2, 0)
pygame.draw.circle(window, black, hour_centre, inner_radius2, 1)
pygame.draw.circle(window, black, minute_centre, inner_radius2, 1)

# big ticks on hour dial
for i in range(0, 9):
    angle = rads / 16 * i
    posy = -(math.cos(angle)) * inner_radius1 + hour_centre[1]
    posx = math.sin(angle) * inner_radius1 + hour_centre[0]
    pygame.draw.line(window, black, hour_centre, (posx, posy))
pygame.draw.circle(window, white, hour_centre, (inner_radius2-mid_tick_length), 0)

# mid ticks on minute dial
for i in range(1, 13, 2):
    angle = rads / 24 * i
    posy = -(math.cos(angle)) * inner_radius1 + minute_centre[1]
    posx = -(math.sin(angle)) * inner_radius1 + minute_centre[0]
    pygame.draw.line(window, black, minute_centre, (posx, posy))
pygame.draw.circle(window, white, minute_centre, (inner_radius2-mid_tick_length), 0)
# big ticks on minute dial
for i in range(0, 13, 2):
    angle = rads / 24 * i
    posy = -(math.cos(angle)) * inner_radius1 + minute_centre[1]
    posx = -(math.sin(angle)) * inner_radius1 + minute_centre[0]
    pygame.draw.line(window, black, minute_centre, (posx, posy))
pygame.draw.circle(window, white, minute_centre, (inner_radius2-big_tick_length), 0)

# Centres
pygame.draw.circle(window, black, hour_centre, 4, 0)
pygame.draw.circle(window, black, minute_centre, 4, 0)

# Mock up some hands
angle = rads / 48 * 13
posy = -(math.cos(angle)) * dial_radius + hour_centre[1]
posx = math.sin(angle) * dial_radius + hour_centre[0]
pygame.draw.line(window, black, hour_centre, (posx, posy))

angle = rads / 120 * 37
posy = -(math.cos(angle)) * dial_radius + minute_centre[1]
posx = -(math.sin(angle)) * dial_radius + minute_centre[0]
pygame.draw.line(window, black, minute_centre, (posx, posy))

pygame.display.flip()

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit(0)
        else:
            print event
