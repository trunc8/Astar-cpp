/**
 * @file 3dastar.h
 * @author Siddharth Saha (sahasiddharth611@gmail.com)
 * @brief
 * @version 1.0
 * @date 2023-09-27
 */

#include "utils.h"
#include <unordered_map>
#include <unordered_set>
#include <functional>

struct Node
{
    int x, y, t;
    bool operator==(const Node &n)
    {
        return this->x == n.x and this->y == n.y and this->t == n.t;
    }
};

bool isWithinBounds3D(int r, int c, int t, int r_size, int c_size, int t_size)
{
    return r >= 0 && r < r_size && c >= 0 && c < c_size && t >= 0 && t <= t_size;
}

// Reference: https://stackoverflow.com/a/54690748
using h = std::hash<int>;
auto hash = [](const Node &n)
{ return ((17 * 31 + h()(n.x)) * 31 + h()(n.y)) * 31 + h()(n.t); };
auto equal = [](const Node &l, const Node &r)
{ return l.x == r.x && l.y == r.y && l.t == r.t; };

void backtrackPath3D(
    const Grid &map,
    std::unordered_map<Node, int, decltype(hash), decltype(equal)> g,
    const Node &src_node,
    const Node &dest_node,
    int target_steps)
{
    auto getgval = [&g](const Node &n)
    {
        return g[{n.x, n.y, n.t}];
    };

    if (g.count(dest_node) == 0)
    {
        printf("Not possible to reach target at any time step :(\n");
        return;
    }

    printf("Dest node: %d, %d at t=%d with cost: %d\n", dest_node.y, dest_node.x, dest_node.t, g[dest_node]);

    std::vector<Node> backtrack;
    backtrack.push_back(dest_node);
    int y_size = map.size(), x_size = map[0].size();

    Node curr_node = dest_node;
    while (!(curr_node == src_node))
    {
        for (int i = 0; i < 9; i++)
        {
            int newx = curr_node.x + dX[i];
            int newy = curr_node.y + dY[i];
            int newt = curr_node.t - 1;
            if (isWithinBounds3D(newy, newx, newt, y_size, x_size, target_steps) and map[newy][newx])
            {
                Node new_node = {newx, newy, newt};
                if (g.count(new_node) and getgval(curr_node) == getgval(new_node) + map[curr_node.y][curr_node.x])
                {
                    // printf("Next node: %d, %d at t=%d\n", new_node.y, new_node.x, new_node.t);
                    backtrack.push_back(new_node);
                    curr_node = new_node;
                    break;
                }
            }
        }
    }
    printf("\nBacktrack path:\n");
    for (auto it = backtrack.rbegin(); it != backtrack.rend(); it++)
    {
        printf("(%d,%d) at t=%d,\n", it->y, it->x, it->t);
    }
    printf("SUCCESS\n\n");
}

void aStarSearch3D(
    const Grid &map,
    const Pair &src,
    const std::vector<Pair> &target_traj)
{
    // Timing the whole function execution time
    auto start = std::chrono::high_resolution_clock::now();

    // Defining the size variables
    int y_size = map.size(), x_size = map[0].size();
    int target_steps = target_traj.size() - 1;

    // A star Book-keeping: gvalue dictionary and visited set
    std::unordered_map<Node, int, decltype(hash), decltype(equal)> g(8, hash, equal);
    std::unordered_set<Node, decltype(hash), decltype(equal)> visited(8, hash, equal);

    // Initializing nodes
    Node src_node = {src.second, src.first, 0};
    Node dest_node = {target_traj[target_steps].second, target_traj[target_steps].first, target_steps};
    Node goal_node = {-1, -1, -1};
    // Goal node is a special node that is connected to all target traj nodes. Goal node has 0 map cost;
    std::vector<Node> target_traj_nodes(target_steps + 1);
    for (int i = 0; i <= target_steps; i++)
    {
        target_traj_nodes[i] = {target_traj[i].second, target_traj[i].first, i};
    }

    // Defining lambda functions for convenience
    auto getgval = [&g](const Node &n)
    {
        return g[{n.x, n.y, n.t}];
    };
    auto gethval = [&getgval](const Node &n)
    {
        return getgval(n); // epsilon = 0
        // TODO: Add heuristic
    };

    // A* Bookkeeping: OPEN list as a priority queue
    auto cmp = [&gethval](const Node &n1, const Node &n2)
    {
        return gethval(n1) > gethval(n2);
    };
    std::priority_queue<Node, std::vector<Node>, decltype(cmp)> OPEN(cmp);

    // Start A* by adding src node to OPEN and g
    OPEN.push(src_node);
    g[src_node] = map[src.first][src.second];

    int itr = 0;
    while (!OPEN.empty())
    {
        itr++;
        Node curr_node = OPEN.top();
        OPEN.pop();
        visited.insert(curr_node);
        if (curr_node == goal_node)
            break;

        // Special motion primitive for the target trajectory nodes
        if (curr_node == target_traj_nodes[curr_node.t])
        {
            if (visited.find(goal_node) == visited.end())
            {
                if (g.count(goal_node) == 0 or getgval(curr_node) < getgval(goal_node))
                {
                    g[goal_node] = getgval(curr_node);
                    OPEN.push(goal_node);
                    continue;
                }
            }
        }

        for (int i = 0; i < 9; i++)
        {
            int newx = curr_node.x + dX[i];
            int newy = curr_node.y + dY[i];
            int newt = curr_node.t + 1;
            Node new_node = {newx, newy, newt};
            if (isWithinBounds3D(newy, newx, newt, y_size, x_size, target_steps) and map[newy][newx] and visited.find(new_node) == visited.end())
            {
                int new_gvalue = map[newy][newx] + getgval(curr_node);
                if (g.count(new_node) == 0 or new_gvalue < getgval(new_node))
                {
                    g[new_node] = new_gvalue;
                    OPEN.push(new_node);
                }
            }
        }
    }
    // for(int t=0; t<=target_steps; t++)
    // {
    //     printf("\n Time step: %d\n", t);
    //     for(int y=0; y<y_size; y++){
    //         for(int x=0; x<x_size; x++){
    //             if(g.count({y,x,t}))
    //                 printf("%d ", g[{y,x,t}]);
    //             else
    //                 printf("0 ");
    //         }
    //         printf("\n");
    //     }
    // }

    backtrackPath3D(map, g, src_node, dest_node, target_steps);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    printf("%d Iterations, Duration: %f us\n", itr, duration / 1e3);
    return;
}

// struct node_hash{
//     std::size_t operator()(const node& n) const noexcept
//     {
//         std::size_t h1 = std::hash<int>{}(n.x);
//         std::size_t h2 = std::hash<int>{}(n.y);
//         std::size_t h3 = std::hash<int>{}(n.t);
//         return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 2); // or use boost::hash_combine
//     }
// };