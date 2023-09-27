/**
 * @file dfs.h
 * @author Siddharth Saha (sahasiddharth611@gmail.com)
 * @brief 
 * @version 1.0
 * @date 2023-09-25
 */
#include "utils.h"
#include <stack>

void backtrackAndPrintPath(Grid visited, Pair src, Pair dest)
{
    int r_size = visited.size(), c_size = visited[0].size();
    Grid path = Grid(r_size, std::vector<int>(c_size, 0));
    std::vector<Pair> backtrack;
    Pair curr = dest;
    path[curr.first][curr.second] = 1;
    backtrack.push_back(curr);
    while (visited[curr.first][curr.second] != 1)
    {
        for (int i = 0; i < 8; i++)
        {
            int new_r = curr.first + dR[i];
            int new_c = curr.second + dC[i];
            if (isWithinBounds(new_r, new_c, r_size, c_size) and visited[new_r][new_c] == visited[curr.first][curr.second] - 1)
            {
                curr = std::make_pair(new_r, new_c);
                path[curr.first][curr.second] = 1;
                backtrack.push_back(curr);
                break;
            }
        }
    }
    printf("\nPath map:\n");
    for (auto &r : path)
    {
        for (auto &i : r)
            std::cout << i << " ";
        std::cout << std::endl;
    }

    printf("\nBacktrack path:\n");
    for (auto it = backtrack.rbegin(); it != backtrack.rend(); it++)
    {
        printf("(%d,%d) -> ", it->first, it->second);
    }
    printf("SUCCESS\n\n");
}

void performDfs(const Grid &map, const Pair &src, const Pair &dest)
{
    // Start timer
    auto start = std::chrono::high_resolution_clock::now();
    int r_size = map.size(), c_size = map[0].size();

    Grid visited = Grid(r_size, std::vector<int>(c_size, 0));
    std::stack<Pair> OPEN;
    OPEN.push(src);
    visited[src.first][src.second] = 1;
    int itr = 0;
    while (!OPEN.empty())
    {
        itr++;
        Pair curr = OPEN.top();
        OPEN.pop();
        for (int i = 0; i < 8; i++)
        {
            int new_r = curr.first + dR[i];
            int new_c = curr.second + dC[i];
            if (isWithinBounds(new_r, new_c, r_size, c_size) and map[new_r][new_c] and !visited[new_r][new_c])
            {
                OPEN.push(std::make_pair(new_r, new_c));
                visited[new_r][new_c] = visited[curr.first][curr.second] + 1;
            }
            if (new_r == dest.first and new_c == dest.second)
            {
                // End timer :)
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

                printf("We reached!!\n\nVisited map:\n");
                for (auto &r : visited)
                {
                    for (auto &i : r)
                        std::cout << i << " ";
                    std::cout << std::endl;
                }
                backtrackAndPrintPath(visited, src, dest);

                printf("%d Iterations, Duration: %f us\n", itr, duration / 1e3);
                return;
            }
        }
    }
    // End timer :(
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    printf("Destination is unreachable :(\n");
    printf("%d Iterations, Duration: %f us\n", itr, duration / 1e3);

    return;
}


void backtrackAndPrintPathPointerMap(
    int *visited,
    int x_size,
    int y_size,
    const Pair &src,
    const Pair &dest
    )
{
    int* path = new int[x_size * y_size];
    std::fill(path, path+x_size * y_size, 0);
    std::vector<Pair> backtrack;
    Pair curr = dest;
    path[GETMAPINDEX(curr.second, curr.first, x_size, y_size)] = 1;
    backtrack.push_back(curr);
    while (visited[GETMAPINDEX(curr.second, curr.first, x_size, y_size)] != 1)
    {
        for (int i = 0; i < 8; i++)
        {
            int newy = curr.first + dY[i];
            int newx = curr.second + dX[i];
            if (isWithinBounds(newy-1, newx-1, y_size, x_size) and visited[GETMAPINDEX(newx, newy, x_size, y_size)] == visited[GETMAPINDEX(curr.second, curr.first, x_size, y_size)] - 1)
            {
                curr = std::make_pair(newy, newx);
                path[GETMAPINDEX(curr.second, curr.first, x_size, y_size)] = 1;
                backtrack.push_back(curr);
                break;
            }
        }
    }
    printf("\nPath map:\n");
    for (size_t y = 1; y <= y_size; y++)
    {
        for (size_t x = 1; x <= x_size; x++)
        {
            printf("%d ", path[GETMAPINDEX(x, y, x_size, y_size)]);
        }
        printf("\n");
    }
    delete path;

    printf("\nBacktrack path:\n");
    for (auto it = backtrack.rbegin(); it != backtrack.rend(); it++)
    {
        printf("(%d,%d) -> ", it->first, it->second);
    }
    printf("SUCCESS\n\n");
}

void performDfsPointerMap(
    int *map,
    int x_size,
    int y_size,
    const Pair &src,
    const Pair &dest)
{
    // Start timer
    auto start = std::chrono::high_resolution_clock::now();

    int *visited = new int[x_size * y_size];
    std::fill(visited, visited+x_size * y_size, 0);
    std::stack<Pair> OPEN;
    OPEN.push(src);
    visited[GETMAPINDEX(src.second, src.first, x_size, y_size)] = 1;
    int itr = 0;
    while (!OPEN.empty())
    {
        itr++;
        Pair curr = OPEN.top();
        // curr.first = curr.first;
        // curr.second = curr.second;
        OPEN.pop();
        for (int i = 0; i < 8; i++)
        {

            int newy = curr.first + dR[i];
            int newx = curr.second + dC[i];
            int roboty = curr.first;
            int robotx = curr.second;
            if (isWithinBounds(newy - 1, newx - 1, y_size, x_size) and map[GETMAPINDEX(newx, newy, x_size, y_size)] and !visited[GETMAPINDEX(newx, newy, x_size, y_size)])
            {
                OPEN.push(std::make_pair(newy, newx));
                visited[GETMAPINDEX(newx, newy, x_size, y_size)] = visited[GETMAPINDEX(robotx, roboty, x_size, y_size)] + 1;
            }
            if (newy == dest.first and newx == dest.second)
            {
                // End timer :)
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

                printf("We reached!!\n\nVisited map:\n");
                for (size_t y = 1; y <= y_size; y++)
                {
                    for (size_t x = 1; x <= x_size; x++)
                    {
                        printf("%d ", visited[GETMAPINDEX(x, y, x_size, y_size)]);
                    }
                    printf("\n");
                }
                backtrackAndPrintPathPointerMap(visited, x_size, y_size, src, dest);

                printf("%d Iterations, Duration: %f us\n", itr, duration / 1e3);
                delete visited;
                return;
            }
        }
    }
    // End timer :(
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    printf("Destination is unreachable :(\n");
    printf("%d Iterations, Duration: %f us\n", itr, duration / 1e3);

    delete visited;
    return;
}