/**
 * @file astar.h
 * @author Siddharth Saha (sahasiddharth611@gmail.com)
 * @brief
 * @version 1.0
 * @date 2023-09-25
 */

#include "utils.h"

void aStarSearch(const Grid &map, const Pair &src, const Pair &dest)
{
    // Start timer
    auto start = std::chrono::high_resolution_clock::now();
    int r_size = map.size(), c_size = map[0].size();

    Grid visited = Grid(r_size, std::vector<int>(c_size, 0));

    Grid g = Grid(r_size, std::vector<int>(c_size, INT_MAX));
    g[src.first][src.second] = map[src.first][src.second];

    auto getgval = [&g](const Pair &p)
    {
        return g[p.first][p.second];
    };
    auto gethval = [&getgval, &dest](const Pair& p){
        return getgval(p) + std::max(abs(p.second-dest.second), abs(p.first-dest.first));
    };
    auto cmp = [&g, &gethval](const Pair &p1, const Pair &p2)
    {
        return gethval(p1) > gethval(p2);
    };
    std::priority_queue<Pair, std::vector<Pair>, decltype(cmp)> OPEN(cmp);
    OPEN.push(src);

    int itr = 0;
    while (!visited[dest.first][dest.second] and !OPEN.empty())
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

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    printf("\nVisited map:\n");
    for (auto &r : visited)
    {
        for (auto &i : r)
            printf("%d ", i);
        printf("\n");
    }
    printf("\ng-value map:\n");
    for (auto &r : g)
    {
        for (auto &i : r)
        {
            printf("%d ", i == INT_MAX ? 0 : i);
        }
        printf("\n");
    }

    backtrackPath(map, g, src, dest);

    printf("%d Iterations, Duration: %f us\n", itr, duration / 1e3);
    return;
}


void aStartSearchPointerMap(
    int *map,
    int x_size,
    int y_size,
    const Pair &src,
    const Pair &dest
)
{
    // Start timer
    auto start = std::chrono::high_resolution_clock::now();

    int *visited = new int[x_size * y_size];
    std::fill(visited, visited+x_size * y_size, 0);
    int *g = new int[x_size * y_size];
    std::fill(g, g+x_size * y_size, INT_MAX);

    // Grid g = Grid(y_size, std::vector<int>(x_size, INT_MAX));
    g[GETMAPINDEX(src.second, src.first, x_size, y_size)] = map[GETMAPINDEX(src.second, src.first, x_size, y_size)];


    auto getgval = [&](const Pair& p){
        return g[GETMAPINDEX(p.second, p.first, x_size, y_size)];
    };

    auto gethval = [&getgval, &dest](const Pair& p){
        return getgval(p) + std::max(abs(p.second-dest.second), abs(p.first-dest.first));
    };

    auto cmp = [&gethval](const Pair& p1, const Pair& p2){
         return gethval(p1) > gethval(p2);
    };

    std::priority_queue<Pair, std::vector<Pair>, decltype(cmp)> OPEN(cmp);
    OPEN.push(src);

    int itr = 0;
    while (!visited[GETMAPINDEX(dest.second, dest.first, x_size, y_size)] and !OPEN.empty())
    {
        itr++;
        Pair curr = OPEN.top();
        visited[GETMAPINDEX(curr.second, curr.first, x_size, y_size)] = 1;
        OPEN.pop();
        for (int i = 0; i < 8; i++)
        {
            int newy = curr.first + dY[i];
            int newx = curr.second + dX[i];
            if (isWithinBounds(newy - 1, newx - 1, y_size, x_size) and map[GETMAPINDEX(newx, newy, x_size, y_size)] and !visited[GETMAPINDEX(newx, newy, x_size, y_size)])
            {
                Pair next = std::make_pair(newy, newx);
                int new_gvalue = map[GETMAPINDEX(newx, newy, x_size, y_size)] + getgval(curr);
                if(new_gvalue < getgval(next)){
                    g[GETMAPINDEX(newx, newy, x_size, y_size)] = new_gvalue;
                    OPEN.push(next);
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    

    printf("\nVisited map:\n");
    for (size_t y = 1; y <= y_size; y++)
    {
        for (size_t x = 1; x <= x_size; x++)
        {
            printf("%d ", visited[GETMAPINDEX(x, y, x_size, y_size)]);
        }
        printf("\n");
    }
    printf("\ng-value map:\n");
    for (size_t y = 1; y <= y_size; y++)
    {
        for (size_t x = 1; x <= x_size; x++)
        {
            Pair p({y,x});
            printf("%d ", getgval(p) == INT_MAX ? 0 : getgval(p));
        }
        printf("\n");
    }

    backtrackPathPointerMap(map, visited, g, x_size, y_size, src, dest);
    

    printf("\n%d Iterations, Duration: %f us\n", itr, duration / 1e3);

    delete visited;
    return;
}