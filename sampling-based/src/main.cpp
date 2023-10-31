/**
 * @file main.cpp
 * @author Siddharth Saha (sahasiddharth611@gmail.com)
 * @brief Main function to test the SFML environment
 * @version 1.0
 * @date 2023-10-25
 */

#include <SFML/Graphics.hpp>
#include "utils.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const int RADIUS = 1;

Node start, stop;
std::vector<Node> nodes;

std::vector<Polygon> obstacles;
std::vector<sf::ConvexShape> polygons;
std::vector<Node> node_list;

sf::CircleShape startingNode;
sf::CircleShape endingNode;

std::random_device rd;                                                 // obtain a random number from hardware
std::mt19937 gen(rd());                                                // seed the generator
std::uniform_int_distribution<> distr_w(0, WIDTH), distr_h(0, HEIGHT); // define the range

const int EPS = 20;
const double GOAL_BIAS = 0.1;

enum status
{
    REACHED,
    ADVANCED,
    TRAPPED
};

void init()
{
    start = Node(100, 20);
    stop = Node(500, 20);

    startingNode.setPosition(start.x, start.y);
    endingNode.setPosition(stop.x, stop.y);
    startingNode.setRadius(5 * RADIUS);
    endingNode.setRadius(5 * RADIUS);
    startingNode.setFillColor(sf::Color(255, 0, 255));
    endingNode.setFillColor(sf::Color(0, 255, 0));

    std::vector<Node> obstacle = {Node(200, 0), Node(250, 0), Node(250, 400), Node(200, 400), Node(200, 0)};
    obstacles.push_back(Polygon(obstacle, obstacle.size()));

    polygons.resize(obstacles.size());
    for (int i = 0; i < obstacles.size(); i++)
    {
        polygons[i].setPointCount(obstacles[i].num_nodes);
        polygons[i].setFillColor(
            sf::Color(0, 0, 125));
        for (int j = 0; j < obstacles[i].num_nodes; j++)
        {
            polygons[i].setPoint(j, sf::Vector2f(obstacles[i].nodes[j].x, obstacles[i].nodes[j].y));
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

    if (isNodeInsideObstacle(obstacles, q_new))
    {
        std::cout << "Inside obstacle" << std::endl;
        return TRAPPED;
    }

    q_new.parent = new Node(q_near.x, q_near.y);
    node_list.push_back(q_new);
    if (dist(q_new, q_goal) < EPS)
    {
        Node q_final = q_goal;
        q_final.parent = new Node(q_new.x, q_new.y);
        node_list.push_back(q_final);
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

void draw(sf::RenderWindow &window, std::vector<Node> &node_list)
{
    sf::CircleShape nodeCircle;

    // Draw obstacles
    for (auto &poly : polygons)
        window.draw(poly);

    for (int i = 0; i < node_list.size(); i++)
    {
        Node *parent_node = node_list[i].parent;
        nodeCircle.setPosition(node_list[i].x, node_list[i].y);
        nodeCircle.setRadius(RADIUS);
        nodeCircle.setFillColor(sf::Color(220, 220, 0));
        window.draw(nodeCircle);

        if (parent_node != nullptr)
        {
            const std::array<sf::Vertex, 2> line =
                {sf::Vertex(sf::Vector2f(node_list[i].x, node_list[i].y), sf::Color::Red),
                 sf::Vertex(sf::Vector2f(parent_node->x, parent_node->y), sf::Color::Red)};

            window.draw(line.data(), line.size(), sf::Lines, sf::RenderStates::Default);
        }
    }

    window.draw(startingNode);
    window.draw(endingNode);
}

status runRRT(sf::RenderWindow &window, std::vector<Node> &node_list)
{
    Node q_init{start}, q_goal{stop};
    Node q_rand;
    q_init.parent = nullptr;
    int K = 1000;

    node_list.push_back(q_init);
    for (int k = 0; k < K; k++)
    {
        // RANDOM_CONFIG
        if (rand() * 1.0 / RAND_MAX < GOAL_BIAS)
            q_rand = q_goal;
        else
            q_rand = Node(distr_w(gen), distr_h(gen));
        status s = EXTEND(node_list, q_rand, q_goal);
        window.clear();
        draw(window, node_list);
        window.display();
        if (s == REACHED)
            return s;
    }
    return TRAPPED;
}

status runRRTIteration(sf::RenderWindow &window, std::vector<Node> &node_list, const Node &q_init, const Node &q_goal)
{
    Node q_rand;
    if (rand() * 1.0 / RAND_MAX < GOAL_BIAS)
        q_rand = q_goal;
    else
        q_rand = Node(distr_w(gen), distr_h(gen));
    status s = EXTEND(node_list, q_rand, q_goal);
    window.clear();
    draw(window, node_list);
    window.display();
    if (s == REACHED)
        return s;
    return TRAPPED;
}

int main()
{
    sf::RenderWindow window{{WIDTH, HEIGHT}, "Sampling Planner"};
    window.setFramerateLimit(1000);

    init();

    Node q_init{start}, q_goal{stop};

    q_init.parent = nullptr;
    int K = 1000;
    int k = 0;

    node_list.push_back(q_init);
    status s = ADVANCED;

    while (window.isOpen())
    {
        k++;
        if (s != REACHED and k < K)
        {
            s = runRRTIteration(window, node_list, q_init, q_goal);
        }
        window.clear();
        draw(window, node_list);
        window.display();
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return 0;
                exit(0);
            }
        }
    }
}