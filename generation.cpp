#include <iostream>
#include <vector>
#include "function.h"
#include <stack>
#include <algorithm>
#include <map>
#include <random>
#include <ctime>
#include <set>
#include "function.h"


struct Point {
    size_t x;
    size_t y;
    size_t z;

    Point(size_t x, size_t y, size_t z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void cor(int k = 1) const {
        if (k == 1)
            std::cout << x << ' ' << y << ' ' << z << ' ';
        else
            std::cout << x << ' ' << y << ' ' << z << '\n';
    }

    bool operator<(const Point &right) const {
        if (x < right.x)
            return true;
        else if (x == right.x) {
            if (y < right.y)
                return true;
            else if (y == right.y) {
                return z < right.z;
            }
        }
        return false;
    }
};


auto comp(std::pair<char, int> &a, std::pair<char, int> &b) -> bool {
    return a.second > b.second;
}


//auto gen0(size_t side_x, size_t side_y, size_t side_z, std::vector<float> &vertex, std::vector<int> &index) -> Point {
//    auto vis = std::vector(side_x, std::vector<std::vector<int>>(side_y, std::vector<int>(side_z, false)));
//    auto maze = std::map<Point, std::vector<Point>>();
//    auto st = std::stack<Point>();
//    vis[0][0][0] = true;
//    st.push(Point(0, 0, 0));
//    auto vert_ind = std::map<Point, std::pair<int, int>>();
//    auto k = 0;
//    for (size_t x = 0; x < side_x; x++)
//        for (size_t y = 0; y < side_y; y++)
//            for (size_t z = 0; z < side_z; z++) {
//                vert_ind[Point(x, y, z)] = std::make_pair(k, 0);
//                k++;
//            }
//    vert_ind[Point(0, 0, 0)].second = 1;
//    while (!st.empty()) {
//        auto p = st.top();
//        // выбор направления
//        auto neighbours = std::vector<Point>();
//        if (p.x > 0)
//            if (!vis[p.x - 1][p.y][p.z])
//                neighbours.emplace_back(p.x - 1, p.y, p.z);
//        if (p.x < side_x - 1)
//            if (!vis[p.x + 1][p.y][p.z])
//                neighbours.emplace_back(p.x + 1, p.y, p.z);
//        if (p.y > 0)
//            if (!vis[p.x][p.y - 1][p.z])
//                neighbours.emplace_back(p.x, p.y - 1, p.z);
//        if (p.y < side_y - 1)
//            if (!vis[p.x][p.y + 1][p.z])
//                neighbours.emplace_back(p.x, p.y + 1, p.z);
//        if (p.z > 0)
//            if (!vis[p.x][p.y][p.z - 1])
//                neighbours.emplace_back(p.x, p.y, p.z - 1);
//        if (p.z < side_z - 1)
//            if (!vis[p.x][p.y][p.z + 1])
//                neighbours.emplace_back(p.x, p.y, p.z + 1);
//
//        if (neighbours.empty()) {
//            st.pop();
//        } else {
//            std::random_device rd;
//            std::mt19937 gen(rd());
//            auto num = static_cast<size_t>(gen() % neighbours.size());
//            maze[p].push_back(neighbours[num]);
//            maze[neighbours[num]].push_back(p);
//            vis[neighbours[num].x][neighbours[num].y][neighbours[num].z] = true;
//            st.push(neighbours[num]);
//        }
//    }
//    //заполнение index и vert
//    for (auto item: vert_ind) {
//        vertex.push_back(static_cast<float>(item.first.x));
//        vertex.push_back(static_cast<float>(item.first.y));
//        vertex.push_back(static_cast<float>(item.first.z));
//    }
//    for (const auto &item: maze) {
//        for (auto k: item.second) {
//            index.push_back(vert_ind[item.first].first);
//            index.push_back(vert_ind[k].first);
//        }
//    }
//    cost(Point(0, 0, 0), maze, vert_ind);
//    auto max_cost = 0;
//    auto p = Point(0, 0, 0);
//    for (auto item: vert_ind) {
//        if (item.second.second > max_cost) {
//            max_cost = item.second.second;
//            p = item.first;
//        }
//    }
//    return p;
//}


auto gen(size_t side_x, size_t side_y, size_t side_z, float weightx, float weighty, float weightz,
         std::vector<float> &vertex, std::vector<int> &index) -> Point {
    std::vector<std::pair<char, int>> weights = {{'x', static_cast<int>(weightx * 10)},
                                                 {'y', static_cast<int>(weighty * 10)},
                                                 {'z', static_cast<int>(weightz * 10)}};
    std::cout << weights[0].second << ' ' << weights[1].second << ' ' << weights[2].second << '\n';
    auto vis = std::vector(side_x, std::vector<std::vector<int>>(side_y, std::vector<int>(side_z, false)));
    auto maze = std::map<Point, std::vector<Point>>();
    auto st = std::stack<Point>();
    vis[0][0][0] = true;
    st.push(Point(0, 0, 0));
    auto vert_ind = std::map<Point, std::pair<int, int>>();

    auto k = 0;
    for (size_t x = 0; x < side_x; x++)
        for (size_t y = 0; y < side_y; y++)
            for (size_t z = 0; z < side_z; z++) {
                vert_ind[Point(x, y, z)] = std::make_pair(k, 0);
                k++;
            }
    vert_ind[Point(0, 0, 0)].second = 1;
    while (!st.empty()) {
        auto p = st.top();
        // выбор направления
        auto neighbours = std::vector<Point>();
        auto dir = std::vector<char>();
        auto st_dir = std::set<char>();
        std::random_device rd;
        std::mt19937 gen(rd());

        while (dir.size() < 3) {
            auto coin = gen() % (weights[0].second + weights[1].second + weights[2].second);
            if (coin < weights[0].second && !st_dir.contains('x')) {
                dir.push_back('x');
                st_dir.insert('x');
            }
            if (coin < weights[0].second + weights[0].second && coin >= weights[0].second && !st_dir.contains('y')) {
                dir.push_back('y');
                st_dir.insert('y');
            }
            if (coin >= weights[0].second + weights[1].second && !st_dir.contains('z')) {
                dir.push_back('z');
                st_dir.insert('z');
            }
        }
        if (dir[0] == 'x') {
            if (p.x > 0)
                if (!vis[p.x - 1][p.y][p.z])
                    neighbours.emplace_back(p.x - 1, p.y, p.z);
            if (p.x < side_x - 1)
                if (!vis[p.x + 1][p.y][p.z])
                    neighbours.emplace_back(p.x + 1, p.y, p.z);
        } else if (dir[0] == 'y') {
            if (p.y > 0)
                if (!vis[p.x][p.y - 1][p.z])
                    neighbours.emplace_back(p.x, p.y - 1, p.z);
            if (p.y < side_y - 1)
                if (!vis[p.x][p.y + 1][p.z])
                    neighbours.emplace_back(p.x, p.y + 1, p.z);
        } else {
            if (p.z > 0)
                if (!vis[p.x][p.y][p.z - 1])
                    neighbours.emplace_back(p.x, p.y, p.z - 1);
            if (p.z < side_x - 1)
                if (!vis[p.x][p.y][p.z + 1])
                    neighbours.emplace_back(p.x, p.y, p.z + 1);
        }
        if (neighbours.empty()) {
            if (dir[1] == 'x') {
                if (p.x > 0)
                    if (!vis[p.x - 1][p.y][p.z])
                        neighbours.emplace_back(p.x - 1, p.y, p.z);
                if (p.x < side_x - 1)
                    if (!vis[p.x + 1][p.y][p.z])
                        neighbours.emplace_back(p.x + 1, p.y, p.z);
            } else if (dir[1] == 'y') {
                if (p.y > 0)
                    if (!vis[p.x][p.y - 1][p.z])
                        neighbours.emplace_back(p.x, p.y - 1, p.z);
                if (p.y < side_y - 1)
                    if (!vis[p.x][p.y + 1][p.z])
                        neighbours.emplace_back(p.x, p.y + 1, p.z);
            } else {
                if (p.z > 0)
                    if (!vis[p.x][p.y][p.z - 1])
                        neighbours.emplace_back(p.x, p.y, p.z - 1);
                if (p.z < side_x - 1)
                    if (!vis[p.x][p.y][p.z + 1])
                        neighbours.emplace_back(p.x, p.y, p.z + 1);
            }
        }
        if (neighbours.empty()) {
            if (dir[2] == 'x') {
                if (p.x > 0)
                    if (!vis[p.x - 1][p.y][p.z])
                        neighbours.emplace_back(p.x - 1, p.y, p.z);
                if (p.x < side_x - 1)
                    if (!vis[p.x + 1][p.y][p.z])
                        neighbours.emplace_back(p.x + 1, p.y, p.z);
            } else if (dir[2] == 'y') {
                if (p.y > 0)
                    if (!vis[p.x][p.y - 1][p.z])
                        neighbours.emplace_back(p.x, p.y - 1, p.z);
                if (p.y < side_y - 1)
                    if (!vis[p.x][p.y + 1][p.z])
                        neighbours.emplace_back(p.x, p.y + 1, p.z);
            } else {
                if (p.z > 0)
                    if (!vis[p.x][p.y][p.z - 1])
                        neighbours.emplace_back(p.x, p.y, p.z - 1);
                if (p.z < side_x - 1)
                    if (!vis[p.x][p.y][p.z + 1])
                        neighbours.emplace_back(p.x, p.y, p.z + 1);
            }
        }
        if (neighbours.empty()) {
            st.pop();
        } else {
            std::random_device rd;
            std::mt19937 gen(rd());
            auto num = static_cast<size_t>(gen() % neighbours.size());
            maze[p].push_back(neighbours[num]);
            maze[neighbours[num]].push_back(p);
            vis[neighbours[num].x][neighbours[num].y][neighbours[num].z] = true;
            st.push(neighbours[num]);
        }
    }
    //заполнение index и vert
    for (auto item: vert_ind) {
        vertex.push_back(static_cast<float>(item.first.x));
        vertex.push_back(static_cast<float>(item.first.y));
        vertex.push_back(static_cast<float>(item.first.z));
    }
    for (const auto &item: maze) {
        for (auto k: item.second) {
            index.push_back(vert_ind[item.first].first);
            index.push_back(vert_ind[k].first);
        }
    }
    cost(Point(0, 0, 0), maze, vert_ind);
    auto max_cost = 0;
    auto p = Point(0, 0, 0);
    for (auto item: vert_ind) {
        if (item.second.second > max_cost) {
            max_cost = item.second.second;
            p = item.first;
        }
    }

//    for(size_t i = 0; i < vertex.size(); i++)
//        std::cout<<vertex[i]<<' ';
//    std::cout<<'\n';
//    for(size_t i = 0; i < index.size(); i++)
//        std::cout<<index[i]<<' ';
//    std::cout<<'\n';
    return p;
}

auto cost(Point p0, std::map<Point, std::vector<Point>> &maze, std::map<Point, std::pair<int, int>> &vert_ind) -> void {
    for (auto item: maze[p0]) {
        if (vert_ind[item].second == 0) {
            vert_ind[item].second = vert_ind[p0].second + 1;
            cost(item, maze, vert_ind);
        }
    }
}


template<typename my_type>
auto m_print(std::vector<std::vector<std::vector<my_type>>> &maze) -> void {
    for (size_t i = 0; i < maze.size(); i++) {
        for (size_t j = 0; j < maze[i].size(); j++) {
            for (size_t k = 0; k < maze[i][j].size(); k++)
                std::cout << maze[i][j][k] << ' ';
            std::cout << '\n';
        }
        std::cout << '\n';
    }
}