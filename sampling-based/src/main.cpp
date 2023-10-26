/**
 * @file main.cpp
 * @author Siddharth Saha (sahasiddharth611@gmail.com)
 * @brief Main function to test the SFML environment
 * @version 1.0
 * @date 2023-10-25
 */

#include <SFML/Graphics.hpp>
#include "utils.h"
#include "rrt.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const int RADIUS = 3;

Point start, stop;
std::vector<Point> nodes;

std::vector<Polygon> obstacles;
std::vector<sf::ConvexShape> polygons;

sf::CircleShape startingPoint;
sf::CircleShape endingPoint;

std::random_device rd;                                                 // obtain a random number from hardware
std::mt19937 gen(rd());                                                // seed the generator
std::uniform_int_distribution<> distr_w(0, WIDTH), distr_h(0, HEIGHT); // define the range

const int EPS = 20;
std::vector<Node> node_list;

enum status
{
    REACHED,
    ADVANCED,
    TRAPPED
};

void init()
{
    start = Point(100, 20);
    stop = Point(500, 20);

    startingPoint.setPosition(start.x, start.y);
    endingPoint.setPosition(stop.x, stop.y);
    startingPoint.setRadius(RADIUS);
    endingPoint.setRadius(RADIUS);
    startingPoint.setFillColor(sf::Color(120, 0, 0));
    endingPoint.setFillColor(sf::Color(0, 255, 0));

    std::vector<Point> obstacle = {Point(200, 0), Point(250, 0), Point(250, 400), Point(200, 400), Point(200, 0)};
    obstacles.push_back(Polygon(obstacle, obstacle.size()));

    polygons.resize(obstacles.size());
    for (int i = 0; i < obstacles.size(); i++)
    {
        polygons[i].setPointCount(obstacles[i].num_points);
        polygons[i].setFillColor(
            sf::Color(0, 0, 125));
        for (int j = 0; j < obstacles[i].num_points; j++)
        {
            polygons[i].setPoint(j, sf::Vector2f(obstacles[i].points[j].x, obstacles[i].points[j].y));
        }
    }
}

void NEW_CONFIG(const Node &q_rand, const Node &q_near, Node &q_new)
{
    q_new = q_near + (q_rand - q_near) * (EPS / dist(q_rand, q_near));
}

status EXTEND(std::vector<Node> &node_list, Node &q_rand, const Node &q_goal)
{
    Node q_near;
    double nearest_dist = INT_MAX;
    // NEAREST_NEIGHBOR
    for (auto &q : node_list)
    {
        if (nearest_dist > dist(q_rand, q))
        {
            nearest_dist = dist(q_rand, q);
            q_near = q;
        }
    }
    Node q_new;
    if (nearest_dist < EPS)
    {
        q_new = q_near;
    }
    else
    {
        NEW_CONFIG(q_rand, q_near, q_new);
    }

    if (isPointInsideObstacle(obstacles, q_new.pt))
    {
        std::cout << "Inside obstacle" << std::endl;
        return TRAPPED;
    }

    node_list.push_back(q_new);
    q_new.parent = &q_near;
    if (dist(q_new, q_goal) < EPS)
    {
        std::cout << "Reached" << std::endl;
        return REACHED;
    }
    else
    {
        std::cout << node_list.size() << std::endl;
        std::cout << "Advanced" << std::endl;
        return ADVANCED;
    }

    std::cout << "Trapped" << std::endl;
    return TRAPPED;
}

void draw(sf::RenderWindow &window)
{
    sf::CircleShape nodeCircle;

    // Draw obstacles
    for (auto &poly : polygons)
        window.draw(poly);

    for (int i = 0; i < node_list.size(); i++)
    {
        nodeCircle.setPosition(node_list[i].pt.x, node_list[i].pt.y);
        nodeCircle.setRadius(RADIUS);
        nodeCircle.setFillColor(sf::Color(220, 220, 0));
        window.draw(nodeCircle);
    }

    window.draw(startingPoint);
    window.draw(endingPoint);

    sf::Vertex line[2];
    line[0].position = sf::Vector2f(start.x, start.y);
    line[0].color = sf::Color::Red;
    line[1].position = sf::Vector2f(stop.x, stop.y);
    line[1].color = sf::Color::Red;

    // sf::Vertex line[] = {{{x1, y1}, color}, {{x2, y2}, color}};
    // window.draw(line);
}

status runRRT(sf::RenderWindow &window)
{
    Node q_init{start}, q_goal{stop};
    q_init.parent = nullptr;
    int K = 1000;

    node_list.push_back(q_init);
    for (int k = 0; k < K; k++)
    {
        // RANDOM_CONFIG
        Node q_rand(distr_w(gen), distr_h(gen));
        status s = EXTEND(node_list, q_rand, q_goal);
        window.clear();
        draw(window);
        window.display();
        if (s == REACHED)
            return s;
    }
    return TRAPPED;
}

int main()
{
    sf::RenderWindow window{{WIDTH, HEIGHT}, "Longjump Project"};
    window.setFramerateLimit(50);

    init();

    status s = runRRT(window);

    // while (window.isOpen())
    // {
    //     sf::CircleShape shape(50.f);
    //     shape.setFillColor(sf::Color(100, 250, 50));
    //     window.draw(shape);
    //     for (auto event = sf::Event{}; window.pollEvent(event);)
    //     {
    //         if (event.type == sf::Event::Closed)
    //         {
    //             window.close();
    //             return 0;
    //             exit(0);
    //         }
    //     }

    //     window.clear();
    //     draw(window);
    //     window.display();
    // }
}