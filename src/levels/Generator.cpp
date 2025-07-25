//#include "Generator.hpp"
//
//
//Generator::Generator() : RNG{std::random_device{}()} {
//    // Create a map of random noise of 1s and 0s
//    initmap();
//    // Apply smoothing and neighbour count rules to generate meaning from the grid
//    for (int i = 0; i < iterations; ++i) {
//        generate();
//    }
//    // Place rock tiles
//    placeObstacles();
//    // Make a path that ensures enemy can at least walk from left side of screen to the right side of the screen
//    carveGuaranteedPath();
//    // Create a vector of coordinates from the map for the enemies to use in traversing the level
//    buildPath();
//}
//
//
//const Generator::MapArray& Generator::getMap()  const {
//    return map;
//}
//
//
//const std::vector<sf::Vector2i>& Generator::getPath() const {
//    return path;
//}
//
//
//void Generator::initmap() {
//    std::uniform_int_distribution<int> dist(0, 99);
//
//    for (int y = 0; y < Rows; y++) {
//        for (int x = 0; x < Cols; x++) {
//            // border is always WALL
//            if (y == 0 || y == Rows - 1 || x == 0 || x == Cols - 1) {
//                map[y][x] = WALL;
//            }
//            else {
//                map[y][x] = (dist(RNG) < fillProbability) ? WALL : FLOOR;
//            }
//        }
//    }
//}
//
//
//void Generator::generate() {
//    MapArray newMap = map;
//    for (int y = 1; y < Rows - 1; y++) {
//        for (int x = 1; x < Cols - 1; x++) {
//            // count walls in radius=1 and radius=2
//            int cnt1 = 0, cnt2 = 0;
//            for (int dy = -2; dy <= 2; ++dy) {
//                for (int dx = -2; dx <= 2; ++dx) {
//                    if (dy == 0 && dx == 0) {
//                        continue;
//                    }
//                    int ny = y + dy;
//                    int nx = x + dx;
//                    bool isWall;
//                    // out of bounds check
//                    if (ny < 0 || ny >= Rows || nx < 0 || nx >= Cols) {
//                        isWall = true;
//                    } else {
//                        if (map[ny][nx] == WALL) {
//                            isWall = true;
//                        } else {
//                            isWall = false;
//                        }
//                    }
//                    // radius‐1 count
//                    if (std::abs(dy) <= 1 && std::abs(dx) <= 1) {
//                        if (isWall) {
//                            ++cnt1;
//                        }
//                    }
//                    // radius‐2 count
//                    if (isWall) {
//                        ++cnt2;
//                    }
//                }
//            }
//            if (cnt1 >= r1Cutoff || cnt2 <= r2Cutoff) {
//                newMap[y][x] = WALL;
//            } else {
//                newMap[y][x] = FLOOR;
//            }
//        }
//    }
//    map.swap(newMap);
//}
//
//
//void Generator::buildPath() {
//    path.clear();
//    int mid = Rows/2;
//    for (int x = 0; x < Cols; ++x) {
//        path.emplace_back(x, mid);
//    }
//}
//
//
//void Generator::placeObstacles() {
//    if (obstacleTile == 0) {
//        return;
//    }
//    for (int y = 0; y < Rows; y++) {
//        for (int x = 0; x < Cols; x++) {
//            if (map[y][x] == FLOOR && randomPercent(obstacleTile)) {
//                map[y][x] = obstacleTileID;
//            }
//        }
//    }
//}
//
//
//bool Generator::randomPercent(int chance) {
//    std::uniform_int_distribution<int> d(1,100);
//    return d(RNG) <= chance;
//}
//
//
//void Generator::carveGuaranteedPath() {
//    int mid = Rows/2;
//    for (int x = 0; x < Cols; x++) {
//        map[mid][x] = FLOOR;
//    }
//}
