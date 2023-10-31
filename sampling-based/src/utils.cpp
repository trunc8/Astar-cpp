/**
 * @file utils.cpp
 * @author Siddharth Saha (sahasiddharth611@gmail.com)
 * @brief Utility defintions
 * @version 1.0
 * @date 2023-10-25
 */
#include "utils.h"

void printNode(const Node& q)
{
    std::cout << "Coordinates: " << q.x << "\t" << q.y << std::endl;
}

double dist(const Node &q1, const Node &q2)
{
    return sqrt(pow(q1.x - q2.x, 2) + pow(q1.y - q2.y, 2));
}

Node operator+(const Node &q1, const Node &q2)
{
    return Node(q1.x + q2.x, q1.y + q2.y);
}

Node operator-(const Node &q1, const Node &q2)
{
    return Node(q1.x - q2.x, q1.y - q2.y);
}

Node operator*(const Node &q1, double d)
{
    return Node(d * q1.x, d * q1.y);
}

/**
 * @brief Check if queried node is inside obstacle
 *
 * @details Checking cross product between vectors. The vector between node and vertex
 * should lie on right of the corresponding side of polygon (i.e. cross product should be -ve)
 * Note: Polygon vertices need to be defined clockwise or the effect will reverse.
 *
 * @param obstacles
 * @param p
 * @return true
 * @return false
 */
bool isNodeInsideObstacle(std::vector<Polygon> obstacles, Node q)
{
    for (auto obstacle : obstacles)
    {
        bool is_inside = true;
        for (int i = 0; i < obstacle.num_nodes - 1; i++)
        {
            Node v1 = obstacle.nodes[i + 1] - obstacle.nodes[i];
            Node v2 = obstacle.nodes[i + 1] - q;
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