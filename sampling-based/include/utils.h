/**
 * @file utils.h
 * @author Siddharth Saha (sahasiddharth611@gmail.com)
 * @brief Utility definitions
 * @version 1.0
 * @date 2023-10-25
 */
#pragma once

#include <vector>
#include <iostream>

#include <algorithm>
#include <random>
#include <limits.h>

struct Point
{
    double x, y;
    Point() {}
    Point(double x, double y) : x(x), y(y) {}
};

Point operator+(const Point &q1, const Point &q2);

Point operator-(const Point &q1, const Point &q2);

Point operator*(const Point &q1, double d);

struct Polygon
{
    std::vector<Point> points; // Assumes clockwise/anti-clockwise points input
    int num_points = 0;

    Polygon() {}
    Polygon(std::vector<Point> points, int size) : points(points), num_points(size) {}
};

bool isPointInsideObstacle(std::vector<Polygon> obstacles, Point p);