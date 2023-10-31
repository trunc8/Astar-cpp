#include <gtest/gtest.h>

#include "utils.h"

TEST(InObstacleTest, InsideAndOutside)
{
    std::vector<Polygon> obstacles;
    std::vector<Node> obstacle = {Node(200, 0), Node(250, 0), Node(250, 400), Node(200, 400), Node(200, 0)};
    obstacles.push_back(Polygon(obstacle, obstacle.size()));

    Node q1{230, 800};
    EXPECT_EQ(false, isNodeInsideObstacle(obstacles, q1));

    Node q2{230, 300};
    EXPECT_EQ(true, isNodeInsideObstacle(obstacles, q2));
}