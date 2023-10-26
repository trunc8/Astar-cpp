#include <gtest/gtest.h>

#include "utils.h"

TEST(InObstacleTest, InsideAndOutside)
{
    std::vector<Polygon> obstacles;
    std::vector<Point> obstacle = {Point(200, 0), Point(250, 0), Point(250, 400), Point(200, 400), Point(200, 0)};
    obstacles.push_back(Polygon(obstacle, obstacle.size()));

    Point p1{230, 800};
    EXPECT_EQ(false, isPointInsideObstacle(obstacles, p1));

    Point p2{230, 300};
    EXPECT_EQ(true, isPointInsideObstacle(obstacles, p2));
}