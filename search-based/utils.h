/**
 * @file utils.h
 * @author Siddharth Saha (sahasiddharth611@gmail.com)
 * @brief 
 * @version 1.0
 * @date 2023-09-25
 */
#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <queue>
#include <limits.h>

typedef std::pair<int, int> Pair;
typedef std::vector<std::vector<int>> Grid;

#define NUMOFDIRS 9
#define GETMAPINDEX(X, Y, XSIZE, YSIZE) ((Y - 1) * XSIZE + (X - 1))

/**
y increasing in same direction as r (y=r+1)
x increasing in same direction as c (x=c+1)

x_size === COL
y_size === ROW

Example: (x,y)

(1,1) (2,1) ... (x_size,1)
(1,2) ......... (x_size,2)
.
.
.
(1,y_size) .... (x_size,y_size)
*/

int dR[NUMOFDIRS] = {-1, -1, -1, 0, 0, 1, 1, 1, 0};
int dC[NUMOFDIRS] = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

int dY[NUMOFDIRS] = {-1, -1, -1, 0, 0, 1, 1, 1, 0};
int dX[NUMOFDIRS] = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

bool isWithinBounds(int r, int c, int r_size, int c_size)
{
    return r >= 0 && r < r_size && c >= 0 && c < c_size;
}

bool isWithinBounds3D(int r, int c, int t, int r_size, int c_size, int t_size)
{
    return r >= 0 && r < r_size && c >= 0 && c < c_size && t >= 0 && t <= t_size;
}