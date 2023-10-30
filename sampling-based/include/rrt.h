/**
 * @file rrt.h
 * @author Siddharth Saha (sahasiddharth611@gmail.com)
 * @brief Implementation of RRT
 * @version 1.0
 * @date 2023-10-25
 */

#pragma once
#include <vector>
#include <cmath>

struct Node
{
    Point pt;
    Node *parent;
    // std::vector<Node *> children;
    Node() {}
    Node(int x, int y) : pt(Point(x,y)) {}
    Node(Point p) : pt(p) {}
    const Node &operator=(const Node &q)
    {
        this->pt = q.pt;
        this->parent = q.parent;
        return *this;
    }
};

void printNode(const Node& q)
{
    std::cout << "Coordinates: " << q.pt.x << "\t" << q.pt.y << std::endl;
}

double dist(const Node &q1, const Node &q2)
{
    return sqrt(pow(q1.pt.x - q2.pt.x, 2) + pow(q1.pt.y - q2.pt.y, 2));
}

Node operator+(const Node &q1, const Node &q2)
{
    return Node(q1.pt + q2.pt);
}

Node operator-(const Node &q1, const Node &q2)
{
    return Node(q1.pt - q2.pt);
}

Node operator*(const Node &q1, double d)
{
    return Node(q1.pt*d);
}