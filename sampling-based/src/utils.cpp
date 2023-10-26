/**
 * @file utils.cpp
 * @author Siddharth Saha (sahasiddharth611@gmail.com)
 * @brief Utility defintions
 * @version 1.0
 * @date 2023-10-25
 */
#include "utils.h"

Point operator+(const Point &q1, const Point &q2)
{
    return Point(q1.x + q2.x, q1.y + q2.y);
}

Point operator-(const Point &q1, const Point &q2)
{
    return Point(q1.x - q2.x, q1.y - q2.y);
}

Point operator*(const Point &q1, double d)
{
    return Point(d * q1.x, d * q1.y);
}

/**
 * @brief Check if queried point is inside obstacle
 *
 * @details Checking cross product between vectors. The vector between point and vertex
 * should lie on right of the corresponding side of polygon (i.e. cross product should be -ve)
 * Note: Polygon vertices need to be defined clockwise or the effect will reverse.
 *
 * @param obstacles
 * @param p
 * @return true
 * @return false
 */
bool isPointInsideObstacle(std::vector<Polygon> obstacles, Point p)
{
    for (auto obstacle : obstacles)
    {
        bool is_inside = true;
        for (int i = 0; i < obstacle.num_points - 1; i++)
        {
            Point v1 = obstacle.points[i + 1] - obstacle.points[i];
            Point v2 = obstacle.points[i + 1] - p;
            // Cross product
            if (v1.x * v2.y - v1.y * v2.x > 0)
            {
                is_inside = false; // It's not inside this polygon
                break;
            }
        }
        // If it proved to pass the polygon interior check everytime,
        // Then point is inside this polygon. Just return true
        if (is_inside)
            return true;
    }
    // If true wasn't returned for any of the polygons above
    return false;
}