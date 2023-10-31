/**
 * @file utils.h
 * @author Siddharth Saha (sahasiddharth611@gmail.com)
 * @brief Utility declarations
 * @version 1.0
 * @date 2023-10-25
 */
#pragma once

#include <vector>
#include <iostream>

#include <algorithm>
#include <random>
#include <limits.h>
#include <cmath>

struct Node
{
    double x, y;
    Node *parent;
    // std::vector<Node *> children;
    Node() {}
    Node(double x, double y) : x(x), y(y) {}
    const Node &operator=(const Node &q)
    {
        this->x = q.x;
        this->y = q.y;
        this->parent = q.parent;
        return *this;
    }
};

void printNode(const Node& q);

double dist(const Node &q1, const Node &q2);

Node operator+(const Node &q1, const Node &q2);

Node operator-(const Node &q1, const Node &q2);

Node operator*(const Node &q1, double d);

struct Polygon
{
    std::vector<Node> nodes; // Assumes clockwise/anti-clockwise nodes input
    int num_nodes = 0;

    Polygon() {}
    Polygon(std::vector<Node> nodes, int size) : nodes(nodes), num_nodes(size) {}
};

bool isNodeInsideObstacle(std::vector<Polygon> obstacles, Node q);
