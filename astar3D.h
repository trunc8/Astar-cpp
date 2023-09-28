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

Grid getDjikstraAsHeuristic(const Grid &map, const std::vector<Pair> &target_traj)
{
    int r_size = map.size(), c_size = map[0].size();

    Grid visited = Grid(r_size, std::vector<int>(c_size, 0));

    Grid g = Grid(r_size, std::vector<int>(c_size, INT_MAX));
    
    auto getgval = [&g](const Pair &p)
    {
        return g[p.first][p.second];
    };
    auto cmp = [&g, &getgval](const Pair &p1, const Pair &p2)
    {
        return getgval(p1) > getgval(p2);
    };
    std::priority_queue<Pair, std::vector<Pair>, decltype(cmp)> OPEN(cmp);

    for(auto &p: target_traj)
    {
        g[p.first][p.second] = map[p.first][p.second];
        OPEN.push(p);
    }

    int itr = 0;
    while (!OPEN.empty())
    {
        itr++;
        Pair curr = OPEN.top();
        OPEN.pop();

        visited[curr.first][curr.second] = 1;
        for (int i = 0; i < 8; i++)
        {
            int new_r = curr.first + dR[i];
            int new_c = curr.second + dC[i];
            if (isWithinBounds(new_r, new_c, r_size, c_size) and map[new_r][new_c] and !visited[new_r][new_c])
            {
                Pair next = std::make_pair(new_r, new_c);
                int new_gvalue = map[new_r][new_c] + getgval(curr);
                if (new_gvalue < getgval(next))
                {
                    g[new_r][new_c] = new_gvalue;
                    OPEN.push(next);
                }
            }
        }
    }
    return g;
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

    Grid djikstra_as_heuristic = getDjikstraAsHeuristic(map, target_traj);
    // Grid djikstra_as_heuristic = Grid(y_size, std::vector<int>(x_size, 0));

    // Defining lambda functions for convenience
    auto getgval = [&g](const Node &n)
    {
        return g[{n.x, n.y, n.t}];
    };
    
    int epsilon = 1; // weight for A*
    auto gethval = [&](const Node &n)
    {
        if(isWithinBounds(n.y, n.x, y_size, x_size))
            return getgval(n) + epsilon*djikstra_as_heuristic[n.y][n.x];
        else
            return getgval(n);
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
                    dest_node = curr_node;
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

    backtrackPath3D(map, g, src_node, dest_node, target_steps);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    printf("%d Iterations, Duration: %f us\n", itr, duration / 1e3);
    return;
}









void backtrackPath3DPointerMap(
    int *map,
    std::unordered_map<Node, int, decltype(hash), decltype(equal)> g,
    int x_size,
    int y_size,
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

    Node curr_node = dest_node;
    while (!(curr_node == src_node))
    {
        for (int i = 0; i < 9; i++)
        {
            int newx = curr_node.x + dX[i];
            int newy = curr_node.y + dY[i];
            int newt = curr_node.t - 1;
            if (isWithinBounds3D(newy-1, newx-1, newt, y_size, x_size, target_steps) and map[GETMAPINDEX(newx, newy, x_size, y_size)])
            {
                Node new_node = {newx, newy, newt};
                // printf("Next node: %d, %d at t=%d\n", new_node.y, new_node.x, new_node.t);
                // printf("check: %d\n", g.count(new_node));
                if (g.count(new_node) and getgval(curr_node) == getgval(new_node) + map[GETMAPINDEX(curr_node.x, curr_node.y, x_size, y_size)])
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


int* getDjikstraAsHeuristicPointerMap(
    int *map,
    int x_size,
    int y_size,
    const std::vector<Pair> &target_traj
    )
{

    int *visited = new int[x_size * y_size];
    std::fill(visited, visited + x_size * y_size, 0);
    int *g = new int[x_size * y_size];
    std::fill(g, g + x_size * y_size, INT_MAX);
    
    auto getgval = [&](const Pair &p)
    {
        return g[GETMAPINDEX(p.second, p.first, x_size, y_size)];
    };
    auto cmp = [&g, &getgval](const Pair &p1, const Pair &p2)
    {
        return getgval(p1) > getgval(p2);
    };
    std::priority_queue<Pair, std::vector<Pair>, decltype(cmp)> OPEN(cmp);

    for(auto &p: target_traj)
    {
        g[GETMAPINDEX(p.second, p.first, x_size, y_size)] = map[GETMAPINDEX(p.second, p.first, x_size, y_size)];
        OPEN.push(p);
    }

    int itr = 0;
    while (!OPEN.empty())
    {
        itr++;
        Pair curr = OPEN.top();
        OPEN.pop();

        visited[GETMAPINDEX(curr.second, curr.first, x_size, y_size)] = 1;
        for (int i = 0; i < 8; i++)
        {
            int newy = curr.first + dY[i];
            int newx = curr.second + dX[i];
            if (isWithinBounds(newy - 1, newx - 1, y_size, x_size) and map[GETMAPINDEX(newx, newy, x_size, y_size)] and !visited[GETMAPINDEX(newx, newy, x_size, y_size)])
            {
                Pair next = std::make_pair(newy, newx);
                int new_gvalue = map[GETMAPINDEX(newx, newy, x_size, y_size)] + getgval(curr);
                if (new_gvalue < getgval(next))
                {
                    g[GETMAPINDEX(newx, newy, x_size, y_size)] = new_gvalue;
                    OPEN.push(next);
                }
            }
        }
    }
    return g;
}


void aStarSearch3DPointerMap(
    int *map,
    int x_size,
    int y_size,
    const Pair &src,
    const std::vector<Pair> &target_traj)
{
    // Timing the whole function execution time
    auto start = std::chrono::high_resolution_clock::now();

    // Defining the size variables
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

    int* djikstra_as_heuristic = getDjikstraAsHeuristicPointerMap(map, x_size, y_size, target_traj);

    // Defining lambda functions for convenience
    auto getgval = [&g](const Node &n)
    {
        return g[{n.x, n.y, n.t}];
    };
    
    int epsilon = 1; // weight for A*
    auto gethval = [&](const Node &n)
    {
        if(isWithinBounds(n.y-1, n.x-1, y_size, x_size))
            return getgval(n) + epsilon*djikstra_as_heuristic[GETMAPINDEX(n.x, n.y, x_size, y_size)];
        else
            return getgval(n);
    };

    // A* Bookkeeping: OPEN list as a priority queue
    auto cmp = [&gethval](const Node &n1, const Node &n2)
    {
        return gethval(n1) > gethval(n2);
    };
    std::priority_queue<Node, std::vector<Node>, decltype(cmp)> OPEN(cmp);

    // Start A* by adding src node to OPEN and g
    OPEN.push(src_node);
    g[src_node] = map[GETMAPINDEX(src_node.x, src_node.y, x_size, y_size)];

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
                    dest_node = curr_node;
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
            if (isWithinBounds3D(newy-1, newx-1, newt, y_size, x_size, target_steps) and map[GETMAPINDEX(newx, newy, x_size, y_size)] and visited.find(new_node) == visited.end())
            {
                int new_gvalue = map[GETMAPINDEX(newx, newy, x_size, y_size)] + getgval(curr_node);
                if (g.count(new_node) == 0 or new_gvalue < getgval(new_node))
                {
                    g[new_node] = new_gvalue;
                    OPEN.push(new_node);
                }
            }
        }
    }

    backtrackPath3DPointerMap(map, g, x_size, y_size, src_node, dest_node, target_steps);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    printf("%d Iterations, Duration: %f us\n", itr, duration / 1e3);
    return;
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


// struct node_hash{
//     std::size_t operator()(const node& n) const noexcept
//     {
//         std::size_t h1 = std::hash<int>{}(n.x);
//         std::size_t h2 = std::hash<int>{}(n.y);
//         std::size_t h3 = std::hash<int>{}(n.t);
//         return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 2); // or use boost::hash_combine
//     }
// };