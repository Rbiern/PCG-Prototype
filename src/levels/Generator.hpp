#pragma once
#include <iostream>
#include <vector>
#include "SFML/System/Vector2.hpp"


class Generator {
public:
    static constexpr int Rows = 16;
    static constexpr int Cols = 24;

    const int (&getMap() const)[Rows][Cols] {
        return map;
    }

    const std::vector<sf::Vector2i>& getPath() const {
        return path;
    }

private:
    int map[16][24] = {
            {13, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {10, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 0, 1, 6, 1, 0, 0, 5, 3, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 0, 1, 0, 1, 4, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 2, 1, 3, 0, 1, 2, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 13, 16, 12, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 17, 7, 15, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {16, 21, 8, 15, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 13, 16, 16},
            {7, 9, 7, 15, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 17, 7, 8},
            {14, 14, 14, 11, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 17, 9, 7}
    };
    std::vector<sf::Vector2i> listOfAllPaths;

    std::vector<sf::Vector2i> path = {
            {-1, 3}, {0, 3}, {1, 3}, {2, 3}, //right 3
            {2, 2},// up1
            {3, 2}, {4, 2}, {5, 2}, {6, 2}, // right 4
            {6, 3}, {6, 4}, // down 2
            {7, 4},//right 1
            {7, 5}, // down 1
            {8, 5}, {9, 5}, // right 2
            {9, 4}, {9, 3}, // up 2
            {10, 3}, //right 1
            {10, 2}, // up1
            {11, 2}, {12, 2}, {13, 2}, {14, 2}, {15, 2}, {16, 2}, {17, 2}, {18, 2}, {19, 2}// right 2
//            {6, 5}, {6, 6}, {6, 7}, {6, 8}
    };

    std::vector<sf::Vector2i> path2 = {
            {-1, 3}, {0, 3}, {1, 3}, {2, 3}, //right 3
            {2, 2},// up1
            {3, 2}, {4, 2}, {5, 2}, {6, 2}, // right 4
            {6, 3}, {6, 4}, // down 2
            {7, 4},//right 1
            {7, 5}, // down 1
            {8, 5}, {9, 5}, // right 2
            {9, 4}, {9, 3}, // up 2
            {10, 3}, //right 1
            {10, 2}, // up1
            {11, 2}, {12, 2} // right 2
//            {6, 5}, {6, 6}, {6, 7}, {6, 8}
    };
};