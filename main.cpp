#include "dfs.h"
#include "dijkstra.h"
#include "astar.h"
#include "astar3D.h"

#define MAP_NUMBER 2

int main()
{
    /* Description of the Grid-
     1--> The cell is not blocked
     0--> The cell is blocked    */
    
    #if MAP_NUMBER == 1
    int y_size = 9, x_size = 10;
    int map[y_size][x_size] =
        {
            {1, 0, 1, 1, 1, 1, 0, 1, 0, 1},
            {0, 0, 1, 0, 1, 1, 1, 0, 0, 1},
            {1, 1, 1, 0, 1, 1, 0, 1, 0, 1},
            {0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
            {1, 1, 1, 0, 1, 1, 1, 0, 1, 0},
            {1, 0, 1, 1, 1, 1, 0, 1, 0, 0},
            {1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
            {1, 0, 1, 1, 1, 1, 0, 1, 1, 1},
            {1, 1, 1, 0, 0, 0, 1, 0, 0, 1}};
    Pair src = std::make_pair(8, 0);
    Pair dest = std::make_pair(8, 2);
    // Pair dest = std::make_pair(0, 9);

    #elif MAP_NUMBER == 2
    int y_size = 4, x_size = 4;
    int map[y_size][x_size] =
        {
            {10, 10, 0, 20},
            {20, 20, 10, 30},
            {20, 10, 0, 10},
            {10, 40, 100, 40}};
    Pair src({3,0});
    Pair dest({0,3});
    std::vector<Pair> target_traj({{0,3},{1,3},{2,3},{3,3},{3,3}});
    // std::vector<Pair> target_traj({{0,3},{1,3},{0,3},{1,3},{2,3}});

    #endif

    //////////////////////////////////////
    // 2D VECTOR ARRAY BASED MAP

    // Converting 2D array to 2D vector
    Grid vec_map;
    for (int i = 0; i < y_size; i++)
    {
        vec_map.push_back(std::vector<int>());
        for (int j = 0; j < x_size; j++)
        {
            vec_map[i].push_back(map[i][j]);
        }
    }

    // Printing 2D vector
    printf("\n2D VECTOR ARRAY BASED MAP:\n");
    for (auto &r : vec_map)
    {
        for (auto &i : r)
            printf("%d ", i);
        printf("\n");
    }
    printf("\n--------------------DFS----------------------\n");
    performDfs(vec_map, src, dest);
    printf("\n--------------------DIJKSTRA----------------------\n");
    dijkstra(vec_map, src, dest);
    printf("\n--------------------A STAR----------------------\n");
    aStarSearch(vec_map, src, dest);
    printf("\n--------------------3D A STAR----------------------\n");
    aStarSearch3D(vec_map, src, target_traj);


    //////////////////////////////////////
    // POINTER BASED MAP

    // Convert 2D vector array into pointer based map
    int *map_ptr = new int[x_size * y_size];
    for (size_t i = 0; i < x_size; i++)
    {
        for (size_t j = 0; j < y_size; j++)
        {
            map_ptr[j * x_size + i] = (int)map[j][i]; // Reverse j and i for v_grid
        }
    }
    // Printing pointer based map
    printf("\n\n\nPOINTER BASED MAP:\n");
    for (size_t y = 1; y <= y_size; y++)
    {
        for (size_t x = 1; x <= x_size; x++)
        {
            printf("%d ", map_ptr[GETMAPINDEX(x, y, x_size, y_size)]);
        }
        printf("\n");
    }
    printf("\n");

    src.first += 1;
    src.second += 1;
    dest.first += 1;
    dest.second += 1;

    // printf("\n--------------------DFS POINTER MAP----------------------\n");
    // performDfsPointerMap(map_ptr, x_size, y_size, src, dest);
    // printf("\n--------------------DIJKSTRA POINTER MAP---------------------\n");
    // dijkstraPointerMap(map_ptr, x_size, y_size, src, dest);
    // printf("\n--------------------A STAR POINTER MAP----------------------\n");
    // aStartSearchPointerMap(map_ptr, x_size, y_size, src, dest);
    delete map_ptr;
    

    return (0);
}