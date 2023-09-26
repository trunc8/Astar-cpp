/**
 * @file dijkstra.h
 * @author Siddharth Saha (sahasiddharth611@gmail.com)
 * @brief 
 * @version 1.0
 * @date 2023-09-25
 */

#include "utils.h"
#include <queue>
#include <limits.h>



void dijkstra(const Grid &map, const Pair &src, const Pair &dest)
{
    // Start timer
    auto start = std::chrono::high_resolution_clock::now();
    int r_size = map.size(), c_size = map[0].size();

    Grid visited = Grid(r_size, std::vector<int>(c_size, 0));

    Grid g = Grid(r_size, std::vector<int>(c_size, INT_MAX));
    g[src.first][src.second] = map[src.first][src.second];

    auto cmp = [&g](const Pair& p1, const Pair& p2){
         return g[p1.first][p1.second] > g[p2.first][p2.second];
    };
    std::priority_queue<Pair, std::vector<Pair>, decltype(cmp)> OPEN(cmp);
    OPEN.push(src);
    auto getgval = [&g](const Pair& p){
        return g[p.first][p.second];
    };

    // std::priority_queue<std::pair<int, Pair>, std::vector<std::pair<int, Pair>>, std::greater<std::pair<int, Pair>>> OPEN;

    // OPEN.push(std::make_pair(getgval(src),src));

    int itr = 0;
    while(!OPEN.empty())
    {
        itr++;
        // int min_so_far = OPEN.top().first;
        // Pair curr = OPEN.top().second;
        Pair curr = OPEN.top();
        
        OPEN.pop();
        // if(getgval(curr) < min_so_far )
        //     continue;
        if(visited[curr.first][curr.second] == 1){
            continue;
            printf("Saved an iteration\n");
        }
        visited[curr.first][curr.second] = 1;
        for (int i=0; i<8; i++)
        {
            int new_r = curr.first + dR[i];
            int new_c = curr.second + dC[i];
            if (isWithinBounds(new_r, new_c, r_size, c_size) and map[new_r][new_c] and !visited[new_r][new_c])
            {
                Pair next = std::make_pair(new_r, new_c);
                int new_gvalue = map[new_r][new_c] + getgval(curr);
                if(getgval(next)<10000 and new_gvalue < getgval(next))
                    printf("%d\n", getgval(next));
                if(new_gvalue < getgval(next)){
                    g[new_r][new_c] = new_gvalue;
                    // OPEN.push(std::make_pair(getgval(next), next));
                    OPEN.push(next);
                }
            }
        }
        // if(itr==3) break;
    }
    int size = OPEN.size();
    for(int i = 0; i < size; ++i) {
        printf("%d, %d: %d\n", OPEN.top().first, OPEN.top().second, getgval(OPEN.top()));
        OPEN.pop();
    }
    printf("\n");

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
    
    printf("\nVisited map:\n");
    for (auto &r : visited)
    {
        for (auto &i : r)
            std::cout << i << " ";
        std::cout << std::endl;
    }
    printf("\ng-value map:\n");
    for (auto &r : g)
    {
        for (auto &i : r)
            std::cout << i << " ";
        std::cout << std::endl;
    }

    printf("%d Iterations, Duration: %f us\n", itr, duration / 1e3);
    return;
}