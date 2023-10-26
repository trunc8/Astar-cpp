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
    int x, y;
    Node *parent;
    // std::vector<Node *> children;
    Node() {}
    Node(int x, int y) : x(x), y(y) {}
    // double dist(Node* q2)
    // {
    //     return sqrt(pow(this->x - q2->x, 2) + pow(this->y - q2->y, 2));
    // }
    // Node operator+(const Node &q1, const Node &q2);
    const Node &operator=(const Node &q)
    {
        this->x = q.x;
        this->y = q.y;
        return *this;
    }
};

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
    return Node(d*q1.x, d*q1.y);
}